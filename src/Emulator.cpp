#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <vector>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "Bus.h"
#include "nesCPU.h"
#include "Renderer.h"

// Global or shared pointer to Bus so callback can access it
Bus* global_nes = nullptr;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    if (global_nes == nullptr) return;

    float* pOut = (float*)pOutput;
    
    std::unique_lock<std::mutex> lock(global_nes->muxAudio);
    
    for (ma_uint32 i = 0; i < frameCount; ++i) {
        float sample = 0.0f;
        if (!global_nes->audioSamples.empty()) {
            sample = global_nes->audioSamples.front();
            global_nes->audioSamples.pop_front();
        }
        pOut[i] = sample; 
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <rom_file>" << std::endl;
        return 1;
    }
    
    std::string rom = argv[1];
    
    Bus nes;
    global_nes = &nes;
    
    std::shared_ptr<Cartridge> cart = std::make_shared<Cartridge>(rom);
    
    if (!cart->ImageValid()) {
        std::cout << "Failed to load ROM: " << rom << std::endl;
        return 1;
    }
    
    nes.insertCartridge(cart);
    nes.reset();
    nes.SetSampleFrequency(44100);
    
    // Audio Setup
    ma_context context;
    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
        std::cout << "Failed to initialize miniaudio context." << std::endl;
        return -1;
    }

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate        = 44100;
    config.dataCallback      = data_callback;
    config.pUserData         = nullptr;

    ma_device device;
    if (ma_device_init(&context, &config, &device) != MA_SUCCESS) {
        std::cout << "Failed to initialize miniaudio device." << std::endl;
        ma_context_uninit(&context);
        return -1;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        std::cout << "Failed to start miniaudio device." << std::endl;
        ma_device_uninit(&device);
        ma_context_uninit(&context);
        return -1;
    }
    
    // Width 256, Height 240, Scale 2
    Renderer renderer(256, 240, 2);
    if (!renderer.Init()) {
        ma_device_uninit(&device);
        ma_context_uninit(&context);
        return 1;
    }
    
    bool bEmulationRun = true;
    float fResidualTime = 0.0f;
    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    
    while (!renderer.ShouldClose()) {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();
        
        renderer.PollEvents();
        
        nes.controller[0] = 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_F) ? 0x80 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_G) ? 0x40 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_BACKSPACE) ? 0x20 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_ENTER) ? 0x10 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_W) ? 0x08 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_S) ? 0x04 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_A) ? 0x02 : 0x00;
        nes.controller[0] |= renderer.IsKeyHeld(GLFW_KEY_D) ? 0x01 : 0x00;
        
        if (renderer.IsKeyHeld(GLFW_KEY_R)) nes.reset();
        
        if (bEmulationRun) {
            if (fResidualTime > 0.0f)
                fResidualTime -= fElapsedTime;
            else {
                fResidualTime += (1.0f / 60.0f) - fElapsedTime;
                do {
                    nes.clock();
                } while (!nes.ppu.frame_complete);
                nes.ppu.frame_complete = false;
            }
        }
        
        renderer.Draw(nes.ppu.GetScreen());
    }
    
    ma_device_uninit(&device);
    ma_context_uninit(&context);
    return 0;
}
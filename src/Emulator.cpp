#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory>

#include "Bus.h"
#include "nesCPU.h"
#include "Renderer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <rom_file>" << std::endl;
        return 1;
    }
    
    std::string rom = argv[1];
    
    Bus nes;
    std::shared_ptr<Cartridge> cart = std::make_shared<Cartridge>(rom);
    
    if (!cart->ImageValid()) {
        std::cout << "Failed to load ROM: " << rom << std::endl;
        return 1;
    }
    
    nes.insertCartridge(cart);
    nes.reset();
    
    // Width 256, Height 240, Scale 2
    Renderer renderer(256, 240, 2);
    if (!renderer.Init()) {
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
        
        // Simple reset handling (will reset every frame if held, which is acceptable for now)
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
    
    return 0;
}
#include <iostream>
#include <sstream>
#include <string>

#include "Bus.h"
#include "nesCPU.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

std::string rom;

class Emulator : public olc::PixelGameEngine {
   public:
    Emulator() { sAppName = "NES Emulator"; }

   private:
    Bus nes;
    std::shared_ptr<Cartridge> cart;
    bool bEmulationRun = true;
    float fResidualTime = 0.0f;
    static Emulator* instance;

    bool OnUserCreate() {
        cart = std::make_shared<Cartridge>(rom);

        if (!cart->ImageValid()) return false;

        nes.insertCartridge(cart);
        instance = this;
        nes.setSampleFreq(44100);
        // initialize audio

        // set user synth

        nes.reset();
        return true;
    }

    static float soundOut(int channel, float globalTime, float timeSpent) {
        while (!instance->nes.clock()) {
        }
        return static_cast<float>(instance->nes.audioSample);
    }

    bool OnUserDestroy() override {
        // destroy audio
        return true;
    }

    bool audio(float fElapsedTime) {
        nes.controller[0] = 0x00;
        nes.controller[0] |= GetKey(olc::Key::F).bHeld ? 0x80 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::G).bHeld ? 0x40 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::BACK).bHeld ? 0x20 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::ENTER).bHeld ? 0x10 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::W).bHeld ? 0x08 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x04 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x02 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::D).bHeld ? 0x01 : 0x00;

        if (GetKey(olc::Key::R).bPressed) nes.reset();

        DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        audio(fElapsedTime);
        return true;
    }

    bool emulatorWithoutAudio(float fElapsedTime) {
        nes.controller[0] = 0x00;
        nes.controller[0] |= GetKey(olc::Key::F).bHeld ? 0x80 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::G).bHeld ? 0x40 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::BACK).bHeld ? 0x20 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::ENTER).bHeld ? 0x10 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::W).bHeld ? 0x08 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x04 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x02 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::D).bHeld ? 0x01 : 0x00;

        if (GetKey(olc::Key::R).bPressed) nes.reset();

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

        DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
        return true;
    }
};

Emulator* Emulator::instance = nullptr;

int main(int argc, char* argv[]) {
    rom = argv[1];
    Emulator demo;
    demo.Construct(512, 480, 2, 2);
    demo.Start();
    return 0;
}
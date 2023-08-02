#include <iostream>
#include <sstream>

#include "Bus.h"
#include "nesCPU.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Emulator : public olc::PixelGameEngine {
public:
	Emulator() { sAppName = "NES Emulator"; }

private:
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	bool bEmulationRun = true;
	float fResidualTime = 0.0f;

	bool OnUserCreate() {
		cart = std::make_shared<Cartridge>("../mario.nes");

		if (!cart->ImageValid())
			return false;

		nes.insertCartridge(cart);

		nes.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

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

		if (bEmulationRun)
		{
			if (fResidualTime > 0.0f)
				fResidualTime -= fElapsedTime;
			else
			{
				fResidualTime += (1.0f / 60.0f) - fElapsedTime;
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}
		}
		
		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
		return true;
	}
};

int main() {
	Emulator demo;
	demo.Construct(512, 480, 2, 2);
	demo.Start();
	return 0;
}
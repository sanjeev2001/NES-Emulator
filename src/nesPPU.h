#pragma once

#include <cstdint>
#include <memory>

#include "cartridge.h"
#include "olcPixelGameEngine.h"

class nesPPU {
private:
    std::shared_ptr<Cartridge> cartridge;
    uint8_t     tblName[2][1024] = {};
    uint8_t     tblPattern[2][4096] = {};
    uint8_t		tblPalette[32] = {};

    olc::Pixel palScreen[0x40];
    // olc::Sprite sprScreen = olc::Sprite(256, 240);
    // olc::Sprite sprNameTable[2] = {olc::Sprite(256, 240),
    //                                olc::Sprite(256, 240)};
    // olc::Sprite sprPatternTable[2] = {olc::Sprite(128, 128),
    //                                   olc::Sprite(128, 128)};
    olc::Sprite* sprScreen;
    olc::Sprite* sprNameTable[2];
    olc::Sprite* sprPatternTable[2];

    int16_t scanline = 0;
    int16_t cycle = 0;

public:
    nesPPU();
    ~nesPPU();

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    void connectCartridge(const std::shared_ptr<Cartridge>& cart);
    void clock();

    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t i);
    olc::Sprite& GetPatternTable(uint8_t i);
    bool frame_complete = false;
};

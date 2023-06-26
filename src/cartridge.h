#pragma once

#include <cstdint>

class Cartridge {
   private:
    /* data */
   public:
    Cartridge(/* args */);
    ~Cartridge();

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    uint8_t cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    uint8_t ppuWrite(uint16_t addr, uint8_t data);
};

Cartridge::Cartridge(/* args */) {}

Cartridge::~Cartridge() {}

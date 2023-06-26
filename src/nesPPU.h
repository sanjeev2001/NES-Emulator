#pragma once

#include <cstdint>
#include <memory>

#include "cartridge.h"

class nesPPU {
   private:
    std::shared_ptr<Cartridge> cartridge;

   public:
    nesPPU(/* args */);
    ~nesPPU();

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    uint8_t cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    uint8_t ppuWrite(uint16_t addr, uint8_t data);

    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
};

nesPPU::nesPPU(/* args */) {}

nesPPU::~nesPPU() {}

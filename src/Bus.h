#pragma once
#include <array>
#include <cstdint>
#include <memory>

#include "cartridge.h"
#include "nesCPU.h"
#include "nesPPU.h"

class Bus {
   public:
    Bus();
    ~Bus();

    void cpuWrite(uint16_t address, uint8_t data);
    uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

    nesCPU cpu;
    nesPPU ppu;
    std::shared_ptr<Cartridge> cartridge;

    std::array<uint8_t, 2048> cpuRAM;

   private:
    uint32_t clockCounter = 0;
};
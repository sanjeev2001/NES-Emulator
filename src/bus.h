#pragma once
#include <array>
#include <cstdint>

#include "nesCPU.h"

class Bus {
   public:
    Bus();
    ~Bus();

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool bReadOnly = false);

    nesCPU cpu;

    std::array<uint8_t, 64 * 1024> ram;
};
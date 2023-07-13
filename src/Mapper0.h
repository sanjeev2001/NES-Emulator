#pragma once
#include "Mapper.h"

class Mapper0 : public Mapper {
public:
    Mapper0(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper0();

    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
};
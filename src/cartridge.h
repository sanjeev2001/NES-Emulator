#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Mapper0.h"

class Cartridge {
   private:
    bool bImageValid = false;
    std::vector<uint8_t> prgMemory;
    std::vector<uint8_t> chrMemory;

    uint8_t mapperID = 0;
    uint8_t prgBanks = 0;
    uint8_t chrBanks = 0;

    std::shared_ptr<Mapper> pMapper;

   public:
    Cartridge(const std::string &fileName);
    ~Cartridge();

    bool ImageValid();

    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);
    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);
};

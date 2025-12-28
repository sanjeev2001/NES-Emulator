#include "Cartridge.h"
#include <iostream>
#include <fstream>

Cartridge::Cartridge(const std::string& fileName) {
    // iNES Format Header
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);
    if (ifs.is_open()) {
        ifs.read((char*)&header, sizeof(sHeader));

        if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

        mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t nFileType = 1;

        if (nFileType == 0) {
        }
        if (nFileType == 1) {
            prgBanks = header.prg_rom_chunks;
            prgMemory.resize(prgBanks * 16384);
            ifs.read((char*)prgMemory.data(), prgMemory.size());

            chrBanks = header.chr_rom_chunks;
            if (chrBanks == 0) {
                // Create 8K RAM for CHR
                chrMemory.resize(8192);
            } else {
                chrMemory.resize(chrBanks * 8192);
                ifs.read((char*)chrMemory.data(), chrMemory.size());
            }
        }
        if (nFileType == 2) {
        }

        switch (mapperID) {
        case 0:
            pMapper = std::make_shared<Mapper0>(prgBanks, chrBanks);
            break;
        }

        bImageValid = true;
        ifs.close();
    }
}

Cartridge::~Cartridge() {
}

bool Cartridge::ImageValid() {
    return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        if (mapped_addr == 0xFFFFFFFF) {
            return true;
        }
        data = prgMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr)) {
        if (mapped_addr == 0xFFFFFFFF) {
            return true;
        }
        prgMemory[mapped_addr] = data;
        return true;
    }
    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = chrMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapWrite(addr, mapped_addr)) {
        chrMemory[mapped_addr] = data;
        return true;
    }
    return false;
}

void Cartridge::reset() {
    if (pMapper != nullptr)
        pMapper->reset();
}

#include "cartridge.h"

Cartridge::Cartridge(const std::string& fileName) {
    struct sHeader {
        char name[4];
        uint8_t prgChunks;
        uint8_t chrChunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prgRamSize;
        uint8_t system1;
        uint8_t system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);

    if (ifs.is_open()) {
        ifs.read((char*)&header, sizeof(sHeader));

        if (header.mapper1 & 0x04) {
            ifs.seekg(512, std::ios_base::cur);
        }

        mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t fileType = 1;

        if (fileType == 0) {
        }
        else if (fileType == 1) {
            prgBanks = header.prgChunks;
            prgMemory.resize(prgBanks * 16384);
            ifs.read((char*)prgMemory.data(), prgMemory.size());

            chrBanks = header.chrChunks;
            chrMemory.resize(chrBanks * 8192);
            ifs.read((char*)chrMemory.data(), chrMemory.size());

        }
        else if (fileType == 2) {
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
Cartridge::~Cartridge() {}

bool Cartridge::ImageValid() { return bImageValid; }

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        data = prgMemory[mapped_addr];
        return true;
    }
    else {
        return false;
    }
}
bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr)) {
        prgMemory[mapped_addr] = data;
        return true;
    }
    else {
        return false;
    }
}
bool Cartridge::ppuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = chrMemory[mapped_addr];
        return true;
    }
    else {
        return false;
    }
}
bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        chrMemory[mapped_addr] = data;
        return true;
    }
    else {
        return false;
    }
}
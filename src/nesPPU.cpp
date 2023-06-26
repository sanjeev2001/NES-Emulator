#include "nesPPU.h"

#include <memory>

nesPPU::nesPPU() {}

nesPPU::~nesPPU() {}

uint8_t nesPPU::cpuRead(uint16_t addr, bool readOnly = false) {
    uint8_t data = 0x00;

    switch (addr) {
        case 0x0000:  // PPUCTRL
            break;
        case 0x0001:  // PPUMASK
            break;
        case 0x0002:  // PPUSTATUS
            break;
        case 0x0003:  // OAMADDR
            break;
        case 0x0004:  // OAMDATA
            break;
        case 0x0005:  // PPUSCROLL
            break;
        case 0x0006:  // PPUADDR
            break;
        case 0x0007:  // PPUDATA
            break;
    }
    return data;
}
uint8_t nesPPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:  // PPUCTRL
            break;
        case 0x0001:  // PPUMASK
            break;
        case 0x0002:  // PPUSTATUS
            break;
        case 0x0003:  // OAMADDR
            break;
        case 0x0004:  // OAMDATA
            break;
        case 0x0005:  // PPUSCROLL
            break;
        case 0x0006:  // PPUADDR
            break;
        case 0x0007:  // PPUDATA
            break;
    }
    return data;
}
uint8_t nesPPU::ppuRead(uint16_t addr, bool readOnly = false) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    return data;
}
uint8_t nesPPU::ppuWrite(uint16_t addr, uint8_t data) { addr &= 0x3FFF; }

void nesPPU::connectCartridge(const std::shared_ptr<Cartridge>& cart) {
    this->cartridge = cartridge;
}
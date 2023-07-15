#include "Bus.h"

Bus::Bus() { cpu.connectBus(this); }

Bus::~Bus() {}

void Bus::cpuWrite(uint16_t address, uint8_t data) {
    if (cartridge->cpuWrite(address, data)) {
    } else if (address >= 0x0000 && address <= 0x1FFF) {
        cpuRAM[address & 0x07FF] = data;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.cpuWrite(address & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly) {
    uint8_t data = 0x00;

    if (cartridge->cpuRead(address, data)) {
    } else if (address >= 0x0000 && address <= 0x1FFF) {
        data = cpuRAM[address & 0x07FF];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.cpuRead(address & 0x0007, bReadOnly);
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cartridge = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset() {
    cpu.reset();
    clockCounter = 0;
}

void Bus::clock() {
    ppu.clock();
    if (clockCounter % 3 == 0) {
        cpu.clock();
    }
    clockCounter++;
}
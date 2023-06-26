#include "bus.h"

Bus::Bus() {
    for (auto& i : cpuRAM) {
        i = 0x00;
    }

    cpu.connectBus(this);
}

Bus::~Bus() {}

void Bus::cpuWrite(uint16_t address, uint8_t data) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        cpuRAM[address & 0x07FF] = data;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.cpuWrite(address & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly) {
    uint8_t data = 0x00;

    if (address >= 0x0000 && address <= 0x1FFF) {
        data = cpuRAM[address & 0x07FF];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.cpuRead(address & 0x0007, bReadOnly);
    }

    return 0x00;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cartridge = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset() {
    cpu.reset();
    clockCounter = 0;
}

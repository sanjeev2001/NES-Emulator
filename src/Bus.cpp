#include "Bus.h"

Bus::Bus() {
    cpu.connectBus(this);
}

Bus::~Bus() {
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (cartridge->cpuWrite(addr, data)) {
        // Cartridge handled write
    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        cpuRAM[addr & 0x07FF] = data;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpuWrite(addr & 0x0007, data);
    } else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017) {
        apu.cpuWrite(addr, data);
    } else if (addr == 0x4014) {
        dmaPage = data;
        dmaAddress = 0x00;
        dmaTransfer = true;
    } else if (addr >= 0x4016 && addr <= 0x4017) {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;
    if (cartridge->cpuRead(addr, data)) {
        // Cartridge handled read
    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        data = cpuRAM[addr & 0x07FF];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    } else if (addr == 0x4015) {
        data = apu.cpuRead(addr);
    } else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
    }
    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cartridge = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset() {
    cartridge->reset();
    cpu.reset();
    ppu.reset();
    apu.reset();
    systemClockCounter = 0;
    dmaPage = 0x00;
    dmaAddress = 0x00;
    dmaData = 0x00;
    dmaTransfer = false;
    dmaDummy = true;
}

void Bus::SetSampleFrequency(uint32_t sample_rate) {
    dCyclesPerSample = 1789773.0 / (double)sample_rate;
}

void Bus::clock() {
    ppu.clock();

    if (systemClockCounter % 3 == 0) {
        apu.clock();
        
        if (dmaTransfer) {
            if (dmaDummy) {
                if (systemClockCounter % 2 == 1) {
                    dmaDummy = false;
                }
            } else {
                if (systemClockCounter % 2 == 0) {
                    dmaData = cpuRead(dmaPage << 8 | dmaAddress);
                } else {
                    ppu.pOAM[dmaAddress] = dmaData;
                    dmaAddress++;
                    if (dmaAddress == 0x00) {
                        dmaTransfer = false;
                        dmaDummy = true;
                    }
                }
            }
        } else {
            cpu.clock();
        }
        
        dCycleCounter += 1.0;
        if (dCycleCounter >= dCyclesPerSample) {
            dCycleCounter -= dCyclesPerSample;
            audioSample = apu.GetOutputSample();
            if (audioSamples.size() < 4096) {
                audioSamples.push_back((float)audioSample);
            }
        }
    }

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nonMaskedInterruptRequest();
    }
    
    systemClockCounter++;
}

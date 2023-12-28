#include "Bus.h"

Bus::Bus() { cpu.connectBus(this); }

Bus::~Bus() {}

void Bus::cpuWrite(uint16_t address, uint8_t data) {
    if (cartridge->cpuWrite(address, data)) {
    } else if (address >= 0x0000 && address <= 0x1FFF) {
        cpuRAM[address & 0x07FF] = data;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.cpuWrite(address & 0x0007, data);
    } else if ((address >= 0x4000 && address <= 0x4013) || address == 0x4015 ||
               address == 0x4017) {
        apu.cpuWrite(address, data);
    } else if (address == 0x4014) {
        dmaPage = data;
        dmaAddress = 0x00;
        dmaTransfer = true;
    } else if (address >= 0x4016 && address <= 0x4017) {
        controller_state[address & 0x0001] = controller[address & 0x0001];
    }
}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly) {
    uint8_t data = 0x00;

    if (cartridge->cpuRead(address, data)) {
    } else if (address >= 0x0000 && address <= 0x1FFF) {
        data = cpuRAM[address & 0x07FF];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.cpuRead(address & 0x0007, bReadOnly);

    } else if (address >= 0x4016 && address <= 0x4017) {
        data = (controller_state[address & 0x0001] & 0x80) > 0;
        controller_state[address & 0x0001] <<= 1;
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

bool Bus::clock() {
    ppu.clock();

    apu.clock();
    if (clockCounter % 3 == 0) {
        if (dmaTransfer) {
            if (dmaDummy) {
                if (clockCounter % 2 == 1) {
                    dmaDummy = false;
                }
            } else {
                if (clockCounter % 2 == 0) {
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
    }

    bool audioSampleReady = false;
    audioTime += audioTimePerNESClock;

    if (audioTime >= audioTimePerSystemSample) {
        audioTime -= audioTimePerSystemSample;
        audioSample = apu.getOutputSample();
        audioSampleReady = true;
    }

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nonMaskedInterruptRequest();
    }

    clockCounter++;
    return audioSampleReady;
}

void Bus::setSampleFreq(uint32_t sampleRate) {
    audioTimePerSystemSample = 1.0 / (double)sampleRate;
    audioTimePerNESClock =
        1.0 / 5369318.0;  // 21.4MHz/4 -> known ppu clock freq
}

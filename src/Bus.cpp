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
    audioTimePerSystemSample = 1.0 / (double)sample_rate;
    audioTimePerNESClock = 1.0 / 5369318.0; // Approx PPU clock freq
}

void Bus::clock() {
    ppu.clock();
    apu.clock();

    if (systemClockCounter % 3 == 0) {
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
    }

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nonMaskedInterruptRequest();
    }
    
    // Audio Synchronization
    audioTime += audioTimePerNESClock;
    if (audioTime >= audioTimePerSystemSample) {
        audioTime -= audioTimePerSystemSample;
        audioSample = apu.GetOutputSample();
        
        // Push sample to thread-safe queue
        // Limit size to prevent memory explosion if audio is stuck
        if (audioSamples.size() < 4096) { 
             // std::unique_lock<std::mutex> lock(muxAudio); // Locking every sample (44100 times/sec) is expensive but safe-ish for now
             // Optimization: We will lock in batch or main loop, but here is unsafe without lock.
             // However, locking here inside `clock()` which runs 5MHz is BAD.
             // BETTER: Don't lock here. Just push. The queue is only accessed by main thread filling it.
             // Wait, who empties it? The AUDIO THREAD. So we NEED locking.
             // To avoid locking 5 million times a second (clock is called a lot), we only lock when we PUSH (44k times).
             // 44k locks/sec is acceptable on modern CPUs.
             std::lock_guard<std::mutex> lock(muxAudio);
             audioSamples.push_back((float)audioSample);
        }
    }
    
    systemClockCounter++;
}

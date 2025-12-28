#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <deque>
#include <mutex>

#include "Cartridge.h"
#include "nesCPU.h"
#include "nesPPU.h"
#include "nesAPU.h"

class Bus {
   public:
    Bus();
    ~Bus();

    void cpuWrite(uint16_t address, uint8_t data);
    uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();
    
    // System Clock Counter
    uint32_t systemClockCounter = 0;
    
    // Devices
    nesCPU cpu;
    nesPPU ppu;
    nesAPU apu;
    std::shared_ptr<Cartridge> cartridge;

    std::array<uint8_t, 2048> cpuRAM;
    uint8_t controller[2];
    
    // DMA
    uint8_t dmaPage = 0x00;
    uint8_t dmaAddress = 0x00;
    uint8_t dmaData = 0x00;
    bool dmaTransfer = false;
    bool dmaDummy = true;
    
    void SetSampleFrequency(uint32_t sample_rate);

    // Thread-safe audio access
    std::deque<float> audioSamples;
    std::mutex muxAudio;
    
   private:
    // Audio
    double dCyclesPerSample = 0.0;
    double dCycleCounter = 0.0;
    double audioSample = 0.0;
    
    uint8_t controller_state[2];
};
#pragma once
#include <array>
#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "nesAPU.h"
#include "nesCPU.h"
#include "nesPPU.h"

class Bus {
   public:
    Bus();
    ~Bus();

    void cpuWrite(uint16_t address, uint8_t data);
    uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    bool clock();

    nesCPU cpu;
    nesPPU ppu;
    nesAPU apu;
    std::shared_ptr<Cartridge> cartridge;

    std::array<uint8_t, 2048> cpuRAM;
    uint8_t controller[2];

    double audioSample = 0.0;
    void setSampleFreq(uint32_t sampleRate);

   private:
    uint32_t clockCounter = 0;
    uint8_t controller_state[2];
    uint8_t dmaPage = 0x00;
    uint8_t dmaAddress = 0x00;
    uint8_t dmaData = 0x00;
    bool dmaTransfer = false;
    bool dmaDummy = true;

    double audioTimePerSystemSample = 0.0f;
    double audioTimePerNESClock = 0.0f;
    double audioTime = 0.0;
};
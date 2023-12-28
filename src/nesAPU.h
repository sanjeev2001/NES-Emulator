#pragma once

#include <cstdint>
#include <functional>

class nesAPU {
   public:
    nesAPU();
    ~nesAPU();

   public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    void clock();
    void reset();

    double getOutputSample();

   private:
    bool pulse1_en = false;
    double pulse1_sample = 0.0;
};
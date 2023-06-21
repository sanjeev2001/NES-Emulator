#include "nesCPU.h"

#include <cstdint>

#include "bus.h"

nesCPU::nesCPU() {
    using a = nesCPU;
    lookup = {
        {"ADC", &a::ADC, &a::IMM, 2}, {"ADC", &a::ADC, &a::ZP0, 3},
        {"ADC", &a::ADC, &a::ZPX, 4}, {"ADC", &a::ADC, &a::ABS, 3},
        {"ADC", &a::ADC, &a::ABX, 4}, {"ADC", &a::ADC, &a::ABY, 4},
        {"ADC", &a::ADC, &a::IZX, 6}, {"ADC", &a::ADC, &a::IZY, 5},
        {"AND", &a::AND, &a::IMM, 2}, {"AND", &a::AND, &a::ZP0, 3},
        {"AND", &a::AND, &a::ZPX, 4}, {"AND", &a::AND, &a::ABS, 3},
        {"AND", &a::AND, &a::ABX, 4}, {"AND", &a::AND, &a::ABY, 4},
        {"AND", &a::AND, &a::IZX, 6}, {"AND", &a::AND, &a::IZY, 5},
        {"ASL", &a::ASL, &a::ZP0, 5}, {"ASL", &a::ASL, &a::ZPX, 6},
        {"ASL", &a::ASL, &a::ABS, 6}, {"ASL", &a::ASL, &a::ABX, 7},
        {"BIT", &a::BIT, &a::ZP0, 3}, {"BIT", &a::BIT, &a::ABS, 4},
        {"BRK", &a::BRK, &a::IMP, 7}, {"???", &a::XXX, &a::IMP, 3},
        {"CMP", &a::CMP, &a::IMM, 2}, {"CMP", &a::CMP, &a::ZP0, 3},
        {"CMP", &a::CMP, &a::ZPX, 4}, {"CMP", &a::CMP, &a::ABS, 4},
        {"CMP", &a::CMP, &a::ABX, 4}, {"CMP", &a::CMP, &a::ABY, 4},
        {"CMP", &a::CMP, &a::IZX, 6}, {"CMP", &a::CMP, &a::IZY, 5},
    };
}

nesCPU::~nesCPU() {}

uint8_t nesCPU::read(uint16_t address) { return bus->read(address, false); }

void nesCPU::write(uint16_t address, uint8_t data) {
    bus->write(address, data);
}
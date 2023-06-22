#include "nesCPU.h"

#include <cstdint>

#include "bus.h"

nesCPU::nesCPU() {  // ??? refers to unused/unofficial opcodes
    lookup = {
        {"BRK", &nesCPU::BRK, &nesCPU::IMM, 7},
        {"ORA", &nesCPU::ORA, &nesCPU::IZX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 3},
        {"ORA", &nesCPU::ORA, &nesCPU::ZP0, 3},
        {"ASL", &nesCPU::ASL, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"PHP", &nesCPU::PHP, &nesCPU::IMP, 3},
        {"ORA", &nesCPU::ORA, &nesCPU::IMM, 2},
        {"ASL", &nesCPU::ASL, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"ORA", &nesCPU::ORA, &nesCPU::ABS, 4},
        {"ASL", &nesCPU::ASL, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BPL", &nesCPU::BPL, &nesCPU::REL, 2},
        {"ORA", &nesCPU::ORA, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"ORA", &nesCPU::ORA, &nesCPU::ZPX, 4},
        {"ASL", &nesCPU::ASL, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"CLC", &nesCPU::CLC, &nesCPU::IMP, 2},
        {"ORA", &nesCPU::ORA, &nesCPU::ABY, 4},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"ORA", &nesCPU::ORA, &nesCPU::ABX, 4},
        {"ASL", &nesCPU::ASL, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"JSR", &nesCPU::JSR, &nesCPU::ABS, 6},
        {"AND", &nesCPU::AND, &nesCPU::IZX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"BIT", &nesCPU::BIT, &nesCPU::ZP0, 3},
        {"AND", &nesCPU::AND, &nesCPU::ZP0, 3},
        {"ROL", &nesCPU::ROL, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"PLP", &nesCPU::PLP, &nesCPU::IMP, 4},
        {"AND", &nesCPU::AND, &nesCPU::IMM, 2},
        {"ROL", &nesCPU::ROL, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"BIT", &nesCPU::BIT, &nesCPU::ABS, 4},
        {"AND", &nesCPU::AND, &nesCPU::ABS, 4},
        {"ROL", &nesCPU::ROL, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BMI", &nesCPU::BMI, &nesCPU::REL, 2},
        {"AND", &nesCPU::AND, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"AND", &nesCPU::AND, &nesCPU::ZPX, 4},
        {"ROL", &nesCPU::ROL, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"SEC", &nesCPU::SEC, &nesCPU::IMP, 2},
        {"AND", &nesCPU::AND, &nesCPU::ABY, 4},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"AND", &nesCPU::AND, &nesCPU::ABX, 4},
        {"ROL", &nesCPU::ROL, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"RTI", &nesCPU::RTI, &nesCPU::IMP, 6},
        {"EOR", &nesCPU::EOR, &nesCPU::IZX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 3},
        {"EOR", &nesCPU::EOR, &nesCPU::ZP0, 3},
        {"LSR", &nesCPU::LSR, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"PHA", &nesCPU::PHA, &nesCPU::IMP, 3},
        {"EOR", &nesCPU::EOR, &nesCPU::IMM, 2},
        {"LSR", &nesCPU::LSR, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"JMP", &nesCPU::JMP, &nesCPU::ABS, 3},
        {"EOR", &nesCPU::EOR, &nesCPU::ABS, 4},
        {"LSR", &nesCPU::LSR, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BVC", &nesCPU::BVC, &nesCPU::REL, 2},
        {"EOR", &nesCPU::EOR, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"EOR", &nesCPU::EOR, &nesCPU::ZPX, 4},
        {"LSR", &nesCPU::LSR, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"CLI", &nesCPU::CLI, &nesCPU::IMP, 2},
        {"EOR", &nesCPU::EOR, &nesCPU::ABY, 4},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"EOR", &nesCPU::EOR, &nesCPU::ABX, 4},
        {"LSR", &nesCPU::LSR, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"RTS", &nesCPU::RTS, &nesCPU::IMP, 6},
        {"ADC", &nesCPU::ADC, &nesCPU::IZX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 3},
        {"ADC", &nesCPU::ADC, &nesCPU::ZP0, 3},
        {"ROR", &nesCPU::ROR, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"PLA", &nesCPU::PLA, &nesCPU::IMP, 4},
        {"ADC", &nesCPU::ADC, &nesCPU::IMM, 2},
        {"ROR", &nesCPU::ROR, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"JMP", &nesCPU::JMP, &nesCPU::IND, 5},
        {"ADC", &nesCPU::ADC, &nesCPU::ABS, 4},
        {"ROR", &nesCPU::ROR, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BVS", &nesCPU::BVS, &nesCPU::REL, 2},
        {"ADC", &nesCPU::ADC, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"ADC", &nesCPU::ADC, &nesCPU::ZPX, 4},
        {"ROR", &nesCPU::ROR, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"SEI", &nesCPU::SEI, &nesCPU::IMP, 2},
        {"ADC", &nesCPU::ADC, &nesCPU::ABY, 4},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"ADC", &nesCPU::ADC, &nesCPU::ABX, 4},
        {"ROR", &nesCPU::ROR, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"STA", &nesCPU::STA, &nesCPU::IZX, 6},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"STY", &nesCPU::STY, &nesCPU::ZP0, 3},
        {"STA", &nesCPU::STA, &nesCPU::ZP0, 3},
        {"STX", &nesCPU::STX, &nesCPU::ZP0, 3},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 3},
        {"DEY", &nesCPU::DEY, &nesCPU::IMP, 2},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"TXA", &nesCPU::TXA, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"STY", &nesCPU::STY, &nesCPU::ABS, 4},
        {"STA", &nesCPU::STA, &nesCPU::ABS, 4},
        {"STX", &nesCPU::STX, &nesCPU::ABS, 4},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"BCC", &nesCPU::BCC, &nesCPU::REL, 2},
        {"STA", &nesCPU::STA, &nesCPU::IZY, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"STY", &nesCPU::STY, &nesCPU::ZPX, 4},
        {"STA", &nesCPU::STA, &nesCPU::ZPX, 4},
        {"STX", &nesCPU::STX, &nesCPU::ZPY, 4},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"TYA", &nesCPU::TYA, &nesCPU::IMP, 2},
        {"STA", &nesCPU::STA, &nesCPU::ABY, 5},
        {"TXS", &nesCPU::TXS, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 5},
        {"STA", &nesCPU::STA, &nesCPU::ABX, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"LDY", &nesCPU::LDY, &nesCPU::IMM, 2},
        {"LDA", &nesCPU::LDA, &nesCPU::IZX, 6},
        {"LDX", &nesCPU::LDX, &nesCPU::IMM, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"LDY", &nesCPU::LDY, &nesCPU::ZP0, 3},
        {"LDA", &nesCPU::LDA, &nesCPU::ZP0, 3},
        {"LDX", &nesCPU::LDX, &nesCPU::ZP0, 3},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 3},
        {"TAY", &nesCPU::TAY, &nesCPU::IMP, 2},
        {"LDA", &nesCPU::LDA, &nesCPU::IMM, 2},
        {"TAX", &nesCPU::TAX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"LDY", &nesCPU::LDY, &nesCPU::ABS, 4},
        {"LDA", &nesCPU::LDA, &nesCPU::ABS, 4},
        {"LDX", &nesCPU::LDX, &nesCPU::ABS, 4},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"BCS", &nesCPU::BCS, &nesCPU::REL, 2},
        {"LDA", &nesCPU::LDA, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"LDY", &nesCPU::LDY, &nesCPU::ZPX, 4},
        {"LDA", &nesCPU::LDA, &nesCPU::ZPX, 4},
        {"LDX", &nesCPU::LDX, &nesCPU::ZPY, 4},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"CLV", &nesCPU::CLV, &nesCPU::IMP, 2},
        {"LDA", &nesCPU::LDA, &nesCPU::ABY, 4},
        {"TSX", &nesCPU::TSX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"LDY", &nesCPU::LDY, &nesCPU::ABX, 4},
        {"LDA", &nesCPU::LDA, &nesCPU::ABX, 4},
        {"LDX", &nesCPU::LDX, &nesCPU::ABY, 4},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 4},
        {"CPY", &nesCPU::CPY, &nesCPU::IMM, 2},
        {"CMP", &nesCPU::CMP, &nesCPU::IZX, 6},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"CPY", &nesCPU::CPY, &nesCPU::ZP0, 3},
        {"CMP", &nesCPU::CMP, &nesCPU::ZP0, 3},
        {"DEC", &nesCPU::DEC, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"INY", &nesCPU::INY, &nesCPU::IMP, 2},
        {"CMP", &nesCPU::CMP, &nesCPU::IMM, 2},
        {"DEX", &nesCPU::DEX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"CPY", &nesCPU::CPY, &nesCPU::ABS, 4},
        {"CMP", &nesCPU::CMP, &nesCPU::ABS, 4},
        {"DEC", &nesCPU::DEC, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BNE", &nesCPU::BNE, &nesCPU::REL, 2},
        {"CMP", &nesCPU::CMP, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"CMP", &nesCPU::CMP, &nesCPU::ZPX, 4},
        {"DEC", &nesCPU::DEC, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"CLD", &nesCPU::CLD, &nesCPU::IMP, 2},
        {"CMP", &nesCPU::CMP, &nesCPU::ABY, 4},
        {"NOP", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"CMP", &nesCPU::CMP, &nesCPU::ABX, 4},
        {"DEC", &nesCPU::DEC, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"CPX", &nesCPU::CPX, &nesCPU::IMM, 2},
        {"SBC", &nesCPU::SBC, &nesCPU::IZX, 6},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"CPX", &nesCPU::CPX, &nesCPU::ZP0, 3},
        {"SBC", &nesCPU::SBC, &nesCPU::ZP0, 3},
        {"INC", &nesCPU::INC, &nesCPU::ZP0, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 5},
        {"INX", &nesCPU::INX, &nesCPU::IMP, 2},
        {"SBC", &nesCPU::SBC, &nesCPU::IMM, 2},
        {"NOP", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::SBC, &nesCPU::IMP, 2},
        {"CPX", &nesCPU::CPX, &nesCPU::ABS, 4},
        {"SBC", &nesCPU::SBC, &nesCPU::ABS, 4},
        {"INC", &nesCPU::INC, &nesCPU::ABS, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"BEQ", &nesCPU::BEQ, &nesCPU::REL, 2},
        {"SBC", &nesCPU::SBC, &nesCPU::IZY, 5},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 8},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"SBC", &nesCPU::SBC, &nesCPU::ZPX, 4},
        {"INC", &nesCPU::INC, &nesCPU::ZPX, 6},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 6},
        {"SED", &nesCPU::SED, &nesCPU::IMP, 2},
        {"SBC", &nesCPU::SBC, &nesCPU::ABY, 4},
        {"NOP", &nesCPU::NOP, &nesCPU::IMP, 2},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
        {"???", &nesCPU::NOP, &nesCPU::IMP, 4},
        {"SBC", &nesCPU::SBC, &nesCPU::ABX, 4},
        {"INC", &nesCPU::INC, &nesCPU::ABX, 7},
        {"???", &nesCPU::XXX, &nesCPU::IMP, 7},
    };
}

nesCPU::~nesCPU() {}

uint8_t nesCPU::read(uint16_t address) { return bus->read(address, false); }

void nesCPU::write(uint16_t address, uint8_t data) {
    bus->write(address, data);
}

uint8_t nesCPU::getFlag(flags f) { return ((status & f) > 0) ? 1 : 0; }

void nesCPU::setFlag(flags f, bool v) {
    if (v) {
        status |= f;
    } else {
        status &= ~f;
    }
}

void nesCPU::clock() {
    if (cycles == 0) {
        opcode = read(pc);
        pc++;

        cycles = lookup[opcode].cycles;

        uint8_t addCycleOne = (this->*lookup[opcode].addressingMode)();
        uint8_t addCycleTwo = (this->*lookup[opcode].operate)();

        cycles += (addCycleOne & addCycleTwo);
    }

    cycles--;
}

uint8_t nesCPU::IMP() {
    fetched = a;
    return 0;
}

uint8_t nesCPU::IMM() {
    addr_abs = pc++;
    return 0;
}

uint8_t nesCPU::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t nesCPU::ZPX() {
    addr_abs = read(pc) + x;
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t nesCPU::ZPY() {
    addr_abs = read(pc) + y;
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t nesCPU::REL() {
    addr_rel = read(pc);
    pc++;

    if (addr_rel & 0x80) {
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t nesCPU::ABS() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t nesCPU::ABX() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }

    return 0;
}

uint8_t nesCPU::ABY() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }

    return 0;
}

uint8_t nesCPU::IND() {
    uint8_t ptr_lo = read(pc);
    pc++;
    uint8_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF) {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    } else {
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

uint8_t nesCPU::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t nesCPU::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }

    return 0;
}

// Emulating the instructions

uint8_t nesCPU::fetch() {
    if (lookup[opcode].addressingMode != &nesCPU::IMP) {
        fetched = read(addr_abs);
    }

    return fetched;
}

uint8_t nesCPU::AND() {
    fetch();
    a &= fetched;

    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);

    return 1;
}

uint8_t nesCPU::BCS() {
    if (getFlag(C) == 1) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BCC() {
    if (getFlag(C) == 0) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BEQ() {
    if (getFlag(Z) == 1) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BMI() {
    if (getFlag(N) == 1) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BNE() {
    if (getFlag(Z) == 0) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BPL() {
    if (getFlag(N) == 0) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BVC() {
    if (getFlag(V) == 0) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t nesCPU::BVS() {
    if (getFlag(V) == 1) {
        cycles++;
        addr_abs = addr_abs + pc;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}
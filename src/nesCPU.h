#pragma once

#include <map>
#include <string>
#include <vector>

class Bus;

class nesCPU {
public:
    nesCPU();
    ~nesCPU();

    std::map<uint16_t, std::string> disassemble(uint16_t nStart,
        uint16_t nStop);

    void connectBus(Bus* n) { bus = n; }

    // addressing modes
    uint8_t IMP();  // implied
    uint8_t IMM();  // immediate
    uint8_t ZP0();  // zeropage
    uint8_t ZPX();  // zeropage x indexed
    uint8_t ZPY();  // zeropage y indexed
    uint8_t REL();  // relative
    uint8_t ABS();  // absolute
    uint8_t ABX();  // absolute x indexed
    uint8_t ABY();  // absolute y indexed
    uint8_t IND();  // indirect
    uint8_t IZX();  // x indexed indirect
    uint8_t IZY();  // indirect y indexed

    // opcodes
    uint8_t ADC();  // add with carry
    uint8_t AND();  // and
    uint8_t ASL();  // arithmetic shift left
    uint8_t BCC();  // branch if carry clear
    uint8_t BCS();  // branch if carry set
    uint8_t BEQ();  // branch if equal
    uint8_t BIT();  // bit test
    uint8_t BMI();  // branch if minus
    uint8_t BNE();  // branch if not equal
    uint8_t BPL();  // branch if positive
    uint8_t BRK();  // break
    uint8_t BVC();  // branch if overflow clear
    uint8_t BVS();  // branch if overflow set
    uint8_t CLC();  // clear carry flag
    uint8_t CLD();  // clear decimal flag
    uint8_t CLI();  // clear interrupt flag
    uint8_t CLV();  // clear overflow flag
    uint8_t CMP();  // compare
    uint8_t CPX();
    uint8_t CPY();
    uint8_t DEC();
    uint8_t DEX();
    uint8_t DEY();
    uint8_t EOR();
    uint8_t INC();
    uint8_t INX();
    uint8_t INY();
    uint8_t JMP();
    uint8_t JSR();
    uint8_t LDA();
    uint8_t LDX();
    uint8_t LDY();
    uint8_t LSR();
    uint8_t NOP();
    uint8_t ORA();
    uint8_t PHA();
    uint8_t PHP();
    uint8_t PLA();
    uint8_t PLP();
    uint8_t ROL();
    uint8_t ROR();
    uint8_t RTI();
    uint8_t RTS();
    uint8_t SBC();
    uint8_t SEC();
    uint8_t SED();
    uint8_t SEI();
    uint8_t STA();
    uint8_t STX();
    uint8_t STY();
    uint8_t TAX();
    uint8_t TAY();
    uint8_t TSX();
    uint8_t TXA();
    uint8_t TXS();
    uint8_t TYA();
    uint8_t XXX();  // illegal opcodes

    void clock();
    void reset();
    void interruptRequest();  // interrupt can be disabled with interrupt flag
    void nonMaskedInterruptRequest();  // interrupt cannot be disabled

    bool complete();

    uint8_t fetch();
    uint8_t fetched = 0x00;

    uint16_t addr_abs = 0x0000;
    uint16_t addr_rel = 0x00;
    uint8_t opcode = 0x00;
    uint8_t cycles = 0;

    enum flags {       // status register flags
        C = (1 << 0),  // carry - Set if overflow in bit 7
        Z = (1 << 1),  // zero - Set if overflow in bit 7
        I = (1 << 2),  // disable interrupts
        D = (1 << 3),  // decimal mode
        B = (1 << 4),  // break
        U = (1 << 5),  // unused
        V = (1 << 6),  // overflow - Set if sign bit is incorrect
        N = (1 << 7),  // negative - Set if bit 7 set
    };

    uint8_t a = 0x00;       // accumulator
    uint8_t x = 0x00;       // x register
    uint8_t y = 0x00;       // y register
    uint8_t sp = 0x00;      // stack pointer
    uint16_t pc = 0x0000;   // program counter
    uint8_t status = 0x00;  // status register

private:
    Bus* bus = nullptr;
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address);

    uint8_t getFlag(flags f);
    void setFlag(flags f, bool v);

    struct INSTRUCTION {
        std::string name;                            // instruction name;
        uint8_t(nesCPU::* operate)(void) = nullptr;  // opcode function
        uint8_t(nesCPU::* addressingMode)(void) =
            nullptr;         // opcode address mode
        uint8_t cycles = 0;  // cycle count
    };

    std::vector<INSTRUCTION> lookup;
};
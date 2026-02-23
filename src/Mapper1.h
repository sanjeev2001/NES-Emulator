#pragma once
#include "Mapper.h"

class Mapper1 : public Mapper {
public:
    Mapper1(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper1();

    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
    void reset() override;

private:
    void writeRegister(uint16_t addr, uint8_t data);

    // MMC1 Control Register (Bank select 0)
    // Bit 7: Reset MMC1 (1 = reset)
    // Bit 6-2: Not used
    // Bit 1-0: Mirroring and PRG bank mode
    uint8_t control = 0x0C;

    // Bank select registers
    uint8_t chrBank0 = 0;  // CHR bank 0 (4KB or 8KB)
    uint8_t chrBank1 = 0;  // CHR bank 1 (4KB mode only)
    uint8_t prgBank = 0;   // PRG bank (switchable 16KB or 32KB)

    // Shift register for MMC1's serial write mechanism
    uint8_t shiftRegister = 0;
    uint8_t shiftCount = 0;

    // PRG bank mode: 0 = 32KB, 1 = fixed $8000, fixed $C000, 2 = fixed $8000, switchable $C000, 3 = switchable $8000, fixed $C000 (default)
    // CHR bank mode: 0 = 8KB, 1 = 4KB
    bool is4KBCHRMode() const { return (control & 0x10) != 0; }
    uint8_t getPRGMode() const { return (control >> 2) & 0x03; }
};

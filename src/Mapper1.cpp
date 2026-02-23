#include "Mapper1.h"

Mapper1::Mapper1(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {
    // Initialize control register: 
    // - Bit 7 = 0 (not reset)
    // - Bit 1-0 = 11 (vertical mirroring by default for Mapper1)
    // Note: Bits 1-0: 00 = one-screen lower, 01 = one-screen upper, 
    //                          10 = vertical, 11 = horizontal
    control = 0x0C; // Horizontal mirroring, 32KB PRG mode
    
    shiftRegister = 0;
    shiftCount = 0;
}

Mapper1::~Mapper1() {}

void Mapper1::writeRegister(uint16_t addr, uint8_t data) {
    // Check for reset (bit 7 set)
    if (data & 0x80) {
        // Reset the mapper
        shiftRegister = 0;
        shiftCount = 0;
        control |= 0x0C;  // Set PRG bank mode to 3 (switchable at $8000, fixed at $C000)
        return;
    }

    // Shift register write
    shiftRegister |= ((data & 0x01) << shiftCount);
    shiftCount++;

    // After 5 writes, update the register
    if (shiftCount == 5) {
        uint8_t registerNum = (control >> 2) & 0x03;
        
        switch (registerNum) {
        case 0: // Control Register
            control = shiftRegister | 0x0C;
            break;
        case 1: // CHR Bank 0
            chrBank0 = shiftRegister;
            break;
        case 2: // CHR Bank 1
            chrBank1 = shiftRegister;
            break;
        case 3: // PRG Bank
            prgBank = shiftRegister & 0x0F;  // Only 4 bits used for PRG bank
            break;
        }

        // Reset shift register after write
        shiftRegister = 0;
        shiftCount = 0;
    }
}

bool Mapper1::cpuMapRead(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // PRG RAM at $6000-$7FFF
        mapped_addr = addr - 0x6000;
        return true;
    }

    if (addr >= 0x8000 && addr <= 0xFFFF) {
        uint8_t prgMode = getPRGMode();
        
        if (prgMode == 3) {
            // Mode 3: Switchable 16KB bank at $8000-$BFFF, fixed to last bank at $C000-$FFFF
            if (addr >= 0x8000 && addr <= 0xBFFF) {
                // Switchable bank
                mapped_addr = (prgBank * 16384) + (addr - 0x8000);
                // Handle wrap-around for banks >= number of PRG banks
                mapped_addr = mapped_addr % (nPRGBanks * 16384);
            } else {
                // Fixed to last bank
                mapped_addr = ((nPRGBanks - 1) * 16384) + (addr - 0xC000);
            }
        } else if (prgMode == 2) {
            // Mode 2: Fixed to first bank at $8000-$BFFF, switchable at $C000-$FFFF
            if (addr >= 0x8000 && addr <= 0xBFFF) {
                // Fixed to first bank
                mapped_addr = addr - 0x8000;
            } else {
                // Switchable bank
                mapped_addr = (prgBank * 16384) + (addr - 0xC000);
                mapped_addr = mapped_addr % (nPRGBanks * 16384);
            }
        } else if (prgMode == 1) {
            // Mode 1: 32KB mode at $8000-$FFFF
            // 32KB bank switching, but Mapper1 typically uses 16KB banks
            mapped_addr = ((prgBank * 16384) + (addr - 0x8000)) % (nPRGBanks * 16384);
        } else {
            // Mode 0: Same as mode 3 in practice for most implementations
            if (addr >= 0x8000 && addr <= 0xBFFF) {
                mapped_addr = (prgBank * 16384) + (addr - 0x8000);
                mapped_addr = mapped_addr % (nPRGBanks * 16384);
            } else {
                mapped_addr = ((nPRGBanks - 1) * 16384) + (addr - 0xC000);
            }
        }
        return true;
    }
    return false;
}

bool Mapper1::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) {
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // PRG RAM at $6000-$7FFF
        mapped_addr = addr - 0x6000;
        return true;
    }

    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // Handle MMC1's shift register write mechanism
        // The data written is in the low bits of the address/data
        writeRegister(addr, data);
        return true;
    }
    return false;
}

bool Mapper1::ppuMapRead(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (is4KBCHRMode()) {
            // 4KB CHR bank mode
            if (addr >= 0x0000 && addr <= 0x0FFF) {
                // CHR Bank 0
                mapped_addr = (chrBank0 * 4096) + addr;
                mapped_addr = mapped_addr % (nCHRBanks * 8192);
            } else {
                // CHR Bank 1
                mapped_addr = (chrBank1 * 4096) + (addr - 0x1000);
                mapped_addr = mapped_addr % (nCHRBanks * 8192);
            }
        } else {
            // 8KB CHR bank mode
            mapped_addr = (chrBank0 * 8192) + addr;
            mapped_addr = mapped_addr % (nCHRBanks * 8192);
        }
        return true;
    }
    return false;
}

bool Mapper1::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (nCHRBanks == 0) {
            // CHR RAM is available (8KB)
            if (is4KBCHRMode()) {
                // 4KB CHR bank mode
                if (addr >= 0x0000 && addr <= 0x0FFF) {
                    mapped_addr = (chrBank0 * 4096) + addr;
                } else {
                    mapped_addr = (chrBank1 * 4096) + (addr - 0x1000);
                }
            } else {
                // 8KB CHR bank mode
                mapped_addr = (chrBank0 * 8192) + addr;
            }
            return true;
        }
    }
    return false;
}

void Mapper1::reset() {
    // Reset MMC1 to known state
    control = 0x0C;  // Horizontal mirroring, PRG mode 3
    chrBank0 = 0;
    chrBank1 = 0;
    prgBank = 0;
    shiftRegister = 0;
    shiftCount = 0;
}

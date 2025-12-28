#pragma once

#include <sys/timeb.h>
#include <time.h>

#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "Pixel.h"
#include "Sprite.h"

class Nes;
class sdlLib;

class nesPPU {
   private:
    std::shared_ptr<Cartridge> cartridge;
    /*uint8_t     tblName[2][1024];*/
    uint8_t tblPattern[2][4096];
    uint8_t tblPalette[32];

    Pixel palScreen[0x40];

    Sprite* sprScreen;
    Sprite* sprNameTable[2];
    Sprite* sprPatternTable[2];

    int16_t scanline = 0;
    int16_t cycle = 0;

    struct {};

    union {
        struct {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;

    union {
        struct {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union PPUCTRL {
        struct {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;  // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;

    union loopy_register {
        // Credit to Loopy for working this out :D
        struct {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };

        uint16_t reg = 0x0000;
    };

    loopy_register vram_addr;  // Active "pointer" address into nametable to
                               // extract background tile info
    loopy_register tram_addr;  // Temporary store of information to be
                               // "transferred" into "pointer" at various times

    uint8_t fine_x = 0x00;
    uint8_t addressLatch = 0x00;
    uint8_t ppuDataBuffer = 0x00;
    // uint16_t ppuAddress = 0x0000;

    // Background rendering
    uint8_t bg_next_tile_id = 0x00;
    uint8_t bg_next_tile_attrib = 0x00;
    uint8_t bg_next_tile_lsb = 0x00;
    uint8_t bg_next_tile_msb = 0x00;
    uint16_t bg_shifter_pattern_lo = 0x0000;
    uint16_t bg_shifter_pattern_hi = 0x0000;
    uint16_t bg_shifter_attrib_lo = 0x0000;
    uint16_t bg_shifter_attrib_hi = 0x0000;

    struct attributeEntry {
        uint8_t y;
        uint8_t id;
        uint8_t attribute;
        uint8_t x;
    } OAM[64];

    uint8_t oamAddress = 0x00;

    attributeEntry spriteScanline[8];
    uint8_t spriteCount;
    uint16_t sprite_shifter_pattern_lo[8];
    uint16_t sprite_shifter_pattern_hi[8];

    bool spriteZeroHit = false;
    bool spriteZeroRendered = false;

   public:
    nesPPU();
    ~nesPPU();

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    void connectCartridge(const std::shared_ptr<Cartridge>& cart);
    void clock();
    void reset();
    uint8_t tblName[2][1024];
    Sprite& GetScreen();
    Sprite& GetNameTable(uint8_t i);
    uint8_t getname(uint8_t i, uint8_t j);
    Sprite& GetPatternTable(uint8_t i, uint8_t palette);
    Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
    bool frame_complete = false;
    bool nmi = false;
    uint8_t* pOAM = (uint8_t*)OAM;
};

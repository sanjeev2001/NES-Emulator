#pragma once
#include <cstdint>
#include <vector>
#include "Pixel.h"

class Sprite {
public:
    int32_t width = 0;
    int32_t height = 0;
    std::vector<Pixel> pColData;

    Sprite() = default;
    Sprite(int32_t w, int32_t h) : width(w), height(h) {
        pColData.resize(width * height);
    }
    
    Pixel GetPixel(int32_t x, int32_t y) const {
        if (x >= 0 && x < width && y >= 0 && y < height)
            return pColData[y * width + x];
        return Pixel(0,0,0,0);
    }

    bool SetPixel(int32_t x, int32_t y, Pixel p) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pColData[y * width + x] = p;
            return true;
        }
        return false;
    }
    
    Pixel* GetData() { return pColData.data(); }
};

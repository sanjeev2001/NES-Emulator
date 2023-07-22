#pragma once

#include <cstdint>
class cpuMemory
{
public:
	cpuMemory();
	~cpuMemory();
	static constexpr uint16_t cpuSramSize = 0x800;
	uint8_t cpuSram[cpuSramSize] = { 0 };

	static constexpr uint16_t prgSramBase = 0x6000;
	static constexpr uint16_t prgSramSize = 0x2000;
	uint8_t prgSram[prgSramSize] = { 0 };

};

class VideoMemory {

};
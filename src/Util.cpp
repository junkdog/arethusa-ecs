#include "Util.h"

namespace ecs {
int rightmostBit(unsigned int value) {
	if (value == 0)
		return -1;

	// see https://graphics.stanford.edu/~seander/bithacks.html
	int bit = 32;
	value &= -signed(value);
	if (value) bit--;
	if (value & 0x0000FFFF) bit -= 16;
	if (value & 0x00FF00FF) bit -= 8;
	if (value & 0x0F0F0F0F) bit -= 4;
	if (value & 0x33333333) bit -= 2;
	if (value & 0x55555555) bit -= 1;

	return bit;
}

int leftmostBit(unsigned int value) {
	// see https://graphics.stanford.edu/~seander/bithacks.html
	static const int multiplyDeBruijnBitPosition[32] = {
		0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
		8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
	};

	value |= value >> 1; // first round down to one less than a power of 2
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;

	return multiplyDeBruijnBitPosition[(uint32_t)(value * 0x07C4ACDDU) >> 27];
}
}

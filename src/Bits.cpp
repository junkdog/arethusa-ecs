#include "Bits.h"

namespace ecs {

	Bits::WordProxy& Bits::operator[](size_t bitIndex) {
		proxy.bit = bitIndex % WORD_SIZE;
		auto index = bitIndex / WORD_SIZE;
		if (words.size() <= (index + 1))
			words.resize(index + 1);

		proxy.word = &words[index];
		return proxy;
	}

	unsigned long Bits::wordsInUse() {
		return words.size();
	}

	// PROXY functions
	Bits::WordProxy& Bits::WordProxy::operator=(bool rhs) {
		Word mask = 1ul << bit;
		if (rhs) {
			*word = *word | mask;
		} else {
			*word = *word & ~mask;
		}
		return *this;
	}

	Bits::WordProxy::operator bool() const {
		Word mask = 1ul << bit;
		return (*word & mask) != 0;
	}
}

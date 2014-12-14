#include <algorithm>
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

	bool Bits::operator==(const Bits& rhs) const {
		auto lhsWords = this->words.size();
		auto rhsWords = rhs.words.size();
		auto commonWords = std::min(lhsWords, rhsWords);

		if (lhsWords != rhsWords) {
			auto& vector = (lhsWords > rhsWords)
				? this->words
				: rhs.words;

			auto s = std::max(lhsWords, rhsWords);
			for (auto i = commonWords; s > i; i++) {
				if (vector[i] != 0) return false;
			}
		}

		for (auto i = 0u; commonWords > i; i++) {
			if (this->words[i] != rhs.words[i]) return false;
		}

		return true;
	}

	bool Bits::operator!=(const Bits& rhs) const {
		return !(*this == rhs);
	}

	Bits& Bits::operator|=(const Bits& rhs) {
		syncSize(rhs);

		// just in case *this has more words than rhs
		auto commonWords = std::min(words.size(), rhs.words.size());
		for (auto i = 0u; commonWords > i; i++)
			words[i] |= rhs.words[i];

		return *this;
	}

	Bits Bits::operator|(const Bits& rhs) {
		ecs::Bits bits = *this;
		bits |= rhs;
		return bits;
	}


	Bits& Bits::operator&=(const Bits& rhs) {
		auto commonWords = std::min(words.size(), rhs.words.size());
		for (auto i = 0u; commonWords > i; i++)
			words[i] &= rhs.words[i];
		return *this;
	}

	Bits Bits::operator&(const Bits& rhs) {
		ecs::Bits bits = *this;
		bits &= rhs;
		return bits;
	}

	void Bits::syncSize(const Bits& rhs) {
		if (words.size() < rhs.words.size())
			words.resize(rhs.words.size());
	}
}

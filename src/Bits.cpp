#include <algorithm>
#include <pwd.h>
#include "Bits.h"

namespace ecs {


	Bits::Bits(const size_t words, std::initializer_list<unsigned int> bits)
		:  words(std::vector<Word>(words, {})) {

		for (auto bit : bits) {
			(*this)[bit] = true;
		}
	}

	Bits::WordProxy& Bits::operator[](unsigned int bitIndex) {
		proxy.bit = bitIndex % WORD_SIZE;
		auto index = bitIndex / WORD_SIZE;
		if (words.size() <= (index + 1))
			words.resize(index + 1);

		proxy.word = &words[index];
		return proxy;
	}

	unsigned long Bits::wordsInUse() const {
		return words.size();
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
		if (words.size() < rhs.words.size())
			words.resize(rhs.words.size());

		for (auto i = 0u, s = (unsigned int) rhs.words.size(); s > i; i++)
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

		for (auto i = commonWords; words.size() > i; i++)
			words[i] = 0;

		return *this;
	}

	Bits Bits::operator&(const Bits& rhs) {
		ecs::Bits bits = *this;
		bits &= rhs;
		return bits;
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
		Word mask = 1u << bit;
		return (*word & mask) != 0;
	}
}

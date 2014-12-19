#include <cassert>
#include <algorithm>
#include "Bits.h"

namespace ecs {


	Bits::Bits(const size_t words, std::initializer_list<unsigned int> bits)
		:  words(std::vector<Word>(words, {})) {

		for (auto bit : bits) {
			(*this)[bit] = true;
		}
	}


	int Bits::nextSetBit(unsigned int fromIndex) {
		for (unsigned int i = fromIndex, s = words.size() * WORD_SIZE; s > i; i++)
			if ((*this)[i]) return i;

		return -1;
	}

	Bits::WordProxy& Bits::operator[](unsigned int bitIndex) {
		assert(&proxy != nullptr);

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

	unsigned int Bits::count() {
		auto index = nextSetBit(0);

		auto count = 0u;
		while (index != -1) {
			count++;
			index = nextSetBit(index + 1);
		}

		return count;
	}

	bool Bits::none() {
		for (auto word : words)
			if (word != 0) return false;

		return true;
	}

	bool Bits::any() {
		return !none();
	}

	void Bits::set(unsigned int index, bool value) {
		(*this)[index] = value;
	}

	void Bits::reset() {
//		for (auto& word : words) {
//			word = 0u;
//		}

//		for (auto i = 0u; words.size() > i; i++)
//			words[i] = 0;
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

#include <cassert>
#include "Bits.h"
#include "Util.h"

namespace ecs {

Bits::Bits(const size_t words, std::initializer_list<unsigned int> bits) noexcept
	: words(std::vector<Word>(words, {})) {

	for (auto bit : bits) {
		(*this)[bit] = true;
	}
}

Bits::Bits(Bits&& other) noexcept : Bits() {
	*this = std::move(other);
}

int Bits::nextSetBit(unsigned int fromIndex) const {
	unsigned int wordIndex = fromIndex / WORD_SIZE;
	if (wordIndex >= words.size())
		return -1;

	unsigned int word = words[wordIndex];
	unsigned int bitIndex = fromIndex % WORD_SIZE;
	word &= ~((1 << bitIndex) - 1); // remove trailing bits on the rhs

	if (word > 0)
		return rightmostBit(word) + (wordIndex * WORD_SIZE);
	else
		return nextSetBit((wordIndex + 1) * WORD_SIZE);
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

Bits& Bits::operator=(const Bits& rhs) {
	if (this != &rhs)
		words = rhs.words;

	return *this;
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
	auto index = nextSetBit();

	auto count = 0u;
	while (index != -1) {
		count++;
		index = nextSetBit(index + 1);
	}

	return count;
}

bool Bits::none() const {
	for (auto word : words)
		if (word != 0) return false;

	return true;
}

bool Bits::any() const {
	return !none();
}

void Bits::set(unsigned int index, bool value) {
	(*this)[index] = value;
}

void Bits::reset() {
	for (auto& word : words)
		word = 0u;
}


int Bits::highestBit() const {
	Word highestWordIndex = words.size();
	for (auto word = words.rbegin(); word != words.rend(); word++) {
		highestWordIndex--;
		if (*word > 0)
			break;
	}

	return leftmostBit(words[highestWordIndex]) + (highestWordIndex * WORD_SIZE);
}

int Bits::lowestBit() const {
	Word lowestWordIndex = 0;
	for (auto word = words.begin(); word != words.end(); word++) {
		if (*word > 0)
			break;

		lowestWordIndex++;
	}

	if (lowestWordIndex == words.size())
		return -1;

	return rightmostBit(words[lowestWordIndex]) + (lowestWordIndex * WORD_SIZE);
}

Bits::WordProxy& Bits::WordProxy::operator=(bool rhs) {
	Word mask = 1u << bit;
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

int BitIterator::operator*() const {
	return pos;
}

BitIterator& BitIterator::operator++() {
	pos = bits->nextSetBit((unsigned int)pos + 1);
	return *this;
}

BitIterator Bits::end() const {
	return BitIterator(this, -1);
}

BitIterator Bits::begin() const {
	return BitIterator(this, lowestBit());
}
}

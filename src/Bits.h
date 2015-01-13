#pragma once

#include <memory>
#include <vector>

namespace ecs {

class Bits;

class BitIterator {
  public:
	BitIterator(const Bits* bits, int pos) : pos( pos ), bits(bits) {}

	bool operator==(const BitIterator& other) const {
		return pos == other.pos;
	}

	bool operator!=(const BitIterator& other) const {
		return !(*this == other);
	}

	int operator*() const;
	BitIterator& operator++();

  private:
	int pos;
	const Bits* bits;
};

class Bits {

	using Word = u_int32_t;

	class WordProxy {
		friend class Bits;

	  public:
		WordProxy() {}
		~WordProxy() = default;
		WordProxy& operator=(bool rhs);
		operator bool() const;

	  private:
		size_t bit;
		Word* word = nullptr;
	};

  public:
	Bits() noexcept : Bits(1u, {}) {}
	Bits(std::initializer_list<unsigned int> bits) noexcept : Bits(1u, bits) {};
	Bits(const size_t words, std::initializer_list<unsigned int> bits) noexcept;
	Bits(const Bits& other) : words(other.words) {}
	Bits(Bits&& other) noexcept;

	~Bits() = default;
	WordProxy& operator[](unsigned int bitIndex);
	bool get(unsigned int bitIndex) const {
		auto bit = bitIndex % WORD_SIZE;
		auto index = bitIndex / WORD_SIZE;
		if (words.size() <= (index + 1))
			return false;

		return (words[index] & (1 << bit)) != 0;
	}
	Bits& operator=(const Bits& rhs);
	bool operator==(const Bits& rhs) const;
	bool operator!=(const Bits& rhs) const;
	Bits& operator|=(const Bits& rhs);
	Bits operator|(const Bits& rhs);
	Bits& operator&=(const Bits& rhs);
	Bits operator&(const Bits& rhs);
	unsigned long wordsInUse() const;
	int nextSetBit(unsigned int fromIndex = 0) const;
	unsigned int count();
	void reset();
	int highestBit() const;
	int lowestBit() const;

	bool none() const;
	bool any() const;
	void set(unsigned int index, bool value);

	BitIterator begin() const;
	BitIterator end() const;

  private:
	const static unsigned int WORD_SIZE = sizeof(Word) * 8;

	WordProxy proxy;
	std::vector<Word> words; // move to proxy
};



}

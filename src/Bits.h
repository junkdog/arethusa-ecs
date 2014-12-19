#pragma once

#include <memory>
#include <vector>

namespace ecs {

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
		Bits() : Bits(1u, {}) {}
		Bits(std::initializer_list<unsigned int> bits) : Bits(1u, bits) {};
		Bits(const size_t words, std::initializer_list<unsigned int> bits);
		Bits(const Bits& other) : words(other.words) {}
		~Bits() = default;
		WordProxy& operator[](unsigned int bitIndex);
		bool operator==(const Bits& rhs) const;
		bool operator!=(const Bits& rhs) const;
		Bits& operator|=(const Bits& rhs);
		Bits operator|(const Bits& rhs);
		Bits& operator&=(const Bits& rhs);
		Bits operator&(const Bits& rhs);
		unsigned long wordsInUse() const;
		int nextSetBit(unsigned int fromIndex = 0);

	private:
		const static unsigned int WORD_SIZE = sizeof(Word) * 8;

		WordProxy proxy;
		std::vector<Word> words; // move to proxy
	};
}

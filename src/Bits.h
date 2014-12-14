#pragma once

#include <memory>
#include <vector>

namespace ecs {

	class Bits {

		using Word = u_int64_t;

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
		Bits(const size_t words = 1) : words(std::vector<Word>(words, 0)) {}
		Bits(const Bits& other) : words(other.words) {}
		~Bits() = default;
		WordProxy& operator[](size_t bitIndex);
		bool operator==(const Bits& rhs) const;
		bool operator!=(const Bits& rhs) const;
		Bits& operator|=(const Bits& rhs);
		Bits operator|(const Bits& rhs);
		Bits& operator&=(const Bits& rhs);
		Bits operator&(const Bits& rhs);
		unsigned long wordsInUse();

	private:
		const static unsigned int WORD_SIZE = sizeof(Word) * 8;

		WordProxy proxy;
		std::vector<Word> words; // move to proxy

		void syncSize(const Bits& rhs);
	};
}

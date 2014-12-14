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
		~Bits() = default;
		WordProxy& operator[](size_t bitIndex);
		unsigned long wordsInUse();

	private:
		WordProxy proxy;
		std::vector<Word> words; // move to proxy
		const static unsigned int WORD_SIZE = sizeof(Word) * 8;
	};


}

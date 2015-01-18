#include <Bits.h>
#include "gtest/gtest.h"

TEST(Bits, SimpleSetUnset) {
    ecs::Bits bits;
    bits[2] = true;
    ASSERT_EQ(bits[0], false);
    ASSERT_EQ(bits[2], true);
    bits[2] = false;
    ASSERT_EQ(bits[2], false);
    ASSERT_EQ(bits[0], false);
}

TEST(Bits, SetUnset) {
    ecs::Bits bits;
    bits[64] = true;
    bits[96] = true;
    ASSERT_EQ(bits[63], false);
    ASSERT_EQ(bits[64], true);
    ASSERT_EQ(bits[96], true);
    ASSERT_EQ(bits[100],false);
    ASSERT_EQ(4u, bits.wordsInUse());
}

TEST(Bits, WordsInUse) {
    ecs::Bits bits;
    ASSERT_EQ(1u, bits.wordsInUse());

    ecs::Bits bits2;
    ASSERT_EQ(1u, bits2.wordsInUse());

    ecs::Bits bits3{1, 3};
    ASSERT_EQ(1u, bits3.wordsInUse());

    ecs::Bits bits4{2, 1, 3};
    ASSERT_EQ(1u, bits4.wordsInUse());
    ASSERT_FALSE(bits4[0]);
    ASSERT_TRUE(bits4[1]);
    ASSERT_TRUE(bits4[2]);
    ASSERT_FALSE(bits4[4]);
}

TEST(Bits, BitwiseOR) {
    ecs::Bits bits1;
    bits1[1] = true;
    bits1[2] = true;
    bits1[5] = true;
    bits1[7] = true;
    ecs::Bits bits2;
    bits2[2] = true;
    bits2[4] = true;
    bits2[6] = true;
    bits2[7] = true;

    ecs::Bits bits3 = bits1 | bits2;

    bits1 |= bits2;
    ASSERT_EQ(bits1[0], false);
    ASSERT_EQ(bits1[1], true);
    ASSERT_EQ(bits1[2], true);
    ASSERT_EQ(bits1[3], false);
    ASSERT_EQ(bits1[4], true);
    ASSERT_EQ(bits1[5], true);
    ASSERT_EQ(bits1[6], true);
    ASSERT_EQ(bits1[7], true);
    ASSERT_EQ(bits1[8], false);

    ASSERT_TRUE(bits1 == bits3);

    ASSERT_EQ(4, bits2.count());
    ASSERT_EQ(6, bits1.count());
}

TEST(Bits, IterateBits) {
    ecs::Bits bits {1, 14, 23, 43, 534, 256, 124};
    auto count = 0u;

    auto index = bits.nextSetBit();
    while (index != -1) {
        ASSERT_TRUE(bits[index]);
        count++;
        index = bits.nextSetBit(index + 1);
    }
    ASSERT_EQ(7u, count);
}

TEST(Bits, RangeForIterateBits) {
    ecs::Bits bits {0, 3, 14, 23, 43, 534, 256, 124};
    ecs::Bits bits2 {};
    auto count = 0;
    for (auto bit : bits) {
        bits2[bit] = true;
        count++;
    }

    ASSERT_EQ(8, count);
    ASSERT_EQ(8u, bits2.count());
    ASSERT_EQ(bits, bits2);
}

TEST(Bits, RangeForEmpty) {
    ecs::Bits bits {};
    auto count = 0;
    for (auto bit : bits) {
        count += 1 + bit;
    }

    ASSERT_EQ(0, count);
}

TEST(Bits, IterateBitsEmpty) {
    ecs::Bits bits;
    ASSERT_EQ(-1, bits.nextSetBit());
}

TEST(Bits, BitwiseAND) {
    ecs::Bits bits1;
    bits1[1] = true;
    bits1[2] = true;
    bits1[5] = true;
    bits1[7] = true;
    ecs::Bits bits2;
    bits2[2] = true;
    bits2[4] = true;
    bits2[6] = true;
    bits2[7] = true;

    ecs::Bits bits3 = bits1 & bits2;

    bits1 &= bits2;
    ASSERT_EQ(bits1[0], false);
    ASSERT_EQ(bits1[1], false);
    ASSERT_EQ(bits1[2], true);
    ASSERT_EQ(bits1[3], false);
    ASSERT_EQ(bits1[4], false);
    ASSERT_EQ(bits1[5], false);
    ASSERT_EQ(bits1[6], false);
    ASSERT_EQ(bits1[7], true);
    ASSERT_EQ(bits1[8], false);

    ASSERT_TRUE(bits1 == bits3);
}

TEST(Bits, Equality) {
    ecs::Bits bits1;
    bits1[1] = true;
    bits1[2] = true;
    bits1[5] = true;
    bits1[7] = true;
    ecs::Bits bits2;
    bits2[1] = true;
    bits2[2] = true;
    bits2[5] = true;
    bits2[7] = true;

    ASSERT_TRUE(bits1 == bits2);
}

TEST(Bits, HighestSetBit) {
    ecs::Bits bits1a {2, 4, 14, 24};
    ecs::Bits bits1b {31};
    ecs::Bits bits2 {1, 24, 32, 63};

    ASSERT_EQ(24, bits1a.highestBit());
    ASSERT_EQ(31, bits1b.highestBit());
    ASSERT_EQ(63, bits2.highestBit());
}

TEST(Bits, LowestSetBit) {
    ecs::Bits bits1a {2, 4, 14, 24};
    ecs::Bits bits1b {31};
    ecs::Bits bits2 {1, 24, 32, 63};

    ASSERT_EQ(2, bits1a.lowestBit());
    ASSERT_EQ(31, bits1b.lowestBit());
    ASSERT_EQ(1, bits2.lowestBit());
}

TEST(Bits, EqualityDifferentWordLength) {
    ecs::Bits bits1;
    bits1[1] = true;
    ecs::Bits bits2;
    bits2[1] = true;
    bits2[420] = true;
    bits2[420] = false;

    ASSERT_TRUE(bits1 == bits2);
    bits2[256] = true;
    ASSERT_FALSE(bits1 == bits2);

    ecs::Bits bits3;
    bits3[1] = true;
    bits3[420] = true;
    bits3[420] = false;
    ecs::Bits bits4;
    bits4[1] = true;

    ASSERT_TRUE(bits3 == bits4);
    bits3[256] = true;
    ASSERT_FALSE(bits3 == bits4);
    ASSERT_TRUE(bits3 != bits4);
}

TEST(Bits, FillBitByBit) {
    ecs::Bits bs;
    for (int i = 0; 4096 > i; i++) {
        bs[i] = true;
    }

    for (int i = 0; 4096 > i; i++) {
        ASSERT_TRUE(bs[i]);
    }

    for (int i = 0; 4096 > i; i++) {
        ASSERT_FALSE(bs[i + 4096]);
    }
}


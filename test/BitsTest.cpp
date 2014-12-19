#include <backtrace.h>
#include <src/Bits.h>
#include "gtest/gtest.h"
#include "arethusa.h"

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

    ecs::Bits bits2(4, {});
    ASSERT_EQ(4u, bits2.wordsInUse());

    ecs::Bits bits3(2, {1, 3});
    ASSERT_EQ(2u, bits3.wordsInUse());

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


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
    ASSERT_EQ(2u, bits.wordsInUse());
}

TEST(Bits, WordsInUse) {
    ecs::Bits bits;
    ASSERT_EQ(1u, bits.wordsInUse());

    ecs::Bits bits2 {4};
    ASSERT_EQ(4u, bits2.wordsInUse());
}



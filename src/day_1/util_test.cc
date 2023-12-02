#include "src/day_1/util.h"

#include "gtest/gtest.h"

namespace day_1 {
namespace {


TEST(DayOneTest, TypicalUseCase){
  EXPECT_EQ(*GetEncodedNumber("12"), 12);
  EXPECT_EQ(*GetEncodedNumber("1a2"), 12);
  EXPECT_EQ(*GetEncodedNumber("a1a2"), 12);
  EXPECT_EQ(*GetEncodedNumber("a1a2a"), 12);
}

TEST(DayOneTest, NoNumberIsError){
  EXPECT_EQ(GetEncodedNumber("abc").status().message(),
            "Failed to find a digit.");
}

TEST(DayOneTest, OnlyOneNumber){
  EXPECT_EQ(*GetEncodedNumber("a3bc"), 33);
}

TEST(DayOneTest, ContainsSpaces){
  EXPECT_EQ(*GetEncodedNumber("       1               2  "), 12);
}

} // namespace
} // namespace day_1

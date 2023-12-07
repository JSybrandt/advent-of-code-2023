#include "src/day_1/util.h"

#include "gtest/gtest.h"

namespace day_1 {
namespace {

TEST(DayOneTests, FindSubstringOffsetWorksForward) {
  EXPECT_EQ(FindSubstringOffset("ABCDE", "A", SearchDirection::FORWARD), 0);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "B", SearchDirection::FORWARD), 1);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "CD", SearchDirection::FORWARD), 2);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "CDE", SearchDirection::FORWARD), 2);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "AD", SearchDirection::FORWARD),
            std::nullopt);
}

TEST(DayOneTests, FindSubstringOffsetWorksBackward) {
  EXPECT_EQ(FindSubstringOffset("ABCDE", "A", SearchDirection::BACKWARD), 4);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "B", SearchDirection::BACKWARD), 3);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "CD", SearchDirection::BACKWARD), 1);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "CDE", SearchDirection::BACKWARD), 0);
  EXPECT_EQ(FindSubstringOffset("ABCDE", "AD", SearchDirection::BACKWARD),
            std::nullopt);
}

TEST(DayOneTests, FindFirstWorksForward) {
  EXPECT_EQ(
      FindFirst("X1X2X", GetDigitNumberStrings(), SearchDirection::FORWARD), 1);

  EXPECT_EQ(FindFirst("X1XtwoXthreeX4x", GetEnglishNumberStrings(),
                      SearchDirection::FORWARD),
            2);
}

TEST(DayOneTests, FindFirstWorksBackward) {
  EXPECT_EQ(
      FindFirst("X1X2X", GetDigitNumberStrings(), SearchDirection::BACKWARD),
      2);
  EXPECT_EQ(FindFirst("X1XtwoXthreeX4x", GetEnglishNumberStrings(),
                      SearchDirection::BACKWARD),
            3);
}

} // namespace
} // namespace day_1

#include "src/day_9/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace day_9 {
namespace {

using ::testing::ElementsAre;
using ::testing::FieldsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(DayNineTest, ParseSequenceWorks) {
  EXPECT_THAT(ParseSequence("1 -1 2 -3"), ElementsAre(1, -1, 2, -3));
}

TEST(DayNineTest, GetDeltaSequenceWorks) {
  EXPECT_THAT(GetDeltaSequence({1, 2, 4, 2, -1}), ElementsAre(1, 2, -2, -3));
}

TEST(DayNineTest, IsAllZerosWorks) {
  EXPECT_TRUE(IsAllZeros({0, 0}));
  EXPECT_FALSE(IsAllZeros({0, 1}));
  // I think we just have to live with this case.
  EXPECT_TRUE(IsAllZeros({}));
}

TEST(DayNineTest, ExtrapolateNextWorks) {
  EXPECT_EQ(ExtrapolateNext({0, 3, 6, 9, 12, 15}), 18);
  EXPECT_EQ(ExtrapolateNext({1, 3, 6, 10, 15, 21}), 28);
  EXPECT_EQ(ExtrapolateNext({10, 13, 16, 21, 30, 45}), 68);
}

TEST(DayNineTest, ExtrapolatePrevWorks) {
  EXPECT_EQ(ExtrapolatePrev({0, 3, 6, 9, 12, 15}), -3);
  EXPECT_EQ(ExtrapolatePrev({1, 3, 6, 10, 15, 21}), 0);
  EXPECT_EQ(ExtrapolatePrev({10, 13, 16, 21, 30, 45}), 5);
}

} // namespace
} // namespace day_9

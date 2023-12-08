#include "src/day_4/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <vector>

namespace day_4 {
namespace {

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

TEST(DayFourTest, PareCardWorks) {
  const auto card =
      ParseCard("Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53");
  ASSERT_EQ(card.status(), absl::OkStatus());
  EXPECT_THAT(card->winning_numbers, UnorderedElementsAre(41, 48, 83, 86, 17));
  EXPECT_THAT(card->your_numbers,
              UnorderedElementsAre(83, 86, 6, 31, 17, 9, 48, 53));
}

TEST(DayFourTest, OverlapSizeWorks) {
  EXPECT_THAT(OverlapSize({1, 2, 3, 4}, {3, 4, 5, 6}), 2);
  EXPECT_THAT(OverlapSize({1, 2, 3, 4}, {}), 0);
  EXPECT_THAT(OverlapSize({1, 2, 3}, {4, 3, 2, 1}), 3);
}

} // namespace
} // namespace day_4

#include "src/day_8/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

namespace day_8 {
namespace {

using ::testing::ElementsAre;
using ::testing::FieldsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(DayEightTest, ParseMapWorks) {
  Map actual = *ParseMap({"AAA = (BBB, CCC)", "BBB = (CCC, DDD)"});
  EXPECT_THAT(actual,
              UnorderedElementsAre(Pair("AAA", FieldsAre("BBB", "CCC")),
                                   Pair("BBB", FieldsAre("CCC", "DDD"))));
}

TEST(DayEightTest, ParseInstructions) {
  Instructions instructions = ParseInstructions("RLLRL");
  EXPECT_THAT(instructions.directions,
              ElementsAre(Direction::RIGHT, Direction::LEFT, Direction::LEFT,
                          Direction::RIGHT, Direction::LEFT));
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(instructions.next(), Direction::RIGHT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
    EXPECT_EQ(instructions.next(), Direction::RIGHT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
  }
}

} // namespace
} // namespace day_8

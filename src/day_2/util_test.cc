#include "src/day_2/util.h"

#include "gtest/gtest.h"

namespace day_2 {
namespace {

TEST(DayTwoTest, ParseGameWorks) {
  absl::StatusOr<Game> game =
      ParseGame("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n");
  ASSERT_EQ(game.status(), absl::OkStatus());
}

TEST(DayTwoTest, IsHandfulPossibleWorks) {
  EXPECT_TRUE(IsHandfulPossible({{"red", 1}, {"yellow", 2}, {"blue", 3}},
                                {{"red", 1}, {"yellow", 2}, {"blue", 3}}));
  EXPECT_TRUE(IsHandfulPossible({{"red", 1}, {"yellow", 0}, {"blue", 1}},
                                {{"red", 1}, {"blue", 3}}));
  EXPECT_FALSE(IsHandfulPossible({{"red", 1}, {"yellow", 1}, {"blue", 1}},
                                 {{"red", 1}, {"blue", 3}}));
  EXPECT_FALSE(
      IsHandfulPossible({{"red", 2}, {"blue", 1}}, {{"red", 1}, {"blue", 3}}));
}

TEST(DayTwoTest, ComputePowerWorks) {
  absl::StatusOr<Game> game =
      ParseGame("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n");
  ASSERT_EQ(game.status(), absl::OkStatus());
  EXPECT_EQ(ComputePower(*game), 48);
}

} // namespace
} // namespace day_2

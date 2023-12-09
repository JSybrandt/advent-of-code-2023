#include "src/day_6/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <vector>

namespace day_6 {
namespace {

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

TEST(DaySixTest, ParseRacesWorks) {
  const std::string times = "Time:      7  15   30";
  const std::string dists = "Distance:  9  40  200";
  EXPECT_THAT(*ParseRaces(times, dists),
              ElementsAre(Race{7, 9}, Race{15, 40}, Race{30, 200}));
}

TEST(DaySixTest, ComputeWinningRaceRangeWorks) {
  const WinningRaceRange actual =
      *ComputeWinningRaceRange(Race{.time_limit = 7, .distance_record = 9});
  EXPECT_EQ(actual.first_winning_ms, 2);
  EXPECT_EQ(actual.last_winning_ms, 5);
}

TEST(DaySixTest, ComputeWinningRaceOneTieDoesntCount) {
  // Here if we hold for exactly 3 seconds, we would tie. That's not good
  // enough, we have to win.
  EXPECT_EQ(
      ComputeWinningRaceRange(Race{.time_limit = 6, .distance_record = 9}),
      std::nullopt);
}

TEST(DaySixTest, ComputeWinningRaceZeroSolutionWorks) {
  EXPECT_EQ(
      ComputeWinningRaceRange(Race{.time_limit = 5, .distance_record = 9}),
      std::nullopt);
}

TEST(DaySixTest, ComputeWinningRaceRangeAccountsForTies) {
  const WinningRaceRange actual =
      *ComputeWinningRaceRange(Race{.time_limit = 30, .distance_record = 200});
  // Note that this race could be tied by holding the button for 10 or 20
  // seconds, so we need to hold it for 11 or 19 seconds to beat it.
  // ComputeWinningRaceRange should be able to account for that.
  EXPECT_EQ(actual.first_winning_ms, 11);
  EXPECT_EQ(actual.last_winning_ms, 19);
}

TEST(DaySixTest, ParseBigRaceworks) {
  const std::string times = "Time:      7  15   30";
  const std::string dists = "Distance:  9  40  200";
  EXPECT_THAT(*ParseOneBigRace(times, dists), (Race{71530, 940200}));
}

} // namespace
} // namespace day_6

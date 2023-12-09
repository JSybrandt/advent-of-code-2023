#include "src/day_6/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <sstream>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_6 {
absl::StatusOr<std::vector<Race>>
ParseRaces(absl::string_view serialized_times,
           absl::string_view serialized_distances) {
  std::stringstream time_ss, dist_ss;
  time_ss << serialized_times;
  dist_ss << serialized_distances;

  std::string next_time, next_dist;
  // Skip the first text.
  time_ss >> next_time;
  dist_ss >> next_dist;

  std::vector<Race> races;
  while (time_ss >> next_time && dist_ss >> next_dist) {
    auto &race = races.emplace_back();
    if (!absl::SimpleAtoi(next_time, &race.time_limit)) {
      return absl::InvalidArgumentError("Invalid time: " + next_time);
    }
    if (!absl::SimpleAtoi(next_dist, &race.distance_record)) {
      return absl::InvalidArgumentError("Invalid distance: " + next_dist);
    }
  }
  return races;
}

std::optional<WinningRaceRange> ComputeWinningRaceRange(const Race &race) {
  // I actually did math!
  // Your button-hold to distance function is a parabola, so we just check when
  // the parabola touches the x axis.
  // Let x be the amount of time you hold the button.
  // Let r be the record distance.
  const double r = race.distance_record;
  // Let t be the race time limit.
  const double t = race.time_limit;
  // Then you can describe the amount you won the race by: x(t-x) - r
  // Using the quadratic equation, we can find the points where the race is a
  // tie.
  // x_1 = (t - sqrt(t^2-4r))/2
  // x_2 = (t + sqrt(t^2-4r))/2
  // Note that if t^2-4r is negative, then its impossible to win the race.
  // Lets name that expression "winning_coef."
  double winning_coef = t * t - 4 * r;
  if (winning_coef < 0) {
    // Its impossible to win this race.
    return std::nullopt;
  }
  double x_1 = (t - sqrt(winning_coef)) / 2;
  double x_2 = (t + sqrt(winning_coef)) / 2;
  // We start winning the race when x_1 < x < x_2. So lets nudge our boundary
  // points a little closer together.
  constexpr double kNudgeAmt = 1e-5;
  x_1 += kNudgeAmt;
  x_2 -= kNudgeAmt;
  if (x_1 >= x_2) {
    return std::nullopt;
  }
  // Now we need to round x_1 up, and x_2 down. This find the earliest and
  // latest whole ms integer where we start winning.
  return WinningRaceRange{
      .first_winning_ms = static_cast<uint64_t>(std::ceil(x_1)),
      .last_winning_ms = static_cast<uint64_t>(std::floor(x_2)),
  };
}

absl::StatusOr<Race> ParseOneBigRace(absl::string_view serialized_times,
                                     absl::string_view serialized_distances) {
  std::string big_time =
      absl::StrReplaceAll(serialized_times, {{" ", ""}, {"Time:", ""}});
  std::string big_dist =
      absl::StrReplaceAll(serialized_distances, {{" ", ""}, {"Distance:", ""}});
  Race race;
  if (!absl::SimpleAtoi(big_time, &race.time_limit)) {
    return absl::InvalidArgumentError("Invalid time: '" + big_time + "'");
  }
  if (!absl::SimpleAtoi(big_dist, &race.distance_record)) {
    return absl::InvalidArgumentError("Invalid distance: " + big_dist);
  }
  return race;
}
} // namespace day_6

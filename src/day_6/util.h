#ifndef SRC_DAY_6_UTIL_H_
#define SRC_DAY_6_UTIL_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_6 {

struct Race {
  uint64_t time_limit;
  uint64_t distance_record;

  bool operator==(const Race &other) const {
    return this->time_limit == other.time_limit &&
           this->distance_record == other.distance_record;
  }
  friend std::ostream &operator<<(std::ostream &o, const Race &race) {
    o << "{ time: " << race.time_limit << " distance: " << race.distance_record
      << " }";
    return o;
  }
};

absl::StatusOr<std::vector<Race>>
ParseRaces(absl::string_view serialized_times,
           absl::string_view serialized_distances);

absl::StatusOr<Race> ParseOneBigRace(absl::string_view serialized_times,
                                     absl::string_view serialized_distances);

struct WinningRaceRange {
  uint64_t first_winning_ms;
  uint64_t last_winning_ms;
};
std::optional<WinningRaceRange> ComputeWinningRaceRange(const Race &race);

} // namespace day_6

#endif // SRC_DAY_6_UTIL_H_

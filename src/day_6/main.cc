#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/ascii.h"
#include "absl/strings/string_view.h"

#include "src/day_6/util.h"

using ::day_6::ComputeWinningRaceRange;
using ::day_6::ParseOneBigRace;
using ::day_6::ParseRaces;
using ::day_6::Race;
using ::day_6::WinningRaceRange;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::string serialized_times, serialized_distances;
  if (!std::getline(std::cin, serialized_times)) {
    std::cerr << "Failed to get serialized_times.";
    return 1;
  }
  if (!std::getline(std::cin, serialized_distances)) {
    std::cerr << "Failed to get serialized_distances.";
    return 1;
  }

  { // Part 1.
    std::vector<Race> races =
        *ParseRaces(serialized_times, serialized_distances);
    uint64_t total = 1;
    for (const auto &race : races) {
      std::optional<WinningRaceRange> winning_range =
          ComputeWinningRaceRange(race);
      uint64_t num_solutions = winning_range.has_value()
                                   ? winning_range->last_winning_ms -
                                         winning_range->first_winning_ms + 1
                                   : 0;
      total *= num_solutions;
    }
    std::cout << "Part 1:" << total << std::endl;
  }

  { // Part 2.
    Race race = *ParseOneBigRace(serialized_times, serialized_distances);
    std::optional<WinningRaceRange> winning_range =
        ComputeWinningRaceRange(race);
    uint64_t num_solutions = winning_range.has_value()
                                 ? winning_range->last_winning_ms -
                                       winning_range->first_winning_ms + 1
                                 : 0;
    std::cout << "Part 2:" << num_solutions << std::endl;
  }

  return 0;
}

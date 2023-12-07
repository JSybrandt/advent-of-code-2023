#ifndef SRC_DAY_2_UTIL_H_
#define SRC_DAY_2_UTIL_H_

#include <optional>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_2 {

// Map of color to count.
using CubeCounts = absl::flat_hash_map<std::string, size_t>;

struct Game {
  size_t id;
  // Handfulls of cube counts.
  std::vector<CubeCounts> handfulls;
};

absl::StatusOr<Game> ParseGame(absl::string_view serialized_game);

bool IsGamePossible(const Game &game, const CubeCounts &bag_contents);

bool IsHandfulPossible(const CubeCounts &handful,
                       const CubeCounts &bag_contents);

size_t ComputePower(const Game &game);

} // namespace day_2

#endif // SRC_DAY_2_UTIL_H_

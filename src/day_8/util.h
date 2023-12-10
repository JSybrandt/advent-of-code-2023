#ifndef SRC_DAY_8_UTIL_H_
#define SRC_DAY_8_UTIL_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_8 {

struct LeftAndRight {
  std::string left, right;

  friend std::ostream &operator<<(std::ostream &o, const LeftAndRight &lr) {
    o << "{\"" << lr.left << "\", \"" << lr.right << "\"}";
    return o;
  }
};
using Map = absl::flat_hash_map<std::string, LeftAndRight>;

absl::StatusOr<Map> ParseMap(std::vector<std::string> edges);

enum class Direction { LEFT, RIGHT };

struct Instructions {
  std::vector<Direction> directions;
  size_t current_idx = 0;
  Direction next() {
    const auto result = directions[current_idx];
    current_idx = (current_idx + 1) % directions.size();
    return result;
  }
};

Instructions ParseInstructions(absl::string_view serialized_directions);

std::string Traverse(const Map &map, absl::string_view current_position,
                     Direction direction);

} // namespace day_8

#endif // SRC_DAY_8_UTIL_H_

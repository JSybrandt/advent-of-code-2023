#ifndef SRC_DAY_8_UTIL_H_
#define SRC_DAY_8_UTIL_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "Eigen/Sparse"
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

struct MapIndex {
  std::vector<std::string> name_by_idx;
  absl::flat_hash_map<std::string, size_t> idx_by_name;
  absl::flat_hash_set<size_t> sources;
  absl::flat_hash_set<size_t> targets;
};
MapIndex IndexMap(const Map &map);

struct TransitionMatrices {
  // A square matrix. If row i column j is true, then you can transition from
  // node i to j.
  Eigen::SparseMatrix<bool> left, right;
};
TransitionMatrices ToTransitionMatrices(const Map &map, const MapIndex &index);

// Row vectors that describe each source.
Eigen::SparseMatrix<bool> GetInitialPositionMatrix(const MapIndex &index);

// Column vector that describes that targets.
Eigen::SparseVector<bool> GetTargetPositionVector(const MapIndex &index);

bool CheckComplete(const Eigen::SparseMatrix<bool> &current_positions,
                   const Eigen::SparseVector<bool> &targets);

} // namespace day_8

#endif // SRC_DAY_8_UTIL_H_

#ifndef SRC_DAY_10_UTIL_H_
#define SRC_DAY_10_UTIL_H_

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

namespace day_10 {

struct Pos {
  int64_t row;
  int64_t col;
};

// Add positions together.
inline Pos operator+(const Pos &a, const Pos &b) {
  return {a.row + b.row, a.col + b.col};
}

inline bool operator==(const Pos &a, const Pos &b) {
  return a.row == b.row && a.col == b.col;
}

// Hash positions, for use in absl::flat_hash_map.
template <typename H> H AbslHashValue(H h, const Pos &p) {
  return H::combine(std::move(h), p.row, p.col);
}

inline std::ostream &operator<<(std::ostream &o, const Pos &p) {
  return o << "(" << p.row << ", " << p.col << ")";
}

struct PipeNetwork {
  // Each element of data is a row of characters.
  PipeNetwork(const std::vector<std::string> &data);
  // network[pos] returns the set of positions that you can go from pos. To
  // traverse from i -> j both pipes need an opening to eachother.
  absl::flat_hash_map<Pos, absl::flat_hash_set<Pos>> network;
  // grid[row][col] returns the space.
  std::vector<std::string> grid;
  Pos start;

  // Beginning at `start` walk the pipe network. Each traversable position is
  // visited once.
  void BreadthFirstTraversal(const std::function<void(Pos)> &visit_fn) const;
};

inline std::ostream &operator<<(std::ostream &o,
                                const PipeNetwork &pipe_network) {
  for (const auto &[pos, neighbors] : pipe_network.network) {
    o << pos << "\n";
    for (const auto &neigh : neighbors) {
      o << " -> " << neigh << "\n";
    }
  }
  return o;
}

} // namespace day_10

#endif // SRC_DAY_10_UTIL_H_

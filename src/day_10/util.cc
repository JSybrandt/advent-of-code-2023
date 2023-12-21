#include "src/day_10/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_10 {
namespace {

constexpr Pos kNorth = {-1, 0};
constexpr Pos kSouth = {1, 0};
constexpr Pos kEast = {0, 1};
constexpr Pos kWest = {0, -1};

// A pipe with each shape can open to each of the following directions.
const auto *kPipeOpenings = new absl::flat_hash_map<char, std::vector<Pos>>({
    // | is a vertical pipe connecting north and south.
    {'|', {kNorth, kSouth}},
    // - is a horizontal pipe connecting east and west.
    {'-', {kWest, kEast}},
    // L is a 90-degree bend connecting north and east.
    {'L', {kNorth, kEast}},
    // J is a 90-degree bend connecting north and west.
    {'J', {kNorth, kWest}},
    // 7 is a 90-degree bend connecting south and west.
    {'7', {kSouth, kWest}},
    // F is a 90-degree bend connecting south and east.
    {'F', {kSouth, kEast}},
    // . is ground; there is no pipe in this tile.
    {'.', {}},
    // S is the starting position of the animal; there is a pipe on this tile,
    // but your sketch doesn't show what shape the pipe has.
    {'S', {kNorth, kSouth, kEast, kWest}},
});

absl::flat_hash_set<Pos> GetPipeOpenings(const char pipe, const Pos &pos) {
  CHECK(kPipeOpenings->contains(pipe));
  absl::flat_hash_set<Pos> result;
  for (const Pos &offset : kPipeOpenings->at(pipe)) {
    result.insert(offset + pos);
  }
  return result;
}

} // namespace

PipeNetwork::PipeNetwork(const std::vector<std::string> &data) : grid(data) {
  // pipe_openings[pos] returns the set of positions the pipe at pos can open
  // to. In order to actually connect, that pipe needs an opening to pos as
  // well.
  absl::flat_hash_map<Pos, absl::flat_hash_set<Pos>> pipe_openings;
  for (size_t row = 0; row < grid.size(); ++row) {
    for (size_t col = 0; col < grid[row].size(); ++col) {
      const Pos pos{static_cast<int64_t>(row), static_cast<int64_t>(col)};
      const char pipe = grid[row][col];
      pipe_openings[pos] = GetPipeOpenings(pipe, pos);
      if (pipe == 'S') {
        start = pos;
      }
    }
  }

  // Now we want to check that two pipes open to eachother.
  for (const auto &[current, neighbors] : pipe_openings) {
    for (const auto &neighbor : neighbors) {
      if (!pipe_openings.contains(neighbor)) {
        // The pipe opens outside of the network.
        continue;
      }
      if (pipe_openings.at(neighbor).contains(current)) {
        network[current].insert(neighbor);
        network[neighbor].insert(current);
      }
    }
  }
};

void PipeNetwork::BreadthFirstTraversal(
    const std::function<void(Pos)> &visit_fn) const {
  std::queue<Pos> queue;
  queue.push(start);
  absl::flat_hash_set<Pos> seen{start};

  while (!queue.empty()) {
    const Pos curr = queue.front();
    queue.pop();
    visit_fn(curr);
    for (const Pos neigh : network.at(curr)) {
      if (!seen.contains(neigh)) {
        seen.insert(neigh);
        queue.push(neigh);
      }
    }
  }
}

} // namespace day_10

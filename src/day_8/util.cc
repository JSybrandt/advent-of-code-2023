#include "src/day_8/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <sstream>
#include <unistd.h>
#include <utility>
#include <vector>

#include "Eigen/Sparse"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_8 {

absl::StatusOr<Map> ParseMap(std::vector<std::string> edges) {
  Map result;
  for (const auto &edge : edges) {
    std::vector<std::string> tokens = absl::StrSplit(
        absl::StrReplaceAll(absl::StripAsciiWhitespace(edge),
                            {{" = ", " "}, {"(", ""}, {")", ""}, {",", ""}}),
        ' ');
    if (tokens.size() != 3) {
      return absl::InvalidArgumentError("Invalid edge: " + edge);
    }
    result[tokens[0]] = {.left = tokens[1], .right = tokens[2]};
  }
  return result;
}

Instructions ParseInstructions(absl::string_view serialized_directions) {
  Instructions result;
  for (const char d : serialized_directions) {
    switch (d) {
    case 'R':
      result.directions.push_back(Direction::RIGHT);
      break;
    case 'L':
      result.directions.push_back(Direction::LEFT);
      break;
    default:
      CHECK(false);
      break;
    }
  }
  return result;
}

std::string Traverse(const Map &map, absl::string_view current_position,
                     Direction direction) {
  switch (direction) {
  case Direction::LEFT:
    return map.at(current_position).left;
  case Direction::RIGHT:
    return map.at(current_position).right;
  }
  return "N/A";
}

namespace {
bool IsSource(absl::string_view node) { return node.back() == 'A'; }
bool IsTarget(absl::string_view node) { return node.back() == 'Z'; }
} // namespace

MapIndex IndexMap(const Map &map) {
  MapIndex result;
  for (const auto &[node, _] : map) {
    size_t idx = result.name_by_idx.size();
    result.idx_by_name[node] = idx;
    result.name_by_idx.push_back(node);
    if (IsSource(node)) {
      result.sources.insert(idx);
    }
    if (IsTarget(node)) {
      result.targets.insert(idx);
    }
  }
  return result;
}

TransitionMatrices ToTransitionMatrices(const Map &map, const MapIndex &index) {

  TransitionMatrices result;
  result.left = Eigen::SparseMatrix<bool>(map.size(), map.size());
  result.right = Eigen::SparseMatrix<bool>(map.size(), map.size());

  for (const auto &[node, neighbors] : map) {
    const size_t node_idx = index.idx_by_name.at(node);
    const size_t left_idx = index.idx_by_name.at(neighbors.left);
    const size_t right_idx = index.idx_by_name.at(neighbors.right);

    result.left.insert(node_idx, left_idx) = true;
    result.right.insert(node_idx, right_idx) = true;
  }

  return result;
}

Eigen::SparseMatrix<bool> GetInitialPositionMatrix(const MapIndex &index) {
  Eigen::SparseMatrix<bool> result(index.sources.size(),
                                   index.name_by_idx.size());
  size_t idx = 0;
  for (const size_t source : index.sources) {
    result.insert(idx, source) = true;
    idx++;
  }
  return result;
}

Eigen::SparseVector<bool> GetTargetPositionVector(const MapIndex &index) {
  Eigen::SparseVector<bool> result(index.name_by_idx.size());
  for (const size_t target : index.targets) {
    result.insert(target, 0) = true;
  }
  return result;
}

bool CheckComplete(const Eigen::SparseMatrix<bool> &current_positions,
                   const Eigen::SparseVector<bool> &targets) {
  return Eigen::MatrixX<bool>(current_positions * targets).all();
}

} // namespace day_8

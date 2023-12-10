#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "absl/container/btree_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/ascii.h"
#include "absl/strings/string_view.h"

#include "src/day_8/util.h"

using namespace day_8;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::string serialized_instructions;
  std::getline(std::cin, serialized_instructions);

  std::string garbage;
  std::getline(std::cin, garbage);

  std::string edge;
  std::vector<std::string> serialized_edges;
  while (std::getline(std::cin, edge)) {
    serialized_edges.push_back(edge);
  }

  Map map = *ParseMap(serialized_edges);

  { // Part 1.
    Instructions instructions = ParseInstructions(serialized_instructions);

    static constexpr absl::string_view kStartPosition = "AAA";
    static constexpr absl::string_view kTargetPosition = "ZZZ";

    uint64_t total_steps = 0;
    std::string current_position = std::string(kStartPosition);
    while (current_position != kTargetPosition) {
      current_position = Traverse(map, current_position, instructions.next());
      ++total_steps;
    }
    std::cout << "Part 1: " << total_steps << std::endl;
  }

  { // Part 2.
    const MapIndex index = IndexMap(map);
    const TransitionMatrices transitions = ToTransitionMatrices(map, index);
    const Eigen::SparseVector<bool> targets = GetTargetPositionVector(index);
    Eigen::SparseMatrix<bool> current_positions =
        GetInitialPositionMatrix(index);

    Instructions instructions = ParseInstructions(serialized_instructions);
    uint64_t total_steps = 0;
    while (!CheckComplete(current_positions, targets)) {
      switch (instructions.next()) {
      case Direction::LEFT:
        current_positions = current_positions * transitions.left;
        break;
      case Direction::RIGHT:
        current_positions = current_positions * transitions.right;
        break;
      }
      ++total_steps;
    }

    std::cout << "Part 2: " << total_steps << std::endl;
  }

  return 0;
}

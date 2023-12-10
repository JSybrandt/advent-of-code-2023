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

using ::day_8::Instructions;
using ::day_8::Map;
using ::day_8::ParseInstructions;
using ::day_8::ParseMap;
using ::day_8::Traverse;

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

  { // Part 1.
    Instructions instructions = ParseInstructions(serialized_instructions);
    Map map = *ParseMap(serialized_edges);

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
    uint64_t total = 0;
    std::cout << "Part 2: " << total << std::endl;
  }

  return 0;
}

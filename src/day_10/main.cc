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

#include "src/day_10/util.h"

using namespace day_10;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::vector<std::string> grid_data;
  for (std::string line; std::cin >> line;) {
    grid_data.push_back(line);
  }
  const PipeNetwork pipe_network(grid_data);

  { // Part 1.
    absl::flat_hash_map<Pos, uint64_t> dist_to_start;
    dist_to_start[pipe_network.start] = 0;

    const auto visit_fn = [&](const Pos &current) {
      // Ignore start when we visit it.
      if (dist_to_start.contains(current)) {
        return;
      }
      uint64_t min_dist = std::numeric_limits<uint64_t>::max();
      for (const auto &neighbor : pipe_network.network.at(current)) {
        if (dist_to_start.contains(neighbor)) {
          min_dist = std::min(min_dist, dist_to_start.at(neighbor) + 1);
        }
      }
      dist_to_start[current] = min_dist;
    };

    pipe_network.BreadthFirstTraversal(visit_fn);

    uint64_t max_distance =
        std::max_element(dist_to_start.begin(), dist_to_start.end(),
                         [](const std::pair<Pos, uint64_t> &a,
                            const std::pair<Pos, uint64_t> &b) {
                           return a.second < b.second;
                         })
            ->second;
    std::cout << "Part 1: " << max_distance << std::endl;
  }

  return 0;
}

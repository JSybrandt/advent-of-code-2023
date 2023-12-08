#include <iostream>
#include <numeric>
#include <string>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "src/day_3/util.h"

using ::day_3::Grid;
using ::day_3::GridNumber;
using ::day_3::Pos;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::vector<std::string> grid_data;
  while (std::getline(std::cin, grid_data.emplace_back())) {
  }
  Grid grid(grid_data);

  std::vector<GridNumber> numbers;
  const auto number_visit_fn = [&](const GridNumber &number) {
    if (number.adjacent_to_symbol) {
      numbers.push_back(number);
    }
  };
  grid.VisitNumbers(number_visit_fn);

  std::cout << "Part 1: "
            << std::accumulate(numbers.begin(), numbers.end(), 0,
                               [](int total, const GridNumber &n) {
                                 return n.value + total;
                               })
            << std::endl;

  absl::flat_hash_map<Pos, size_t> number_indices;
  for (size_t idx = 0; idx < numbers.size(); ++idx) {
    for (const auto &pos : numbers[idx].tile_locations) {
      number_indices[pos] = idx;
    }
  }

  int gear_ratio_total = 0;
  const auto gear_symbol_visit_fn = [&](const Pos &gear_pos) {
    absl::flat_hash_set<size_t> adj_number_indices;

    const auto adj_tile_visit_fn = [&](char, const Pos &adj_pos) {
      if (number_indices.contains(adj_pos)) {
        adj_number_indices.insert(number_indices.at(adj_pos));
      }
    };

    grid.VisitAdjacentTiles(gear_pos, adj_tile_visit_fn);

    if (adj_number_indices.size() != 2) {
      return;
    }

    int product = 1;
    for (const size_t idx : adj_number_indices) {
      product *= numbers[idx].value;
    }
    gear_ratio_total += product;
  };
  grid.VisitSymbol('*', gear_symbol_visit_fn);
  std::cout << "Part 2: " << gear_ratio_total << std::endl;
  return 0;
}

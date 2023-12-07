#include <iostream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "src/day_3/util.h"

using ::day_3::Grid;
using ::day_3::GridNumber;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::vector<std::string> grid_data;
  while (std::getline(std::cin, grid_data.emplace_back())) {
  }
  Grid grid(grid_data);

  int total = 0;
  const auto visit_fn = [&total](const GridNumber &number) {
    if (number.adjacent_to_symbol) {
      total += number.value;
    }
  };
  grid.VisitNumbers(visit_fn);
  std::cout << total << std::endl;
  return 0;
}

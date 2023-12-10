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

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::string line;
  while (std::getline(std::cin, line)) {
  }

  { // Part 1.
    uint64_t total = 0;
    std::cout << "Part 1: " << total << std::endl;
  }

  { // Part 2.
    uint64_t total = 0;
    std::cout << "Part 2: " << total << std::endl;
  }

  return 0;
}

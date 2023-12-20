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

#include "src/day_9/util.h"

using namespace day_9;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::vector<std::vector<int64_t>> sequences;

  std::string line;
  while (std::getline(std::cin, line)) {
    sequences.push_back(ParseSequence(line));
  }

  { // Part 1.
    int64_t total = 0;
    for (const auto &sequence : sequences) {
      total += ExtrapolateNext(sequence);
    }
    std::cout << "Part 1: " << total << std::endl;
  }

  { // Part 1.
    int64_t total = 0;
    for (const auto &sequence : sequences) {
      total += ExtrapolatePrev(sequence);
    }
    std::cout << "Part 2: " << total << std::endl;
  }

  return 0;
}

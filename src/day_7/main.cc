#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/ascii.h"
#include "absl/strings/string_view.h"

#include "src/day_7/util.h"

using ::day_7::HandAndBid;
using ::day_7::ParseHandAndBid;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::vector<HandAndBid> hands;
  std::string serialized_hand;
  while (std::getline(std::cin, serialized_hand)) {
    hands.push_back(*ParseHandAndBid(serialized_hand));
  }

  { // Part 1.
    uint64_t total = 0;
    std::sort(hands.begin(), hands.end());
    for (size_t idx = 0; idx < hands.size(); ++idx) {
      uint64_t hand_rank = idx + 1;
      std::cout << "Hand: " << hands[idx].hand << " : " << hands[idx].bid
                << " : " << hand_rank << std::endl;
      total += hand_rank * hands[idx].bid;
    }
    std::cout << "Part 1: " << total << std::endl;
  }

  return 0;
}

#include <iostream>
#include <numeric>
#include <string>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "src/day_4/util.h"

using ::day_4::Card;
using ::day_4::OverlapSize;
using ::day_4::ParseCard;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  int total = 0;
  std::string serialized_card;
  while (std::getline(std::cin, serialized_card)) {
    const Card card = *ParseCard(serialized_card);
    const int overlap = OverlapSize(card.winning_numbers, card.your_numbers);
    if (overlap == 0) {
      continue;
    }
    total += std::pow(2, overlap - 1);
  }
  std::cout << "Part 1: " << total << std::endl;
  return 0;
}

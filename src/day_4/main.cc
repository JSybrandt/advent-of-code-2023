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

  std::vector<Card> cards;

  std::string serialized_card;
  while (std::getline(std::cin, serialized_card)) {
    cards.push_back(*ParseCard(serialized_card));
  }

  { // Part 1.
    int total = 0;
    for (const auto &card : cards) {
      total += card.points;
    }
    std::cout << "Part 1: " << total << std::endl;
  }

  { // Part 2.

    std::vector<int> card_copies(cards.size(), 1);
    for (size_t idx = 0; idx < cards.size(); ++idx) {
      for (size_t offset = 1;
           offset <= cards[idx].num_matches && idx + offset < cards.size();
           ++offset) {
        card_copies[idx + offset] += card_copies[idx];
      }
    }
    std::cout << "Part 2: "
              << std::accumulate(card_copies.begin(), card_copies.end(), 0)
              << std::endl;
  }

  return 0;
}

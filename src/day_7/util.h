#ifndef SRC_DAY_7_UTIL_H_
#define SRC_DAY_7_UTIL_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_7 {

enum class CardValue {
  JOKER = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14
};
std::ostream &operator<<(std::ostream &o, const CardValue hand);

// A set of 5 cards.
using HandOfCards = std::array<CardValue, 5>;
// Order hands by strength.
bool operator<(const HandOfCards &a, const HandOfCards &b);
std::ostream &operator<<(std::ostream &o, const HandOfCards &hand);
absl::StatusOr<HandOfCards> ParseHand(absl::string_view serialized_hand);

enum class HandType {
  HIGH_CARD,
  PAIR,
  TWO_PAIR,
  TRIPLE,
  FULL_HOUSE,
  QUARTET,
  QUINTET
};

HandType GetHandType(const HandOfCards &hand);

struct HandAndBid {
  HandOfCards hand;
  uint64_t bid;
  bool operator==(const HandAndBid &other) const {
    return this->hand == other.hand && this->bid == other.bid;
  }
  bool operator<(const HandAndBid &other) { return this->hand < other.hand; }
};
std::ostream &operator<<(std::ostream &o, const HandAndBid &hand_and_bid);

absl::StatusOr<HandAndBid>
ParseHandAndBid(absl::string_view serialized_hand_and_bid);

HandOfCards ConvertJacksToJokers(const HandOfCards &hand);

} // namespace day_7

#endif // SRC_DAY_7_UTIL_H_

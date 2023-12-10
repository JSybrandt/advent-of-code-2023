#include "src/day_7/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <sstream>
#include <unistd.h>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_7 {
namespace {
static const auto *kValueByCardChar = new absl::flat_hash_map<char, CardValue>{
    {'A', CardValue::ACE},   {'K', CardValue::KING},  {'Q', CardValue::QUEEN},
    {'J', CardValue::JACK},  {'T', CardValue::TEN},   {'9', CardValue::NINE},
    {'8', CardValue::EIGHT}, {'7', CardValue::SEVEN}, {'6', CardValue::SIX},
    {'5', CardValue::FIVE},  {'4', CardValue::FOUR},  {'3', CardValue::THREE},
    {'2', CardValue::TWO},
};

static const auto *kCardCharByValue = new absl::flat_hash_map<CardValue, char>{
    {CardValue::ACE, 'A'},   {CardValue::KING, 'K'},  {CardValue::QUEEN, 'Q'},
    {CardValue::JACK, 'J'},  {CardValue::TEN, 'T'},   {CardValue::NINE, '9'},
    {CardValue::EIGHT, '8'}, {CardValue::SEVEN, '7'}, {CardValue::SIX, '6'},
    {CardValue::FIVE, '5'},  {CardValue::FOUR, '4'},  {CardValue::THREE, '3'},
    {CardValue::TWO, '2'},   {CardValue::JOKER, 'J'}};
} // namespace

bool operator<(const HandOfCards &a, const HandOfCards &b) {
  const auto a_type = GetHandType(a);
  const auto b_type = GetHandType(b);
  // Only consider the first set.
  if (a_type != b_type) {
    return a_type < b_type;
  }
  // If equal, compare the cards in order.
  for (size_t idx = 0; idx < a.size(); ++idx) {
    if (a[idx] != b[idx]) {
      return a[idx] < b[idx];
    }
  }
  return false;
}

HandType GetHandType(const HandOfCards &hand) {
  absl::flat_hash_map<CardValue, size_t> card_counts;
  for (const auto &value : hand) {
    ++card_counts[value];
  }
  // Remember and clear out the jokers.
  const size_t joker_count = card_counts[CardValue::JOKER];
  card_counts[CardValue::JOKER] = 0;
  std::vector<size_t> ordered_card_counts;
  for (const auto &[_, card_count] : card_counts) {
    ordered_card_counts.push_back(card_count);
  }
  std::sort(ordered_card_counts.begin(), ordered_card_counts.end());

  switch (ordered_card_counts.back() + joker_count) {
  case 5:
    return HandType::QUINTET;
  case 4:
    return HandType::QUARTET;
  case 3:
    if (ordered_card_counts[ordered_card_counts.size() - 2] == 2) {
      return HandType::FULL_HOUSE;
    } else {
      return HandType::TRIPLE;
    }
  case 2:
    if (ordered_card_counts[ordered_card_counts.size() - 2] == 2) {
      return HandType::TWO_PAIR;
    } else {
      return HandType::PAIR;
    }
    break;
  case 1:
    return HandType::HIGH_CARD;
  default:
    // This should never happen.
    CHECK(false);
    return HandType::HIGH_CARD;
  }
}

absl::StatusOr<HandOfCards> ParseHand(absl::string_view serialized_hand) {
  HandOfCards result;
  if (serialized_hand.size() != result.size()) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid Hand. Expected ", result.size(), " cards. Found ",
                     serialized_hand.size()));
  }
  for (size_t idx = 0; idx < serialized_hand.size(); ++idx) {
    const char c = serialized_hand[idx];
    if (!kValueByCardChar->contains(c)) {
      return absl::InvalidArgumentError(
          absl::StrCat("Invalid Hand. Contains bad card: ", serialized_hand));
    }
    result[idx] = kValueByCardChar->at(c);
  }
  return result;
}

absl::StatusOr<HandAndBid>
ParseHandAndBid(absl::string_view serialized_hand_and_bid) {
  std::vector<std::string> tokens =
      absl::StrSplit(absl::StripAsciiWhitespace(serialized_hand_and_bid), " ");
  if (tokens.size() != 2) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid hand and bid:", serialized_hand_and_bid));
  }
  absl::StatusOr<HandOfCards> hand = ParseHand(tokens[0]);
  if (!hand.ok()) {
    return hand.status();
  }
  uint64_t bid;
  if (!absl::SimpleAtoi(tokens[1], &bid)) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid hand and bid:", serialized_hand_and_bid));
  }
  return HandAndBid{*hand, bid};
}

HandOfCards ConvertJacksToJokers(const HandOfCards &hand) {
  HandOfCards result = hand;
  for (auto &c : result) {
    if (c == CardValue::JACK) {
      c = CardValue::JOKER;
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Debug Logging ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &o, const HandOfCards &hand) {
  for (const auto card : hand) {
    o << card;
  }
  return o;
}

std::ostream &operator<<(std::ostream &o, const CardValue card) {
  o << kCardCharByValue->at(card);
  return o;
}

std::ostream &operator<<(std::ostream &o, const HandAndBid &hand_and_bid) {
  o << hand_and_bid.hand << ":" << hand_and_bid.bid;
  return o;
}

} // namespace day_7

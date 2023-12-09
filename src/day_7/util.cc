#include "src/day_7/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <sstream>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_7 {

bool CardSet::operator==(const CardSet &other) const {
  return this->set_strength == other.set_strength &&
         std::equal(this->card_values.begin(), this->card_values.end(),
                    other.card_values.begin());
}
bool CardSet::operator<(const CardSet &other) const {
  if (this->set_strength != other.set_strength) {
    return this->set_strength < other.set_strength;
  }
  // Two sets with the same strength will have the same number of card_values.
  CHECK_EQ(this->card_values.size(), other.card_values.size());
  for (size_t idx = 0; idx < this->card_values.size(); ++idx) {
    const auto &a = this->card_values[idx];
    const auto &b = other.card_values[idx];
    if (a != b) {
      return a < b;
    }
  }
  // Everything is equal.
  return false;
}

bool operator<(const HandOfCards &a, const HandOfCards &b) {
  std::vector<CardSet> a_sets = ToOrderedCardSets(a);
  std::vector<CardSet> b_sets = ToOrderedCardSets(b);
  for (size_t idx = 0; idx < std::min(a_sets.size(), b_sets.size()); ++idx) {
    const auto &set_a = a_sets[idx];
    const auto &set_b = b_sets[idx];
    if (set_a != set_b) {
      return set_a < set_b;
    }
  }
  return false;
}

namespace {
using CardCount = std::pair<CardValue, size_t>;
bool SortCardCounts(const CardCount &a, const CardCount &b) {
  if (a.second == b.second)
    return a.first < b.first;
  return a.second < b.second;
}
} // namespace

std::vector<CardSet> ToOrderedCardSets(const HandOfCards &hand) {
  absl::flat_hash_map<CardValue, size_t> card_counts;
  for (const auto &value : hand) {
    ++card_counts[value];
  }
  std::vector<CardCount> ordered_counts = {card_counts.begin(),
                                           card_counts.end()};
  std::sort(ordered_counts.begin(), ordered_counts.end(), &SortCardCounts);

  std::vector<CardSet> card_sets;
  while (!ordered_counts.empty()) {
    const auto [current_value, current_count] = ordered_counts.back();
    ordered_counts.pop_back();
    // We want a one-set lookahead.
    std::optional<CardValue> next_value;
    std::optional<size_t> next_count;
    if (!ordered_counts.empty()) {
      next_value = ordered_counts.back().first;
      next_count = ordered_counts.back().second;
    }
    switch (current_count) {
    case 5:
      card_sets.push_back({{current_value}, SetStrength::QUINTET});
      break;
    case 4:
      card_sets.push_back({{current_value}, SetStrength::QUARTET});
      break;
    case 3:
      if (next_count == 2) {
        ordered_counts.pop_back(); // Pop the lookahead.
        card_sets.push_back(
            {{current_value, *next_value}, SetStrength::FULL_HOUSE});
      } else {
        card_sets.push_back({{current_value}, SetStrength::TRIPLE});
      }
      break;
    case 2:
      if (next_count == 2) {
        ordered_counts.pop_back();
        card_sets.push_back(
            {{current_value, *next_value}, SetStrength::TWO_PAIR});
      } else {
        card_sets.push_back({{current_value}, SetStrength::PAIR});
      }
      break;
    case 1:
      card_sets.push_back({{current_value}, SetStrength::HIGH_CARD});
      break;
    default:
      CHECK(false);
      break;
    }
  }
  return card_sets;
}

std::ostream &operator<<(std::ostream &o, const CardSet &card_set) {
  o << "[" << absl::StrCat(card_set.set_strength) << "]: { ";
  for (const auto &value : card_set.card_values) {
    o << absl::StrCat(value) << " ";
  }
  o << "}";
  return o;
}

absl::StatusOr<HandOfCards> ParseHand(absl::string_view serialized_hand) {
  static const auto *kCardValueMap = new absl::flat_hash_map<char, CardValue>{
      {'A', CardValue::ACE},   {'K', CardValue::KING},  {'Q', CardValue::QUEEN},
      {'J', CardValue::JACK},  {'T', CardValue::TEN},   {'9', CardValue::NINE},
      {'8', CardValue::EIGHT}, {'7', CardValue::SEVEN}, {'6', CardValue::SIX},
      {'5', CardValue::FIVE},  {'4', CardValue::FOUR},  {'3', CardValue::THREE},
      {'2', CardValue::TWO}};
  HandOfCards result;
  if (serialized_hand.size() != result.size()) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid Hand. Expected ", result.size(), " cards. Found ",
                     serialized_hand.size()));
  }
  for (size_t idx = 0; idx < serialized_hand.size(); ++idx) {
    const char c = serialized_hand[idx];
    if (!kCardValueMap->contains(c)) {
      return absl::InvalidArgumentError(
          absl::StrCat("Invalid Hand. Contains bad card: ", serialized_hand));
    }
    result[idx] = kCardValueMap->at(c);
  }
  std::sort(result.begin(), result.end());
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

} // namespace day_7

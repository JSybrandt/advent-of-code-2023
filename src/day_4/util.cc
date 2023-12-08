#include "src/day_4/util.h"

#include <cctype>
#include <iostream>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_4 {
absl::StatusOr<Card> ParseCard(absl::string_view serialized_card) {
  const auto invalid_error = [&](absl::string_view msg) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid card: ", serialized_card, " ", msg));
  };
  std::vector<std::string> split_card =
      absl::StrSplit(absl::StripAsciiWhitespace(serialized_card), ':');
  if (split_card.size() != 2) {
    return invalid_error(
        absl::StrCat("split_card.size() = ", split_card.size(), "!=2"));
  }
  std::vector<std::string> split_contents =
      absl::StrSplit(absl::StripAsciiWhitespace(split_card[1]), '|');
  if (split_contents.size() != 2) {
    return invalid_error(
        absl::StrCat("split_contents.size() = ", split_card.size(), "!=2"));
  }
  std::vector<std::string> winning_numbers =
      absl::StrSplit(absl::StripAsciiWhitespace(split_contents[0]), ' ');
  std::vector<std::string> your_numbers =
      absl::StrSplit(absl::StripAsciiWhitespace(split_contents[1]), ' ');

  Card result;
  for (const auto &winning_number : winning_numbers) {
    if (winning_number.empty())
      continue;
    int num;
    if (!absl::SimpleAtoi(winning_number, &num)) {
      return invalid_error(
          absl::StrCat("Not a winning number: '", winning_number, "'"));
    }
    result.winning_numbers.insert(num);
  }

  for (const auto &your_number : your_numbers) {
    if (your_number.empty())
      continue;
    int num;
    if (!absl::SimpleAtoi(your_number, &num)) {
      return invalid_error(
          absl::StrCat("Not a your number: '", your_number, "'"));
    }
    result.your_numbers.insert(num);
  }
  return result;
}

int OverlapSize(const absl::flat_hash_set<int> &a,
                const absl::flat_hash_set<int> &b) {
  int result = 0;
  for (const int x : a) {
    if (b.contains(x)) {
      ++result;
    }
  }
  return result;
}
} // namespace day_4

#include <iostream>
#include <optional>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "src/day_1/util.h"

ABSL_FLAG(bool, include_english_numbers, false,
          "If set, consider english numbers.");

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  auto number_strings = day_1::GetDigitNumberStrings();
  if (absl::GetFlag(FLAGS_include_english_numbers)) {
    number_strings.merge(day_1::GetEnglishNumberStrings());
  }

  int64_t total = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::optional<int> first_number =
        FindFirst(line, number_strings, day_1::SearchDirection::FORWARD);
    if (!first_number.has_value()) {
      return 1;
    }
    std::optional<int> last_number =
        FindFirst(line, number_strings, day_1::SearchDirection::BACKWARD);
    if (!last_number.has_value()) {
      return 1;
    }
    total += *first_number * 10 + *last_number;
  }
  std::cout << total << std::endl;
  return 0;
}

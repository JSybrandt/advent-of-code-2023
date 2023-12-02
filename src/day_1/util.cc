#include "src/day_1/util.h"

#include <cctype>
#include <algorithm>

#include "absl/strings/string_view.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"

namespace day_1 {

namespace {
  int ToInt(const char c){
    return c - '0';
  }
}

absl::StatusOr<int> GetEncodedNumber(absl::string_view encoding){
  auto first_digit = std::find_if(encoding.begin(), encoding.end(), isdigit);
  if(first_digit == encoding.end()){
    return absl::InvalidArgumentError("Failed to find a digit.");
  }
  auto second_digit = std::find_if(encoding.rbegin(), encoding.rend(), isdigit);
  if(second_digit == encoding.rend()){
    // This shouldn't happen assuming we passed the above.
    return absl::InvalidArgumentError("Failed to find the second digit.");
  }
  return ToInt(*first_digit) * 10 + ToInt(*second_digit);
}

} // day_1

#ifndef SRC_DAY_1_UTIL_H_
#define SRC_DAY_1_UTIL_H_

#include <optional>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_1 {

using NumberStrings = absl::flat_hash_map<std::string, int>;

inline NumberStrings GetDigitNumberStrings() {
  return {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5},
          {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}};
}

inline NumberStrings GetEnglishNumberStrings() {
  return {{"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
          {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
}

enum class SearchDirection { FORWARD, BACKWARD };

std::optional<int> FindFirst(absl::string_view query,
                             const NumberStrings &number_strings,
                             const SearchDirection direction);

// Returns the number of characters that needed to be checked before finding the
// target in the query. Can search either direction.
std::optional<size_t> FindSubstringOffset(absl::string_view query,
                                          absl::string_view target,
                                          const SearchDirection direction);

} // namespace day_1

#endif // SRC_DAY_1_UTIL_H_

#include "src/day_1/util.h"

#include <algorithm>
#include <cctype>
#include <optional>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"

namespace day_1 {

// Returns the number of characters that needed to be checked before finding the
// target in the query. Can search either direction.
std::optional<size_t> FindSubstringOffset(absl::string_view query,
                                          absl::string_view target,
                                          const SearchDirection direction) {
  if (target.size() > query.size()) {
    return std::nullopt;
  }

  auto search_substr = [&](const size_t search_idx) -> absl::string_view {
    switch (direction) {
    case SearchDirection::FORWARD:
      return query.substr(search_idx, target.size());
    case SearchDirection::BACKWARD:
      return query.substr(query.size() - search_idx - target.size(),
                          target.size());
    }
  };

  for (size_t search_idx = 0; search_idx < query.size() - target.size() + 1;
       ++search_idx) {
    if (search_substr(search_idx) == target) {
      return search_idx;
    }
  }
  return std::nullopt;
}

std::optional<int> FindFirst(absl::string_view query,
                             const NumberStrings &number_strings,
                             const SearchDirection direction) {
  std::optional<size_t> min_offset;
  std::optional<int> result;
  for (const auto &[text, value] : number_strings) {
    const std::optional<size_t> offset =
        FindSubstringOffset(query, text, direction);
    if (!offset.has_value())
      continue;
    if (!min_offset.has_value() || *min_offset > *offset) {
      min_offset = offset;
      result = value;
    }
  }
  return result;
}

} // namespace day_1

#ifndef SRC_DAY_4_UTIL_H_
#define SRC_DAY_4_UTIL_H_

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_4 {

struct Card {
  absl::flat_hash_set<int> winning_numbers;
  absl::flat_hash_set<int> your_numbers;
};

absl::StatusOr<Card> ParseCard(absl::string_view serialized_card);

int OverlapSize(const absl::flat_hash_set<int> &a,
                const absl::flat_hash_set<int> &b);

} // namespace day_4

#endif // SRC_DAY_4_UTIL_H_

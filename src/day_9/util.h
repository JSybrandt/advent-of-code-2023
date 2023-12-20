#ifndef SRC_DAY_9_UTIL_H_
#define SRC_DAY_9_UTIL_H_

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

namespace day_9 {

std::vector<int64_t> ParseSequence(absl::string_view sequence);

std::vector<int64_t> GetDeltaSequence(const std::vector<int64_t> &sequence);

bool IsAllZeros(const std::vector<int64_t> &sequence);

int64_t ExtrapolateNext(const std::vector<int64_t> &sequence);

int64_t ExtrapolatePrev(const std::vector<int64_t> &sequence);

} // namespace day_9

#endif // SRC_DAY_9_UTIL_H_

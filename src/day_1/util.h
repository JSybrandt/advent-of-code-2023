#ifndef SRC_DAY_1_UTIL_H_
#define SRC_DAY_1_UTIL_H_

#include "absl/strings/string_view.h"
#include "absl/status/statusor.h"

namespace day_1 {

absl::StatusOr<int> GetEncodedNumber(absl::string_view encoding);

} // day_1

#endif // SRC_DAY_1_UTIL_H_

#include "src/day_9/util.h"

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

namespace day_9 {

std::vector<int64_t> ParseSequence(absl::string_view sequence) {
  std::vector<int64_t> result;
  for (const auto &token : absl::StrSplit(sequence, " ")) {
    int64_t num;
    CHECK(absl::SimpleAtoi(token, &num));
    result.push_back(num);
  }
  return result;
}

std::vector<int64_t> GetDeltaSequence(const std::vector<int64_t> &sequence) {
  std::vector<int64_t> deltas;
  for (size_t idx = 1; idx < sequence.size(); ++idx) {
    deltas.push_back(sequence[idx] - sequence[idx - 1]);
  }
  return deltas;
}

namespace {
bool IsZero(const int64_t v) { return v == 0; }
} // namespace

bool IsAllZeros(const std::vector<int64_t> &sequence) {
  return std::all_of(sequence.begin(), sequence.end(), &IsZero);
}

int64_t ExtrapolateNext(const std::vector<int64_t> &sequence) {
  std::vector<std::vector<int64_t>> sequence_stack = {sequence};
  while (!IsAllZeros(sequence_stack.back())) {
    sequence_stack.push_back(GetDeltaSequence(sequence_stack.back()));
  }
  while (sequence_stack.size() > 1) {
    // The last value of the sequence at the top of the stack.
    int64_t last_value = sequence_stack.back().back();
    // Remove the sequence at the top of the stack.
    sequence_stack.pop_back();
    // Add the last_value to the current top of the stack.
    sequence_stack.back().push_back(sequence_stack.back().back() + last_value);
  }
  return sequence_stack.back().back();
}

int64_t ExtrapolatePrev(const std::vector<int64_t> &sequence) {
  std::vector<int64_t> reversed_sequence = sequence;
  std::reverse(reversed_sequence.begin(), reversed_sequence.end());
  return ExtrapolateNext(reversed_sequence);
}

} // namespace day_9

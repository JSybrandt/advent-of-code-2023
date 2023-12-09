#include "src/day_5/util.h"

#include <cctype>
#include <iostream>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_5 {

absl::StatusOr<CategoryMapper> ParseCategoryMapper(
    const std::vector<std::string> &serialized_category_mapper) {
  CategoryMapper result;
  std::vector<std::string> header_tokens =
      absl::StrSplit(serialized_category_mapper[0], absl::ByAnyChar("- "));
  // "seed-to-soil map:" -> {"seed", "to", "soil", "map:"}
  if (header_tokens.size() != 4) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid header: ", serialized_category_mapper[0]));
  }
  result.source_category = header_tokens[0];
  result.destination_category = header_tokens[2];

  for (size_t idx = 1; idx < serialized_category_mapper.size(); ++idx) {
    absl::StatusOr<MappingInterval> interval =
        ParseInterval(serialized_category_mapper[idx]);
    if (!interval.ok()) {
      return interval.status();
    }
    result.intervals.push_back(*interval);
  }
  return result;
}

absl::StatusOr<MappingInterval>
ParseInterval(absl::string_view serialized_interval) {
  MappingInterval interval;
  std::vector<std::string> map_tokens =
      absl::StrSplit(serialized_interval, " ");
  if (map_tokens.size() != 3) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid map: ", serialized_interval));
  }
  if (!absl::SimpleAtoi(map_tokens[0], &interval.destination_range_start)) {
    return absl::InvalidArgumentError("Invalid destination_range_start: " +
                                      map_tokens[0]);
  }
  if (!absl::SimpleAtoi(map_tokens[1], &interval.source_range_start)) {
    return absl::InvalidArgumentError("Invalid source_range_start: " +
                                      map_tokens[1]);
  }
  if (!absl::SimpleAtoi(map_tokens[2], &interval.range_length)) {
    return absl::InvalidArgumentError("Invalid range_length: " + map_tokens[2]);
  }
  return interval;
}

uint64_t MappingInterval::operator()(const int64_t source_idx) const {
  if (!ContainsSource(source_idx)) {
    return source_idx;
  }
  const uint64_t offset = source_idx - source_range_start;
  return destination_range_start + offset;
}

uint64_t CategoryMapper::operator()(const int64_t source_idx) const {
  // Search for the right interval, otherwise just pass thru.
  for (const auto &interval : intervals) {
    if (interval.ContainsSource(source_idx)) {
      return interval(source_idx);
    }
  }
  return source_idx;
}

absl::StatusOr<std::vector<uint64_t>>
ParseInitialSeeds(absl::string_view serialized_seeds) {
  std::vector<uint64_t> result;
  std::vector<std::string> tokens = absl::StrSplit(serialized_seeds, ' ');
  // Skip the first token, thats just the word "seeds:".
  for (size_t idx = 1; idx < tokens.size(); ++idx) {
    uint64_t seed;
    if (!absl::SimpleAtoi(tokens[idx], &seed)) {
      return absl::InvalidArgumentError("Invalid seed: " + tokens[seed]);
    }
    result.push_back(seed);
  }
  return result;
}

uint64_t TraverseCategories(
    uint64_t initial_idx, absl::string_view initial_category,
    const absl::flat_hash_map<std::string, CategoryMapper> &mappers) {
  uint64_t current_idx = initial_idx;
  std::string current_category = std::string(initial_category);
  while (mappers.contains(current_category)) {
    const auto &mapper = mappers.at(current_category);
    current_idx = mapper(current_idx);
    current_category = mapper.destination_category;
  }
  return current_idx;
}

} // namespace day_5

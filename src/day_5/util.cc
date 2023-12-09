#include "src/day_5/util.h"

#include <algorithm>
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

uint64_t MappingInterval::Map(const int64_t source_idx) const {
  if (!ContainsSource(source_idx)) {
    return source_idx;
  }
  const uint64_t offset = source_idx - source_range_start;
  return destination_range_start + offset;
}

bool MappingInterval::ContainsSourceRange(const IndexRange &index_range) const {
  if (index_range.range_start >= source_range_start + range_length) {
    return false;
  }
  if (index_range.range_start + index_range.range_length <=
      source_range_start) {
    return false;
  }
  return true;
}

uint64_t CategoryMapper::Map(const int64_t source_idx) const {
  // Search for the right interval, otherwise just pass thru.
  for (const auto &interval : intervals) {
    if (interval.ContainsSource(source_idx)) {
      return interval.Map(source_idx);
    }
  }
  return source_idx;
}

std::vector<IndexRange>
MappingInterval::MapRange(const IndexRange &index_range) const {
  if (!ContainsSourceRange(index_range)) {
    return {index_range};
  }
  std::vector<IndexRange> result;
  // This portion of the interval is before the mapped interval.
  if (index_range.range_start < source_range_start) {
    result.push_back(IndexRange{.range_start = index_range.range_start,
                                .range_length = source_range_start -
                                                index_range.range_start});
  }

  // This portion of the interval is within the mapped interval.
  const uint64_t overlap_start =
      std::max(source_range_start, index_range.range_start);
  const uint64_t overlap_end =
      std::min(source_range_end(), index_range.range_end());
  const uint64_t start_offset = overlap_start - source_range_start;
  result.push_back(
      IndexRange{.range_start = destination_range_start + start_offset,
                 .range_length = overlap_end - overlap_start,
                 .was_remapped_by_interval = true});

  // This portion of the interval is after the mapped interval.
  if (index_range.range_end() > source_range_end()) {
    result.push_back(IndexRange{.range_start = source_range_end(),
                                .range_length = index_range.range_end() -
                                                source_range_end()});
  }
  return result;
}

absl::StatusOr<std::vector<uint64_t>>
ParseSeedsAsIndependent(absl::string_view serialized_seeds) {
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
    current_idx = mapper.Map(current_idx);
    current_category = mapper.destination_category;
  }
  return current_idx;
}

absl::StatusOr<std::vector<IndexRange>>
ParseSeedsAsRanges(absl::string_view serialized_seeds) {
  std::vector<IndexRange> result;
  std::vector<std::string> tokens = absl::StrSplit(serialized_seeds, ' ');
  // Skip the first token, thats just the word "seeds:".
  for (size_t idx = 1; idx < tokens.size(); idx += 2) {
    IndexRange index_range;
    if (!absl::SimpleAtoi(tokens[idx], &index_range.range_start)) {
      return absl::InvalidArgumentError("Invalid range_start: " + tokens[idx]);
    }
    if (!absl::SimpleAtoi(tokens[idx + 1], &index_range.range_length)) {
      return absl::InvalidArgumentError("Invalid range_length: " +
                                        tokens[idx + 1]);
    }
    result.push_back(index_range);
  }
  return result;
}

std::vector<IndexRange>
CategoryMapper::MapRange(const IndexRange &index_range) const {
  std::vector<IndexRange> source_ranges = {index_range};
  std::vector<IndexRange> destination_ranges;
  for (const auto &interval : intervals) {
    std::vector<IndexRange> new_source_ranges;
    for (const auto &source_range : source_ranges) {
      for (const auto &mapped_range : interval.MapRange(source_range)) {
        if (mapped_range.was_remapped_by_interval) {
          destination_ranges.push_back(mapped_range);
        } else {
          new_source_ranges.push_back(mapped_range);
        }
      }
    }
    source_ranges = new_source_ranges;
  }
  for (auto &r : destination_ranges) {
    r.was_remapped_by_interval = false;
  }
  destination_ranges.insert(destination_ranges.end(), source_ranges.begin(),
                            source_ranges.end());
  std::sort(destination_ranges.begin(), destination_ranges.end());
  return destination_ranges;
}

std::vector<IndexRange> RangeTraverseCategories(
    const IndexRange &initial_index_range, absl::string_view initial_category,
    const absl::flat_hash_map<std::string, CategoryMapper> &mappers) {
  std::vector<IndexRange> current_ranges = {initial_index_range};
  std::string current_category = std::string(initial_category);

  while (mappers.contains(current_category)) {
    const auto &mapper = mappers.at(current_category);
    std::vector<IndexRange> destination_ranges;
    for (const auto &current_range : current_ranges) {
      for (const auto &dest_range : mapper.MapRange(current_range)) {
        destination_ranges.push_back(dest_range);
      }
    }
    current_ranges = destination_ranges;
    current_category = mapper.destination_category;
  }
  return current_ranges;
}

} // namespace day_5

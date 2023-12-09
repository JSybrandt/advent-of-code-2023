#ifndef SRC_DAY_5_UTIL_H_
#define SRC_DAY_5_UTIL_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_5 {

struct IndexRange {
  uint64_t range_start;
  uint64_t range_length;
  uint64_t range_end() const { return range_start + range_length; }
  bool was_remapped_by_interval = false;

  // Print the interval for debugging purposes.
  friend std::ostream &operator<<(std::ostream &o,
                                  const IndexRange &index_range) {
    o << "[" << index_range.range_start << ", "
      << index_range.range_start + index_range.range_length << ")";
    if (index_range.was_remapped_by_interval) {
      o << "*";
    }
    return o;
  }
  bool operator==(const IndexRange &other) const {
    return this->range_start == other.range_start &&
           this->range_length == other.range_length;
  }
  // Enable sorting by range_start.
  bool operator<(const IndexRange &other) const {
    return this->range_start < other.range_start;
  }
};

struct MappingInterval {
  uint64_t source_range_start;
  uint64_t destination_range_start;
  uint64_t range_length;
  uint64_t source_range_end() const {
    return source_range_start + range_length;
  }

  bool ContainsSource(const uint64_t source_idx) const {
    return source_range_start <= source_idx &&
           source_idx < source_range_start + range_length;
  }

  bool ContainsSourceRange(const IndexRange &index_range) const;

  uint64_t Map(int64_t source_idx) const;
  std::vector<IndexRange> MapRange(const IndexRange &index_range) const;

  // Print the interval for debugging purposes.
  friend std::ostream &operator<<(std::ostream &o,
                                  const MappingInterval &interval) {
    o << "(" << interval.source_range_start << " -> "
      << interval.destination_range_start << "):" << interval.range_length;
    return o;
  }
  bool operator==(const MappingInterval &other) const {
    return this->source_range_start == other.source_range_start &&
           this->destination_range_start == other.destination_range_start &&
           this->range_length == other.range_length;
  }
};

absl::StatusOr<MappingInterval>
ParseInterval(absl::string_view serialized_interval);

struct CategoryMapper {
  std::string source_category;
  std::string destination_category;
  // Should be sorted by source_idx.
  std::vector<MappingInterval> intervals;
  uint64_t Map(int64_t source_idx) const;
  std::vector<IndexRange> MapRange(const IndexRange &index_range) const;

  // Print the mapper for debugging purposes.
  friend std::ostream &operator<<(std::ostream &o,
                                  const CategoryMapper &mapper) {
    o << "(" << mapper.source_category << " -> " << mapper.destination_category
      << "\n";
    for (const auto &interval : mapper.intervals) {
      o << "  " << interval << "\n";
    }
    return o;
  }

  bool operator==(const CategoryMapper &other) const {
    return this->source_category == other.source_category &&
           this->destination_category == other.destination_category &&
           std::equal(this->intervals.begin(), this->intervals.end(),
                      other.intervals.begin());
  }
};

absl::StatusOr<CategoryMapper>
ParseCategoryMapper(const std::vector<std::string> &serialized_category_mapper);

absl::StatusOr<std::vector<uint64_t>>
ParseSeedsAsIndependent(absl::string_view serialized_seeds);

absl::StatusOr<std::vector<IndexRange>>
ParseSeedsAsRanges(absl::string_view serialized_seeds);

// Traverses from the initial category through the mappers until no mapper can
// be found.
uint64_t TraverseCategories(
    uint64_t initial_idx, absl::string_view initial_category,
    const absl::flat_hash_map<std::string, CategoryMapper> &mappers);

// Passes the initial range through the mappers until the final output ranges.
std::vector<IndexRange> RangeTraverseCategories(
    const IndexRange &initial_index_range, absl::string_view initial_category,
    const absl::flat_hash_map<std::string, CategoryMapper> &mappers);

} // namespace day_5

#endif // SRC_DAY_5_UTIL_H_

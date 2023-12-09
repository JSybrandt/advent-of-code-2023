#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/ascii.h"
#include "absl/strings/string_view.h"

#include "src/day_5/util.h"

using ::day_5::CategoryMapper;
using ::day_5::ParseCategoryMapper;
using ::day_5::ParseSeedsAsIndependent;
using ::day_5::TraverseCategories;

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  std::string serialized_seeds;
  if (!std::getline(std::cin, serialized_seeds)) {
    std::cerr << "Failed to get seed line.";
    return 1;
  }

  // Read one empty line between seeds and mappers.
  std::string line;
  std::getline(std::cin, line);

  // Index each mapper by its source category.
  absl::flat_hash_map<std::string, CategoryMapper> mappers;

  // Collect one mapper's data, and then fill mappers.
  std::vector<std::string> serialized_category_mapper;
  const auto maybe_add_mapper = [&]() {
    if (!serialized_category_mapper.empty()) {
      absl::StatusOr<CategoryMapper> mapper =
          ParseCategoryMapper(serialized_category_mapper);
      serialized_category_mapper.clear();
      mappers[mapper->source_category] = *mapper;
    }
  };
  while (std::getline(std::cin, line)) {
    line = absl::StripAsciiWhitespace(line);
    if (line.empty()) {
      maybe_add_mapper();
    } else {
      serialized_category_mapper.push_back(line);
    }
  }
  maybe_add_mapper();

  {
    std::vector<uint64_t> seeds = *ParseSeedsAsIndependent(serialized_seeds);
    static constexpr absl::string_view kInitialSource = "seed";
    uint64_t smallest_location = std::numeric_limits<uint64_t>::max();
    for (const uint64_t seed_idx : seeds) {
      smallest_location =
          std::min(smallest_location,
                   TraverseCategories(seed_idx, kInitialSource, mappers));
    }
    std::cout << "Part 1: " << smallest_location << std::endl;
  }

  return 0;
}

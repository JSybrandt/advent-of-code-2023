#include "src/day_5/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <vector>

namespace day_5 {
namespace {

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

TEST(DayFiveTest, ParseIntervalWorks) {
  EXPECT_EQ(*ParseInterval("1 2 3"),
            (MappingInterval{.source_range_start = 2,
                             .destination_range_start = 1,
                             .range_length = 3}));
}

TEST(DayFiveTest, ContainsSourceIdxWorks) {
  MappingInterval interval{.source_range_start = 100,
                           .destination_range_start = 200,
                           .range_length = 3};
  EXPECT_FALSE(interval.ContainsSource(99));
  EXPECT_TRUE(interval.ContainsSource(100));
  EXPECT_TRUE(interval.ContainsSource(101));
  EXPECT_TRUE(interval.ContainsSource(102));
  EXPECT_FALSE(interval.ContainsSource(103));
}

TEST(DayFiveTest, MapSouceIndexWorks) {
  MappingInterval interval{.source_range_start = 100,
                           .destination_range_start = 200,
                           .range_length = 3};
  // Outside of scope values map 1:1.
  EXPECT_EQ(interval.Map(99), 99);
  // Inside of cope values map correctly.
  EXPECT_EQ(interval.Map(100), 200);
  EXPECT_EQ(interval.Map(101), 201);
  EXPECT_EQ(interval.Map(102), 202);
  // Outside of scope values map 1:1.
  EXPECT_EQ(interval.Map(103), 103);
}

TEST(DayFiveTest, ParseCategoryMapperWorks) {
  CategoryMapper actual = *ParseCategoryMapper({
      "foo-to-bar map:",
      "1 2 3",
      "4 5 6",
      "7 8 9",
  });

  CategoryMapper expected =
      CategoryMapper{.source_category = "foo",
                     .destination_category = "bar",
                     .intervals = {MappingInterval{.source_range_start = 2,
                                                   .destination_range_start = 1,
                                                   .range_length = 3},
                                   MappingInterval{.source_range_start = 5,
                                                   .destination_range_start = 4,
                                                   .range_length = 6},
                                   MappingInterval{.source_range_start = 8,
                                                   .destination_range_start = 7,
                                                   .range_length = 9}}};

  EXPECT_EQ(actual, expected);
}

TEST(DayFiveTest, MapCategoriesWorks) {
  CategoryMapper mapper = *ParseCategoryMapper({
      "seed-to-soil map:",
      "50 98 2",
      "52 50 48",
  });
  // Pass thru.
  EXPECT_EQ(mapper.Map(1), 1);
  // Hits range 1.
  EXPECT_EQ(mapper.Map(99), 51);
  // Hits range 2.
  EXPECT_EQ(mapper.Map(58), 60);
}

TEST(DayFiveTest, ParseSeedsWorks) {
  EXPECT_THAT(*ParseSeedsAsIndependent("Seeds: 1 2 33 444"),
              ElementsAre(1, 2, 33, 444));
}

TEST(DayFiveTest, TraverseCategoriesWorks) {
  absl::flat_hash_map<std::string, CategoryMapper> mappers;
  mappers["seed"] = *ParseCategoryMapper({
      "seed-to-soil map:",
      "50 98 2",
      "52 50 48",
  });
  mappers["soil"] = *ParseCategoryMapper({
      "soil-to-fertilizer map:",
      "0 15 37",
      "37 52 2",
      "39 0 15",
  });

  // seed(50) -> soil(52) -> fertilizer(37).
  EXPECT_EQ(TraverseCategories(/*source_idx=*/50, /*source_category=*/"seed",
                               mappers),
            37);
}

TEST(DayFiveTest, ParseSeedRangesWorks) {
  EXPECT_THAT(*ParseSeedsAsRanges("Seeds: 1 2 33 444"),
              ElementsAre(IndexRange{.range_start = 1, .range_length = 2},
                          IndexRange{.range_start = 33, .range_length = 444}));
}

TEST(DayFiveTest, ContainsIntervalWorks) {
  MappingInterval interval{.source_range_start = 100,
                           .destination_range_start = 0,
                           .range_length = 50};
  EXPECT_TRUE(interval.ContainsSourceRange(
      IndexRange{.range_start = 125, .range_length = 5}));
  EXPECT_TRUE(interval.ContainsSourceRange(
      IndexRange{.range_start = 125, .range_length = 500}));
  EXPECT_TRUE(interval.ContainsSourceRange(
      IndexRange{.range_start = 1, .range_length = 500}));
  EXPECT_FALSE(interval.ContainsSourceRange(
      IndexRange{.range_start = 99, .range_length = 1}));
  EXPECT_FALSE(interval.ContainsSourceRange(
      IndexRange{.range_start = 150, .range_length = 1}));
  EXPECT_FALSE(interval.ContainsSourceRange(
      IndexRange{.range_start = 200, .range_length = 1}));
}

TEST(DayFiveTest, MapRangeWorks) {
  MappingInterval interval{.source_range_start = 100,
                           .destination_range_start = 2000,
                           .range_length = 50};
  // Total overlap.
  EXPECT_THAT(interval.MapRange({.range_start = 100, .range_length = 50}),
              ElementsAre(IndexRange{.range_start = 2000,
                                     .range_length = 50,
                                     .was_remapped_by_interval = true}));

  // Partial inner overlap.
  EXPECT_THAT(interval.MapRange({.range_start = 125, .range_length = 10}),
              ElementsAre(IndexRange{.range_start = 2025,
                                     .range_length = 10,
                                     .was_remapped_by_interval = true}));

  // Partial overlap with unmapped starting portion.
  EXPECT_THAT(interval.MapRange({.range_start = 50, .range_length = 75}),
              ElementsAre(
                  // This portion of the range was unmapped.
                  IndexRange{.range_start = 50, .range_length = 50},
                  // This portion of the range was mapped.
                  IndexRange{.range_start = 2000,
                             .range_length = 25,
                             .was_remapped_by_interval = true}));

  // Partial overlap with unmapped ending portion.
  EXPECT_THAT(interval.MapRange({.range_start = 125, .range_length = 75}),
              ElementsAre(
                  // This portion of the range was mapped.
                  IndexRange{.range_start = 2025,
                             .range_length = 25,
                             .was_remapped_by_interval = true},
                  // This portion of the range was unmapped.
                  IndexRange{.range_start = 150, .range_length = 50}));

  // Total overlap with unmapped portions on both ends.
  EXPECT_THAT(interval.MapRange({.range_start = 75, .range_length = 100}),
              ElementsAre(
                  // This portion of the range was unmapped.
                  IndexRange{.range_start = 75, .range_length = 25},
                  // This portion of the range was mapped.
                  IndexRange{.range_start = 2000,
                             .range_length = 50,
                             .was_remapped_by_interval = true},
                  // This portion of the range was unmapped.
                  IndexRange{.range_start = 150, .range_length = 25}));
}

TEST(DayFiveTest, CategoryMapperMapRangeWorks) {
  const auto mapper = *ParseCategoryMapper({
      "soil-to-fertilizer map:",
      "0 15 37",
      "37 52 2",
      "39 0 15",
  });

  EXPECT_THAT(
      mapper.MapRange(IndexRange{.range_start = 10, .range_length = 50}),
      ElementsAre(IndexRange{.range_start = 0, .range_length = 37},
                  IndexRange{.range_start = 37, .range_length = 2},
                  IndexRange{.range_start = 49, .range_length = 5},
                  IndexRange{.range_start = 54, .range_length = 6}));
}

TEST(DayFiveTest, RangeTraverseCategoriesWorks) {
  absl::flat_hash_map<std::string, CategoryMapper> mappers;
  mappers["seed"] = *ParseCategoryMapper({
      "seed-to-soil map:",
      "50 98 2",
      "52 50 48",
  });
  mappers["soil"] = *ParseCategoryMapper({
      "soil-to-fertilizer map:",
      "0 15 37",
      "37 52 2",
      "39 0 15",
  });

  /*
   * Seeds [25, 75) -> Soil [25-50), [52, 77)
   * -> Fertilizer [10,35), [37, 39), [54, 77)
   */

  EXPECT_THAT(
      RangeTraverseCategories(IndexRange{.range_start = 25, .range_length = 50},
                              /*source_category=*/"seed", mappers),
      ElementsAre(IndexRange{.range_start = 10, .range_length = 25},
                  IndexRange{.range_start = 37, .range_length = 2},
                  IndexRange{.range_start = 54, .range_length = 23}));
}

} // namespace
} // namespace day_5

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
  EXPECT_EQ(interval(99), 99);
  // Inside of cope values map correctly.
  EXPECT_EQ(interval(100), 200);
  EXPECT_EQ(interval(101), 201);
  EXPECT_EQ(interval(102), 202);
  // Outside of scope values map 1:1.
  EXPECT_EQ(interval(103), 103);
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
  EXPECT_EQ(mapper(1), 1);
  // Hits range 1.
  EXPECT_EQ(mapper(99), 51);
  // Hits range 2.
  EXPECT_EQ(mapper(58), 60);
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

} // namespace
} // namespace day_5

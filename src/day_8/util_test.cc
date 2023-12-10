#include "src/day_8/util.h"

#include "Eigen/Sparse"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace day_8 {
namespace {

using ::testing::ElementsAre;
using ::testing::FieldsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(DayEightTest, ParseMapWorks) {
  Map actual = *ParseMap({"AAA = (BBB, CCC)", "BBB = (CCC, DDD)"});
  EXPECT_THAT(actual,
              UnorderedElementsAre(Pair("AAA", FieldsAre("BBB", "CCC")),
                                   Pair("BBB", FieldsAre("CCC", "DDD"))));
}

TEST(DayEightTest, ParseInstructions) {
  Instructions instructions = ParseInstructions("RLLRL");
  EXPECT_THAT(instructions.directions,
              ElementsAre(Direction::RIGHT, Direction::LEFT, Direction::LEFT,
                          Direction::RIGHT, Direction::LEFT));
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(instructions.next(), Direction::RIGHT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
    EXPECT_EQ(instructions.next(), Direction::RIGHT);
    EXPECT_EQ(instructions.next(), Direction::LEFT);
  }
}

TEST(DayEightTest, MapIndexWorks) {
  Map map =
      *ParseMap({"AAA = (BBB, ZZZ)", "BBB = (ZZZ, AAA)", "ZZZ = (AAA, BBB)"});
  MapIndex actual = IndexMap(map);

  EXPECT_TRUE(actual.idx_by_name.contains("AAA"));
  EXPECT_TRUE(actual.idx_by_name.contains("BBB"));
  EXPECT_TRUE(actual.idx_by_name.contains("ZZZ"));

  EXPECT_EQ(actual.name_by_idx[actual.idx_by_name["AAA"]], "AAA");
  EXPECT_EQ(actual.name_by_idx[actual.idx_by_name["BBB"]], "BBB");
  EXPECT_EQ(actual.name_by_idx[actual.idx_by_name["ZZZ"]], "ZZZ");

  EXPECT_THAT(actual.sources, UnorderedElementsAre(actual.idx_by_name["AAA"]));
  EXPECT_THAT(actual.targets, UnorderedElementsAre(actual.idx_by_name["ZZZ"]));
}

TEST(DayEightTest, GetSourceDiagonalMatrixWorks) {
  MapIndex index;
  index.name_by_idx.push_back("AAA");
  index.name_by_idx.push_back("BBA");
  index.name_by_idx.push_back("ZZZ");
  index.sources = {0, 1};

  Eigen::SparseMatrix<bool> expected(2, 3);
  expected.insert(0, 0) = true;
  expected.insert(1, 1) = true;

  EXPECT_TRUE(GetInitialPositionMatrix(index).isApprox(expected));
}

TEST(DayEightTest, GetTargetPositionVectorWorks) {
  MapIndex index;
  index.name_by_idx.push_back("AAA");
  index.name_by_idx.push_back("BBZ");
  index.name_by_idx.push_back("ZZZ");
  index.targets = {1, 2};

  Eigen::Vector3<bool> expected;
  expected.setConstant(false);
  expected(1) = true;
  expected(2) = true;

  EXPECT_TRUE(GetTargetPositionVector(index).isApprox(expected));
}

TEST(DayEightTest, GetTransitionMatricesWorks) {

  MapIndex index;
  index.idx_by_name["A"] = 0;
  index.idx_by_name["B"] = 1;
  index.idx_by_name["C"] = 2;
  index.idx_by_name["D"] = 3;
  index.idx_by_name["E"] = 4;
  index.idx_by_name["F"] = 5;

  Eigen::SparseMatrix<bool> expected_left(6, 6);
  Eigen::SparseMatrix<bool> expected_right(6, 6);

  Map map;
  map["A"] = {"B", "C"};
  expected_left.insert(0, 1) = true;
  expected_right.insert(0, 2) = true;

  map["B"] = {"C", "D"};
  expected_left.insert(1, 2) = true;
  expected_right.insert(1, 3) = true;

  map["C"] = {"C", "C"};
  expected_left.insert(2, 2) = true;
  expected_right.insert(2, 2) = true;

  map["D"] = {"E", "F"};
  expected_left.insert(3, 4) = true;
  expected_right.insert(3, 5) = true;

  map["E"] = {"A", "A"};
  expected_left.insert(4, 0) = true;
  expected_right.insert(4, 0) = true;

  map["F"] = {"A", "B"};
  expected_left.insert(5, 0) = true;
  expected_right.insert(5, 1) = true;

  const TransitionMatrices actual = ToTransitionMatrices(map, index);
  EXPECT_TRUE(actual.left.isApprox(expected_left));
  EXPECT_TRUE(actual.right.isApprox(expected_right));
}

TEST(DayEightTest, CheckCompleteWorks) {
  Eigen::SparseMatrix<bool> current_positions(3, 6);
  Eigen::SparseVector<bool> targets(6);

  // Each row indicated by sources in current_positions needs to have at least
  // one true value in common with targets.
  targets.insert(0) = true;
  targets.insert(4) = true;

  current_positions.insert(0, 0) = true;
  current_positions.insert(1, 4) = true;
  current_positions.insert(2, 0) = true;

  EXPECT_TRUE(CheckComplete(current_positions, targets));

  current_positions.coeffRef(0, 0) = false;

  EXPECT_FALSE(CheckComplete(current_positions, targets));

  current_positions.insert(0, 3) = true;
  targets.insert(3) = true;

  EXPECT_TRUE(CheckComplete(current_positions, targets));
}

} // namespace
} // namespace day_8

#include "src/day_3/util.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <vector>

namespace day_3 {
namespace {

using ::testing::ElementsAre;

Grid GetTestGrid() {
  return Grid({
      //     0123456789
      /*0*/ "467..114..",
      /*1*/ "...*......",
      /*2*/ "..35..633.",
      /*3*/ "......#...",
      /*4*/ "617*......",
      /*5*/ ".....+.58.",
      /*6*/ "..592.....",
      /*7*/ "......755.",
      /*8*/ "...$.*....",
      /*9*/ ".664.598..",
  });
}

TEST(DayThreeTest, TestGridAtWorks) {
  const auto grid = GetTestGrid();
  EXPECT_EQ(grid.at(0, 0), '4');
  EXPECT_EQ(grid.at(0, 4), '6');
  EXPECT_EQ(grid.at(3, 1), '*');
  EXPECT_EQ(grid.at(6, 3), '#');
}

TEST(DayThreeTest, TestGridVisitAdjacentTilesWorksInCorner) {
  const auto grid = GetTestGrid();
  std::vector<char> adjacent_chars;
  const auto visit_fn = [&](const char c) { adjacent_chars.push_back(c); };
  grid.VisitAdjacentTiles(0, 0, visit_fn);
  EXPECT_THAT(adjacent_chars, ElementsAre('6', '.', '.'));
}

TEST(DayThreeTest, TestGridVisitAdjacentTilesWorksOnSide) {
  const auto grid = GetTestGrid();
  std::vector<char> adjacent_chars;
  const auto visit_fn = [&](const char c) { adjacent_chars.push_back(c); };
  grid.VisitAdjacentTiles(9, 6, visit_fn);
  EXPECT_THAT(adjacent_chars, ElementsAre('8', '.', '.', '5', '.'));
}

TEST(DayThreeTest, TestGridVisitNumbers) {
  const auto grid = GetTestGrid();
  std::vector<bool> adj_to_symbols;
  std::vector<int> values;
  const auto visit_fn = [&](const GridNumber &number) {
    adj_to_symbols.push_back(number.adjacent_to_symbol);
    values.push_back(number.value);
  };
  grid.VisitNumbers(visit_fn);
  EXPECT_THAT(adj_to_symbols, ElementsAre(true, false, true, true, true, false,
                                          true, true, true, true));
  EXPECT_THAT(values,
              ElementsAre(467, 114, 35, 633, 617, 58, 592, 755, 664, 598));
}

TEST(DayThreeTest, TestIsAdjToSymbol) {
  const auto grid = GetTestGrid();
  EXPECT_FALSE(grid.IsAdjacentToSymbol(0, 0));
  EXPECT_TRUE(grid.IsAdjacentToSymbol(2, 0));
  EXPECT_TRUE(grid.IsAdjacentToSymbol(5, 4));
  EXPECT_FALSE(grid.IsAdjacentToSymbol(1, 6));
}

} // namespace
} // namespace day_3

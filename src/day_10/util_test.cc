#include "src/day_10/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace day_10 {
namespace {

using ::testing::ElementsAre;
using ::testing::FieldsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(DayTenTest, CreatePipeNetworkWorks) {
  PipeNetwork pipe_network({
      /*     01234*/
      /*0*/ "..|..", // <- Ignore this pipe.
      /*1*/ ".S-7.",
      /*2*/ ".|.|.",
      /*3*/ ".L-J.",
      /*4*/ ".....",
  });

  EXPECT_THAT(pipe_network.network,
              UnorderedElementsAre(
                  Pair(Pos{1, 1}, UnorderedElementsAre(Pos{1, 2}, Pos{2, 1})),
                  Pair(Pos{1, 2}, UnorderedElementsAre(Pos{1, 1}, Pos{1, 3})),
                  Pair(Pos{1, 3}, UnorderedElementsAre(Pos{1, 2}, Pos{2, 3})),
                  Pair(Pos{2, 1}, UnorderedElementsAre(Pos{1, 1}, Pos{3, 1})),
                  Pair(Pos{2, 3}, UnorderedElementsAre(Pos{1, 3}, Pos{3, 3})),
                  Pair(Pos{3, 1}, UnorderedElementsAre(Pos{2, 1}, Pos{3, 2})),
                  Pair(Pos{3, 2}, UnorderedElementsAre(Pos{3, 1}, Pos{3, 3})),
                  Pair(Pos{3, 3}, UnorderedElementsAre(Pos{3, 2}, Pos{2, 3}))));
}

TEST(DayTenTest, TraverseNetworkWorks) {
  PipeNetwork pipe_network({
      /*     01234*/
      /*0*/ ".....",
      /*1*/ ".S-7.",
      /*2*/ ".|.|.",
      /*3*/ ".L-J.",
      /*4*/ ".....",
  });
  std::vector<Pos> visited;
  const auto visit_fn = [&](const Pos &pos) { visited.push_back(pos); };
  pipe_network.BreadthFirstTraversal(visit_fn);
  EXPECT_THAT(visited, ElementsAre(Pos{1, 1}, Pos{2, 1}, Pos{1, 2}, Pos{3, 1},
                                   Pos{1, 3}, Pos{3, 2}, Pos{2, 3}, Pos{3, 3}));
}

TEST(DayTenTest, CreatePipeSegmentMaskWorks) {
  PipeNetwork pipe_network({
      /*     01234*/
      /*0*/ ".....",
      /*1*/ ".S-7.",
      /*2*/ ".|.|.",
      /*3*/ ".L-J.",
      /*4*/ ".....",
  });

  PipeSegmentMask pipe_segment_mask(pipe_network);

  EXPECT_THAT(pipe_segment_mask.segments,
              ElementsAre(ElementsAre(25, 25, 25, 25, 25),
                          ElementsAre(25, -1, -1, -1, 25),
                          ElementsAre(25, -1, 13, -1, 25),
                          ElementsAre(25, -1, -1, -1, 25),
                          ElementsAre(25, 25, 25, 25, 25)));

  EXPECT_THAT(pipe_segment_mask.segment_graph,
              UnorderedElementsAre(Pair(-1, UnorderedElementsAre(13, 25)),
                                   Pair(25, UnorderedElementsAre(-1, -2)),
                                   Pair(13, UnorderedElementsAre(-1)),
                                   Pair(-2, UnorderedElementsAre(25))));

  EXPECT_THAT(pipe_segment_mask.segment_sizes,
              UnorderedElementsAre(Pair(-1, 8), Pair(13, 1), Pair(25, 16)));
}

} // namespace
} // namespace day_10

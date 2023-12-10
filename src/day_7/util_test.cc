#include "src/day_7/util.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest-matchers.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

namespace day_7 {
namespace {

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

TEST(DaySevenTest, ParseHandWorks) {
  EXPECT_THAT(*ParseHand("A2TQ4"),
              ElementsAre(CardValue::ACE, CardValue::TWO, CardValue::TEN,
                          CardValue::QUEEN, CardValue::FOUR));
}

TEST(DaySevenTest, GetBestCardSetWorks) {
  EXPECT_EQ(GetHandType(*ParseHand("AAAAA")), HandType::QUINTET);

  EXPECT_EQ(GetHandType(*ParseHand("KK3KK")), HandType::QUARTET);

  EXPECT_EQ(GetHandType(*ParseHand("QTQTT")), HandType::FULL_HOUSE);

  EXPECT_EQ(GetHandType(*ParseHand("98979")), HandType::TRIPLE);

  EXPECT_EQ(GetHandType(*ParseHand("65454")), HandType::TWO_PAIR);

  EXPECT_EQ(GetHandType(*ParseHand("33885")), HandType::TWO_PAIR);

  EXPECT_EQ(GetHandType(*ParseHand("24335")), HandType::PAIR);

  EXPECT_THAT(GetHandType(*ParseHand("6789T")), HandType::HIGH_CARD);
}

TEST(DaySevenTest, CardComparisonWorks) {
  EXPECT_TRUE(*ParseHand("23456") < *ParseHand("34567"));
  // First-order tie.
  EXPECT_TRUE(*ParseHand("345TT") < *ParseHand("TT234"));
  EXPECT_TRUE(*ParseHand("66666") < *ParseHand("77777"));
  EXPECT_TRUE(*ParseHand("55455") < *ParseHand("K5555"));
  // First-order tie.
  EXPECT_FALSE(*ParseHand("65432") < *ParseHand("23456"));
  EXPECT_TRUE(*ParseHand("TT998") < *ParseHand("TT99K"));
  EXPECT_TRUE(*ParseHand("TT998") < *ParseHand("TT99K"));
  EXPECT_TRUE(*ParseHand("TT998") < *ParseHand("TT999"));
  EXPECT_TRUE(*ParseHand("AAA99") < *ParseHand("AAATT"));
  EXPECT_TRUE(*ParseHand("AAA98") < *ParseHand("AAATT"));
  EXPECT_TRUE(*ParseHand("AA9T8") < *ParseHand("AATT9"));
}

TEST(DaySevenTest, ParseHandAndBidWorks) {
  HandAndBid hand_and_bid = *ParseHandAndBid("32T3K 765");
  EXPECT_THAT(hand_and_bid.hand,
              ElementsAre(CardValue::THREE, CardValue::TWO, CardValue::TEN,
                          CardValue::THREE, CardValue::KING));
  EXPECT_EQ(hand_and_bid.bid, 765);
}

TEST(DaySevenTest, SortHandAndBidWorks) {
  std::vector<HandAndBid> hands{
      *ParseHandAndBid("KK677 28"), *ParseHandAndBid("QQQJA 483"),
      *ParseHandAndBid("32T3K 765"), *ParseHandAndBid("KTJJT 220"),
      *ParseHandAndBid("T55J5 684")};
  std::sort(hands.begin(), hands.end());
  EXPECT_THAT(hands, ElementsAre(*ParseHandAndBid("32T3K 765"),
                                 *ParseHandAndBid("KTJJT 220"),
                                 *ParseHandAndBid("KK677 28"),
                                 *ParseHandAndBid("T55J5 684"),
                                 *ParseHandAndBid("QQQJA 483")));
}

TEST(DaySevenTest, ConvertToJokersWorks) {
  EXPECT_THAT(ConvertJacksToJokers(*ParseHand("J2TJ4")),
              ElementsAre(CardValue::JOKER, CardValue::TWO, CardValue::TEN,
                          CardValue::JOKER, CardValue::FOUR));
}

} // namespace
} // namespace day_7

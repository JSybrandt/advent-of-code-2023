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
  // Should sort output.
  EXPECT_THAT(*ParseHand("A2TQ4"),
              ElementsAre(CardValue::TWO, CardValue::FOUR, CardValue::TEN,
                          CardValue::QUEEN, CardValue::ACE));
}

TEST(DaySevenTest, ToOrderedCardSetsWorks) {
  EXPECT_THAT(ToOrderedCardSets(*ParseHand("AAAAA")),
              ElementsAre(CardSet{.card_values = {CardValue::ACE},
                                  .set_strength = SetStrength::QUINTET}));

  EXPECT_THAT(ToOrderedCardSets(*ParseHand("KK3KK")),
              ElementsAre(CardSet{.card_values = {CardValue::KING},
                                  .set_strength = SetStrength::QUARTET},
                          CardSet{.card_values = {CardValue::THREE},
                                  .set_strength = SetStrength::HIGH_CARD}));

  EXPECT_THAT(
      ToOrderedCardSets(*ParseHand("QTQTT")),
      ElementsAre(CardSet{.card_values = {CardValue::TEN, CardValue::QUEEN},
                          .set_strength = SetStrength::FULL_HOUSE}));

  EXPECT_THAT(ToOrderedCardSets(*ParseHand("98979")),
              ElementsAre(CardSet{.card_values = {CardValue::NINE},
                                  .set_strength = SetStrength::TRIPLE},
                          CardSet{.card_values = {CardValue::EIGHT},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::SEVEN},
                                  .set_strength = SetStrength::HIGH_CARD}));

  EXPECT_THAT(
      ToOrderedCardSets(*ParseHand("65454")),
      ElementsAre(CardSet{.card_values = {CardValue::FIVE, CardValue::FOUR},
                          .set_strength = SetStrength::TWO_PAIR},
                  CardSet{.card_values = {CardValue::SIX},
                          .set_strength = SetStrength::HIGH_CARD}));

  EXPECT_THAT(
      ToOrderedCardSets(*ParseHand("33885")),
      ElementsAre(CardSet{.card_values = {CardValue::EIGHT, CardValue::THREE},
                          .set_strength = SetStrength::TWO_PAIR},
                  CardSet{.card_values = {CardValue::FIVE},
                          .set_strength = SetStrength::HIGH_CARD}));

  EXPECT_THAT(ToOrderedCardSets(*ParseHand("24335")),
              ElementsAre(CardSet{.card_values = {CardValue::THREE},
                                  .set_strength = SetStrength::PAIR},
                          CardSet{.card_values = {CardValue::FIVE},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::FOUR},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::TWO},
                                  .set_strength = SetStrength::HIGH_CARD}));

  EXPECT_THAT(ToOrderedCardSets(*ParseHand("6789T")),
              ElementsAre(CardSet{.card_values = {CardValue::TEN},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::NINE},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::EIGHT},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::SEVEN},
                                  .set_strength = SetStrength::HIGH_CARD},
                          CardSet{.card_values = {CardValue::SIX},
                                  .set_strength = SetStrength::HIGH_CARD}));
}

TEST(DaySevenTest, CardComparisonWorks) {
  EXPECT_TRUE(*ParseHand("23456") < *ParseHand("34567"));
  EXPECT_TRUE(*ParseHand("TT234") < *ParseHand("345TT"));
  EXPECT_TRUE(*ParseHand("66666") < *ParseHand("77777"));
  EXPECT_TRUE(*ParseHand("55455") < *ParseHand("K5555"));
  EXPECT_FALSE(*ParseHand("23456") < *ParseHand("65432"));
  EXPECT_TRUE(*ParseHand("23456") == *ParseHand("65432"));
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
              ElementsAre(CardValue::TWO, CardValue::THREE, CardValue::THREE,
                          CardValue::TEN, CardValue::KING));
  EXPECT_EQ(hand_and_bid.bid, 765);
}

TEST(DaySevenTest, SortHandAndBidWorks) {
  std::vector<HandAndBid> hands{
      *ParseHandAndBid("KK677 28"), *ParseHandAndBid("QQQJA 483"),
      *ParseHandAndBid("32T3K 765"), *ParseHandAndBid("KTJJT 220"),
      *ParseHandAndBid("T55J5 684")};
  std::sort(hands.begin(), hands.end());
  EXPECT_THAT(hands, ElementsAre(*ParseHandAndBid("233TK 765"),
                                 *ParseHandAndBid("TTJJK 220"),
                                 *ParseHandAndBid("677KK 28"),
                                 *ParseHandAndBid("555TJ 684"),
                                 *ParseHandAndBid("JQQQA 483")));
}

} // namespace
} // namespace day_7

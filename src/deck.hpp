#pragma once

#include "card.hpp"
#include <array>
#include <random>
#include <vector>

class Deck {
public:
  Deck(unsigned numDecks);

  Card deal();
  void shuffle();
  [[nodiscard]] size_t size() const;
  void rebuild();

private:
  unsigned numDecks_;
  std::vector<Card> cards_;
  std::mt19937 rng_{std::random_device{}()};

  inline static constexpr std::array<Rank, 13> allRanks{
      Rank::TWO,   Rank::THREE, Rank::FOUR, Rank::FIVE, Rank::SIX,
      Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN,  Rank::JACK,
      Rank::QUEEN, Rank::KING,  Rank::ACE};

  inline static constexpr std::array<Suit, 4> allSuits{
      Suit::CLUBS, Suit::DIAMONDS, Suit::HEARTS, Suit::SPADES};
};

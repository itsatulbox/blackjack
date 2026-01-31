#include "card.hpp"

Card::Card(Rank rank, Suit suit) : rank_{rank}, suit_{suit} {}

Rank Card::rank() const noexcept { return rank_; }

Suit Card::suit() const noexcept { return suit_; }

int Card::value() const {
  switch (rank_) {
  case Rank::ACE:   return 1;
  case Rank::TWO:   return 2;
  case Rank::THREE: return 3;
  case Rank::FOUR:  return 4;
  case Rank::FIVE:  return 5;
  case Rank::SIX:   return 6;
  case Rank::SEVEN: return 7;
  case Rank::EIGHT: return 8;
  case Rank::NINE:  return 9;
  default:          return 10;
  }
}

#include "deck.hpp"
#include "card.hpp"
#include <algorithm>
#include <stdexcept>

Deck::Deck(unsigned numDecks) : numDecks_{numDecks} { rebuild(); }

void Deck::shuffle() { std::shuffle(cards_.begin(), cards_.end(), rng_); }

Card Deck::deal() {
  if (cards_.empty()) throw std::runtime_error("deck is empty");
  Card card = cards_.back();
  cards_.pop_back();
  return card;
}

size_t Deck::size() const { return cards_.size(); }

void Deck::rebuild() {
  cards_.clear();
  cards_.reserve(numDecks_ * 52);
  for (size_t i = 0; i < numDecks_; ++i) {
    for (Rank rank : allRanks) {
      for (Suit suit : allSuits) {
        cards_.emplace_back(rank, suit);
      }
    }
  }

  shuffle();
}

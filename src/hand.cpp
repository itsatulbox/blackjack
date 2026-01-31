#include "hand.hpp"
#include <cassert>
#include <vector>

const std::vector<Card> &Hand::cards() const { return cards_; }

unsigned Hand::score() const {
  unsigned score = 0;
  unsigned numAces = 0;

  for (const Card &card : cards_) {
    score += card.value();
    if (card.rank() == Rank::ACE) {
      numAces += 1;
    }
  }

  for (size_t i = 0; i < numAces; ++i) {
    if (score + 10 <= 21) {
      score += 10;
    }
  }

  return score;
}

bool Hand::busted() const { return score() > 21; }

void Hand::hit(Card card) { cards_.push_back(card); }

size_t Hand::size() const { return cards_.size(); }

void Hand::splitInto(Hand &other) {
  assert(!cards_.empty());
  other.hit(cards_.back());
  cards_.pop_back();
}

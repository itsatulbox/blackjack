#include "player.hpp"
#include "card.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

Player::Player(int64_t funds) : funds_{funds} { newHand(); }

const std::vector<Hand> &Player::hands() const { return hands_; }

const Hand &Player::getHand(size_t hand) const {
  assert(hand < hands_.size());
  return hands_[hand];
}

Hand &Player::getHand(size_t hand) {
  assert(hand < hands_.size());
  return hands_[hand];
}

void Player::hit(Card card, size_t hand) { getHand(hand).hit(std::move(card)); }

void Player::stand(size_t hand) {
  assert(hand < hands_.size());
  standing_[hand] = true;
}

bool Player::standing(size_t hand) const {
  assert(hand < hands_.size());
  return standing_[hand];
}

bool Player::hasBlackjack(size_t hand) const {
  return (getHand(hand).size() == 2) && (getHand(hand).score() == 21);
}

void Player::newHand() {
  hands_.emplace_back();
  standing_.push_back(false);
  bets_.push_back(0);
}

int64_t Player::getBet(size_t hand) const { return bets_[hand]; }

int64_t Player::getBetSum() const {
  int64_t sum = 0;
  for (int64_t bet : bets_) sum += bet;
  return sum;
}

void Player::placeBet(int64_t bet, size_t hand) {
  assert(hand < bets_.size());
  bets_[hand] = bet;
}

void Player::doubleBet(size_t hand) { bets_[hand] *= 2; }

const std::vector<int64_t> &Player::bets() const { return bets_; }

int64_t Player::getFunds() const { return funds_; }

void Player::setFunds(int64_t funds) { funds_ = funds; }

void Player::reset() {
  hands_.clear();
  bets_.clear();
  standing_.clear();
  newHand();
}

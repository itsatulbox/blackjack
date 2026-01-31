#pragma once

#include "card.hpp"
#include "hand.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

class Player {
public:
  Player(int64_t funds = 0);

  [[nodiscard]] const std::vector<Hand> &hands() const;
  [[nodiscard]] const Hand &getHand(size_t hand) const;
  Hand &getHand(size_t hand);

  [[nodiscard]] int64_t getBet(size_t hand) const;
  [[nodiscard]] int64_t getBetSum() const;
  void placeBet(int64_t bet, size_t hand);
  void doubleBet(size_t hand);
  [[nodiscard]] const std::vector<int64_t> &bets() const;

  [[nodiscard]] int64_t getFunds() const;
  void setFunds(int64_t funds);

  void hit(Card card, size_t hand);
  void stand(size_t hand);
  [[nodiscard]] bool standing(size_t hand) const;

  [[nodiscard]] bool hasBlackjack(size_t hand) const;

  void newHand();
  void reset();

private:
  std::vector<Hand> hands_;
  std::vector<bool> standing_;
  std::vector<int64_t> bets_;
  int64_t funds_ = 0;
};

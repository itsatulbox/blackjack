#pragma once

#include "hand.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

class GameOutput {
public:
  virtual ~GameOutput() = default;
  virtual void showMainMenu() = 0;
  virtual void showGameOver() = 0;
  virtual void showMenu() = 0;
  virtual void dealCards(const Hand &playerHand, const Hand &dealerHand) = 0;
  virtual void showHands(const std::vector<Hand> &playerHands,
                         const Hand &dealerHand,
                         bool dealerRevealed = false) = 0;
  virtual void playerLoses(int64_t bet) = 0;
  virtual void playerWins(int64_t bet) = 0;
  virtual void playerPushes() = 0;
  virtual void promptNumDecks() = 0;
  virtual void promptFunds() = 0;
  virtual void promptBet() = 0;
  virtual void promptMove() = 0;
  virtual void dealerHas21() = 0;
  virtual void invalidMove() = 0;
  virtual void updateFunds(int64_t funds) {}
  virtual void setActiveHand(size_t hand) {}
};

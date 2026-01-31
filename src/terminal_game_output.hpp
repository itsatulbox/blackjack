#pragma once
#include "game_output.hpp"
#include "hand.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

class TerminalGameOutput : public GameOutput {
public:
  void showMainMenu() override;
  void showGameOver() override;
  void showMenu() override;
  void invalidMove() override;
  void promptMove() override;
  void promptBet() override;
  void promptFunds() override;
  void playerPushes() override;
  void dealerHas21() override;
  void promptNumDecks() override;
  void playerLoses(int64_t bet) override;
  void playerWins(int64_t bet) override;
  void updateFunds(int64_t funds) override;
  void setActiveHand(size_t hand) override;
  void dealCards(const Hand &playerHand, const Hand &dealerHand) override;
  void showHands(const std::vector<Hand> &playerHands,
                 const Hand &dealerHand,
                 bool dealerRevealed = false) override;

private:
  void renderGameScreen();
  void showResult(const std::string &msg);

  std::vector<Hand> playerHands_;
  Hand dealerHand_;
  bool    dealerHidden_ = false;
  int64_t funds_        = 0;
  size_t  activeHand_   = std::numeric_limits<size_t>::max();
  std::string statusMsg_;
};

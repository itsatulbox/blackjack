#pragma once

#include "controller.hpp"
#include "deck.hpp"
#include "game_output.hpp"
#include "player.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

class Game {
public:
  Game(std::unique_ptr<GameOutput> output,
       std::unique_ptr<Controller> controller);
  void run();
  void init();
  void round();
  void reset();
  [[nodiscard]] bool isOver() const;

private:
  std::optional<Deck> deck_;
  unsigned numDecks_ = 0;
  Player dealer_;
  Player player_;

  bool quit_ = false;

  std::unique_ptr<GameOutput> output_;
  std::unique_ptr<Controller> controller_;

  void deal();
  bool playerTurn();
  void dealerTurn();
  void resolvePayout();

  void playerLoses(int64_t bet);
  void playerPushes();
  void playerWins(int64_t bet);

  void split(size_t hand);
  [[nodiscard]] bool canSplit(size_t hand) const;
  void doubleDown(size_t hand);
  [[nodiscard]] bool canDoubleDown(size_t hand) const;
};

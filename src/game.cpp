#include "game.hpp"
#include "deck.hpp"
#include "game_output.hpp"
#include "player.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>

static constexpr int MENU_QUIT = 2;

Game::Game(std::unique_ptr<GameOutput> output,
           std::unique_ptr<Controller> controller)
    : output_{std::move(output)}, controller_{std::move(controller)} {}

void Game::init() {
  output_->showMenu();
  do {
    output_->promptNumDecks();
    numDecks_ = controller_->getNumDecks();
  } while (numDecks_ == 0);

  deck_ = Deck(numDecks_);

  do {
    output_->promptFunds();
    player_.setFunds(controller_->getFunds());
  } while (player_.getFunds() <= 0);
  output_->updateFunds(player_.getFunds());
}

void Game::round() {
  assert(deck_.has_value());
  if (player_.getFunds() <= 0) return;

  int64_t bet;
  do {
    output_->promptBet();
    bet = controller_->getBet();
  } while (bet <= 0 || bet > player_.getFunds());

  player_.placeBet(bet, 0);
  deal();

  if (dealer_.hasBlackjack(0)) {
    output_->dealerHas21();
    if (!player_.hasBlackjack(0)) {
      playerLoses(bet);
    } else {
      playerPushes();
    }
    return;
  }

  if (!playerTurn()) return;

  dealerTurn();
  resolvePayout();
}

bool Game::playerTurn() {
  Move playerMove;
  for (size_t i = 0; i < player_.hands().size(); ++i) {
    // NOTE: hands().size() may grow during this loop if the player splits.
    // This is intentional — split hands are processed in the same pass.
    output_->setActiveHand(i);
    if (player_.hasBlackjack(i)) continue;

    do {
      output_->promptMove();
      playerMove = controller_->getMove();

      switch (playerMove) {
      case Move::HIT:
        player_.hit(deck_->deal(), i);
        output_->showHands(player_.hands(), dealer_.getHand(0));
        break;
      case Move::STAND:
        player_.stand(i);
        break;
      case Move::SPLIT:
        if (canSplit(i)) {
          split(i);
          output_->showHands(player_.hands(), dealer_.getHand(0));
        } else {
          output_->invalidMove();
        }
        break;
      case Move::DOUBLEDOWN:
        if (canDoubleDown(i)) {
          doubleDown(i);
          output_->showHands(player_.hands(), dealer_.getHand(0));
        } else {
          output_->invalidMove();
        }
        break;
      case Move::QUIT:
        quit_ = true;
        return false;
      }
    } while (!player_.standing(i) && !player_.getHand(i).busted());
  }
  return true;
}

void Game::dealerTurn() {
  size_t bustedHandCount = 0;
  for (size_t i = 0; i < player_.hands().size(); ++i) {
    if (player_.getHand(i).busted()) ++bustedHandCount;
  }

  bool playerHasNatural = player_.hasBlackjack(0) && player_.hands().size() == 1u;
  while (!playerHasNatural && bustedHandCount != player_.hands().size() &&
         dealer_.getHand(0).score() < 17) {
    dealer_.hit(deck_->deal(), 0);
  }

  output_->showHands(player_.hands(), dealer_.getHand(0), true);
}

void Game::resolvePayout() {
  int64_t winnings = 0;
  for (size_t i = 0; i < player_.hands().size(); ++i) {
    int64_t handBet = player_.getBet(i);
    const Hand &hand = player_.getHand(i);
    if (player_.hasBlackjack(i) && player_.hands().size() == 1u) {
      winnings += handBet * 3 / 2;  // 3:2 payout
    } else if (hand.busted() || (!dealer_.getHand(0).busted() &&
                                  hand.score() < dealer_.getHand(0).score())) {
      winnings -= handBet;
    } else if (dealer_.getHand(0).busted() ||
               hand.score() > dealer_.getHand(0).score()) {
      winnings += handBet;
    }
  }

  if (winnings > 0) {
    playerWins(winnings);
  } else if (winnings < 0) {
    playerLoses(-winnings);
  } else {
    playerPushes();
  }
}

void Game::deal() {
  assert(deck_.has_value());

  player_.hit(deck_->deal(), 0);
  dealer_.hit(deck_->deal(), 0);
  player_.hit(deck_->deal(), 0);
  dealer_.hit(deck_->deal(), 0);

  output_->dealCards(player_.hands()[0], dealer_.hands()[0]);
}

bool Game::canSplit(size_t hand) const {
  return (player_.getBet(hand) <= player_.getFunds() - player_.getBetSum()) &&
         (player_.getHand(hand).cards().size() == 2) &&
         (player_.getHand(hand).cards()[0].value() ==
          player_.getHand(hand).cards()[1].value());
}

void Game::split(size_t hand) {
  player_.newHand();
  player_.getHand(hand).splitInto(player_.getHand(player_.hands().size() - 1));
  player_.placeBet(player_.getBet(hand), player_.hands().size() - 1);
  player_.hit(deck_->deal(), hand);
  player_.hit(deck_->deal(), player_.hands().size() - 1);
}

bool Game::canDoubleDown(size_t hand) const {
  return (player_.getBet(hand) <= player_.getFunds() - player_.getBetSum()) &&
         (player_.getHand(hand).size() == 2);
}

void Game::doubleDown(size_t hand) {
  player_.hit(deck_->deal(), hand);
  player_.stand(hand);
  player_.doubleBet(hand);
}

void Game::playerLoses(int64_t winnings) {
  player_.setFunds(player_.getFunds() - winnings);
  output_->updateFunds(player_.getFunds());
  output_->playerLoses(winnings);
  controller_->waitForInput();
}

void Game::playerPushes() {
  output_->playerPushes();
  controller_->waitForInput();
}

void Game::playerWins(int64_t winnings) {
  player_.setFunds(player_.getFunds() + winnings);
  output_->updateFunds(player_.getFunds());
  output_->playerWins(winnings);
  controller_->waitForInput();
}

bool Game::isOver() const { return player_.getFunds() <= 0 || quit_; }

void Game::run() {
  while (true) {
    output_->showMainMenu();
    if (controller_->getMenuChoice() == MENU_QUIT) break;

    quit_ = false;
    init();
    while (!isOver()) {
      round();
      reset();
    }
    if (!quit_) {
      output_->showGameOver();
      controller_->waitForInput();
    }
  }
}

void Game::reset() {
  assert(deck_.has_value());

  player_.reset();
  dealer_.reset();

  if (deck_->size() < numDecks_ * 26u) {
    deck_->rebuild();
  }
}

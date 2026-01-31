#include "terminal_controller.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

static void discardLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int64_t TerminalController::getBet() {
  double bet;
  std::cin >> bet;
  if (std::cin.fail() || bet <= 0) {
    std::cin.clear();
    discardLine();
    return 0;
  }
  discardLine();
  return static_cast<int64_t>(std::round(bet * 100));
}

int64_t TerminalController::getFunds() {
  double funds;
  std::cin >> funds;
  if (std::cin.fail() || funds <= 0) {
    std::cin.clear();
    discardLine();
    return 0;
  }
  discardLine();
  return static_cast<int64_t>(std::round(funds * 100));
}

Move TerminalController::getMove() {
  char move;
  while (true) {
    std::cin >> move;
    discardLine();
    switch (move) {
    case 'h': return Move::HIT;
    case 's': return Move::STAND;
    case 'd': return Move::DOUBLEDOWN;
    case 'x': return Move::SPLIT;
    case 'q': return Move::QUIT;
    default:
      std::cout << "Unknown key. Enter h/s/d/x/q: ";
    }
  }
}

unsigned TerminalController::getNumDecks() {
  unsigned numDecks;
  std::cin >> numDecks;
  if (std::cin.fail()) {
    std::cin.clear();
    discardLine();
    return 0;
  }
  discardLine();
  return numDecks;
}

int TerminalController::getMenuChoice() {
  int choice;
  while (true) {
    std::cin >> choice;
    if (!std::cin.fail() && (choice == 1 || choice == 2)) {
      discardLine();
      return choice;
    }
    std::cin.clear();
    discardLine();
    std::cout << "  Enter 1 or 2: " << std::flush;
  }
}

void TerminalController::waitForInput() {
  std::cout << "\n  Press ENTER to continue..." << std::flush;
  discardLine();
}

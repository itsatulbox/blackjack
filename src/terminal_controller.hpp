#pragma once
#include "controller.hpp"

class TerminalController : public Controller {
public:
  int64_t getBet() override;
  int64_t getFunds() override;
  Move getMove() override;
  unsigned getNumDecks() override;
  int getMenuChoice() override;
  void waitForInput() override;
};

#pragma once

#include "move.hpp"
#include <cstdint>

class Controller {
public:
  virtual ~Controller() = default;
  virtual int64_t getBet() = 0;
  virtual Move getMove() = 0;
  virtual int64_t getFunds() = 0;
  virtual unsigned getNumDecks() = 0;
  virtual int getMenuChoice() = 0;
  virtual void waitForInput() = 0;
};

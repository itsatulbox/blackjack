#pragma once

#include "card.hpp"
#include <vector>

class Hand {
public:
  [[nodiscard]] const std::vector<Card> &cards() const;
  [[nodiscard]] unsigned score() const;
  [[nodiscard]] bool busted() const;
  [[nodiscard]] size_t size() const;
  void hit(Card card);
  void splitInto(Hand &other);

private:
  std::vector<Card> cards_;
};

#pragma once

enum class Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
enum class Rank : int {
  ACE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING
};

class Card {
public:
  Card(Rank rank, Suit suit);

  [[nodiscard]] int value() const;
  [[nodiscard]] Rank rank() const noexcept;
  [[nodiscard]] Suit suit() const noexcept;

private:
  Rank rank_;
  Suit suit_;
};

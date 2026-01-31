#include "terminal_game_output.hpp"
#include "card.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <string>
#include <vector>

static const char *TITLE_ART = R"ART(
                               _..._                      .---.                _..._
          .---.             .-'_..._''.                   |   |             .-'_..._''.
/|        |   |           .' .'      '.\    .             '---'           .' .'      '.\    .
||        |   |          / .'             .'|             .---.          / .'             .'|
||        |   |         . '             .'  |             |   |         . '             .'  |
||  __    |   |    __   | |            <    |             |   |    __   | |            <    |
||/'__ '. |   | .:--.'. | |             |   | ____        |   | .:--.'. | |             |   | ____
|:/`  '. '|   |/ |   \ |. '             |   | \ .'        |   |/ |   \ |. '             |   | \ .'
||     | ||   |`" __ | | \ '.          .|   |/  .         |   |`" __ | | \ '.          .|   |/  .
||\    / '|   | .'.''| |  '. `._____.-'/|    /\  \        |   | .'.''| |  '. `._____.-'/|    /\  \
|/\'..' / '---'/ /   | |_   `-.______ / |   |  \  \    __.'   '/ /   | |_   `-.______ / |   |  \  \
'  `'-'`       \ \._,\ '/            `  '    \  \  \  |      ' \ \._,\ '/            `  '    \  \  \
                `--'  `"               '------'  '---'|____.'   `--'  `"               '------'  '---'   )ART";

static void clearScreen() {
  std::cout << "\033[2J\033[H" << std::flush;
}

static void printHeader() {
  std::cout << "  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
               "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
               "\u2550\u2557\n"
            << "  \u2551    BLACKJACK     \u2551\n"
            << "  \u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
               "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
               "\u2550\u255d\n\n";
}

static std::string fmtMoney(int64_t cents) {
  return std::format("${}.{:02d}", cents / 100, cents % 100);
}

static std::string rankStr(Rank rank) {
  switch (rank) {
  case Rank::ACE:   return "A";
  case Rank::TWO:   return "2";
  case Rank::THREE: return "3";
  case Rank::FOUR:  return "4";
  case Rank::FIVE:  return "5";
  case Rank::SIX:   return "6";
  case Rank::SEVEN: return "7";
  case Rank::EIGHT: return "8";
  case Rank::NINE:  return "9";
  case Rank::TEN:   return "10";
  case Rank::JACK:  return "J";
  case Rank::QUEEN: return "Q";
  case Rank::KING:  return "K";
  }
  return "?";
}

static std::string suitStr(Suit suit) {
  switch (suit) {
  case Suit::CLUBS:    return "\u2663";
  case Suit::DIAMONDS: return "\u2666";
  case Suit::HEARTS:   return "\u2665";
  case Suit::SPADES:   return "\u2660";
  }
  return "?";
}

static std::string repeat(const std::string &s, int n) {
  std::string out;
  out.reserve(s.size() * n);
  for (int i = 0; i < n; ++i) out += s;
  return out;
}

static constexpr int INNER = 9;

static std::vector<std::string> cardArt(const Card &card) {
  std::string r = rankStr(card.rank());
  std::string s = suitStr(card.suit());

  std::string topRank = r + std::string(INNER - (int)r.size(), ' ');
  std::string botRank = std::string(INNER - (int)r.size(), ' ') + r;
  int pad = (INNER - 1) / 2;
  std::string midSuit = std::string(pad, ' ') + s + std::string(INNER - 1 - pad, ' ');

  std::string top   = repeat("\u2500", INNER);
  std::string blank = std::string(INNER, ' ');
  return {
    "\u250c" + top   + "\u2510",
    "\u2502" + topRank + "\u2502",
    "\u2502" + blank   + "\u2502",
    "\u2502" + midSuit + "\u2502",
    "\u2502" + blank   + "\u2502",
    "\u2502" + botRank + "\u2502",
    "\u2514" + top   + "\u2518",
  };
}

static std::vector<std::string> hiddenCardArt() {
  std::string top  = repeat("\u2500", INNER);
  std::string fill = repeat("\u2592", INNER);
  return {
    "\u250c" + top  + "\u2510",
    "\u2502" + fill + "\u2502",
    "\u2502" + fill + "\u2502",
    "\u2502" + fill + "\u2502",
    "\u2502" + fill + "\u2502",
    "\u2502" + fill + "\u2502",
    "\u2514" + top  + "\u2518",
  };
}

static void printCards(const std::vector<std::vector<std::string>> &cards) {
  for (int row = 0; row < 7; ++row) {
    std::cout << "  ";
    for (size_t i = 0; i < cards.size(); ++i) {
      if (i > 0) std::cout << " ";
      std::cout << cards[i][row];
    }
    std::cout << "\n";
  }
}

void TerminalGameOutput::renderGameScreen() {
  clearScreen();
  printHeader();

  if (funds_ > 0)
    std::cout << "  Funds: " << fmtMoney(funds_) << "\n\n";

  std::cout << "  DEALER  [";
  if (dealerHidden_) {
    std::cout << "?";
  } else {
    std::cout << dealerHand_.score();
    if (dealerHand_.busted())
      std::cout << "  BUST!";
  }
  std::cout << "]\n";

  std::vector<std::vector<std::string>> dcards;
  if (dealerHidden_) {
    dcards.push_back(cardArt(dealerHand_.cards()[0]));
    dcards.push_back(hiddenCardArt());
  } else {
    for (const Card &c : dealerHand_.cards())
      dcards.push_back(cardArt(c));
  }
  printCards(dcards);
  std::cout << "\n";

  for (size_t i = 0; i < playerHands_.size(); ++i) {
    const Hand &hand = playerHands_[i];
    if (playerHands_.size() > 1) {
      bool active = dealerHidden_ && i == activeHand_;
      std::cout << (active ? "> " : "  ") << "HAND " << (i + 1);
    } else {
      std::cout << "  PLAYER";
    }
    std::cout << "  [" << hand.score() << "]";
    if (hand.busted())
      std::cout << "  BUST!";
    std::cout << "\n";

    std::vector<std::vector<std::string>> pcards;
    for (const Card &c : hand.cards())
      pcards.push_back(cardArt(c));
    printCards(pcards);
    if (i + 1 < playerHands_.size())
      std::cout << "\n";
  }

  if (!statusMsg_.empty())
    std::cout << "\n" << statusMsg_ << "\n";

  std::cout << std::flush;
}

void TerminalGameOutput::showResult(const std::string &msg) {
  statusMsg_ = (statusMsg_.empty() ? "" : statusMsg_ + "\n") + msg;
  renderGameScreen();
}

void TerminalGameOutput::showMainMenu() {
  clearScreen();
  std::cout << TITLE_ART << "\n\n";
  std::cout << "  1. New Game\n";
  std::cout << "  2. Quit\n\n";
  std::cout << "  Choice: " << std::flush;
}

void TerminalGameOutput::showGameOver() {
  clearScreen();
  printHeader();
  std::cout << "  Out of funds -- game over.\n";
  std::cout << "\n  Press ENTER to return to menu..." << std::flush;
}

void TerminalGameOutput::updateFunds(int64_t funds) { funds_ = funds; }

void TerminalGameOutput::setActiveHand(size_t hand) {
  activeHand_ = hand;
  if (playerHands_.size() > 1 && dealerHidden_)
    renderGameScreen();
}

void TerminalGameOutput::showMenu() {
  clearScreen();
  printHeader();
}

void TerminalGameOutput::promptNumDecks() {
  std::cout << "  How many decks? " << std::flush;
}

void TerminalGameOutput::promptFunds() {
  std::cout << "  Starting funds: $" << std::flush;
}

void TerminalGameOutput::promptBet() {
  clearScreen();
  printHeader();
  if (funds_ > 0)
    std::cout << "  Funds: " << fmtMoney(funds_) << "\n\n";
  std::cout << "  Place your bet: $" << std::flush;
}

void TerminalGameOutput::promptMove() {
  std::cout << "\n  h=hit  s=stand  d=double  x=split  q=quit: " << std::flush;
}

void TerminalGameOutput::invalidMove() {
  renderGameScreen();
  std::cout << "\n  That move is not available.\n";
}

void TerminalGameOutput::dealCards(const Hand &playerHand,
                                   const Hand &dealerHand) {
  playerHands_  = {playerHand};
  dealerHand_   = dealerHand;
  dealerHidden_ = true;
  statusMsg_.clear();
  renderGameScreen();
}

void TerminalGameOutput::showHands(const std::vector<Hand> &playerHands,
                                   const Hand &dealerHand,
                                   bool dealerRevealed) {
  playerHands_  = playerHands;
  dealerHand_   = dealerHand;
  dealerHidden_ = !dealerRevealed;
  renderGameScreen();
}

void TerminalGameOutput::dealerHas21() {
  dealerHidden_ = false;
  statusMsg_    = "  Dealer has blackjack!";
  renderGameScreen();
}

void TerminalGameOutput::playerWins(int64_t bet) {
  showResult(std::format("  You win {}!", fmtMoney(bet)));
}

void TerminalGameOutput::playerLoses(int64_t bet) {
  showResult(std::format("  You lose {}.", fmtMoney(bet)));
}

void TerminalGameOutput::playerPushes() {
  showResult("  Push \u2014 bet returned.");
}

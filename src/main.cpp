#include "game.hpp"
#include "terminal_controller.hpp"
#include "terminal_game_output.hpp"
#include <memory>

int main() {
  Game game(std::make_unique<TerminalGameOutput>(),
            std::make_unique<TerminalController>());
  game.run();
}

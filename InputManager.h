#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "GameState/Board.h"
#include "scenes/GameScene/CommandController.h"


class InputManager {
public:
  InputManager(Board *board, const Command::BoardCoordinate &coords)
    : board(board) {
    command_controller = new CommandController(board, coords);

    key_mappings = {
      {KEY_LEFT, Board::Direction::LEFT},
      {KEY_RIGHT, Board::Direction::RIGHT},
      {KEY_UP, Board::Direction::UP},
      {KEY_DOWN, Board::Direction::DOWN}
    };
  }

  void update() {
    for (auto [key, direction] : key_mappings) {
      if (IsKeyPressed(key)) {
        command_controller->make_move(
          board->move(command_controller->get_player_position(),
          direction)
        );
        return;;
      }
    }

    if (IsKeyPressed(KEY_Z)) {
      command_controller->undo();
    } else if (IsKeyPressed(KEY_X)) {
      command_controller->redo();
    }
  }

  bool is_game_won() {
    return board->check_board();
  }

private:
  Board* board;
  CommandController* command_controller;

  std::map<int, Board::Direction> key_mappings;
};



#endif //INPUTMANAGER_H

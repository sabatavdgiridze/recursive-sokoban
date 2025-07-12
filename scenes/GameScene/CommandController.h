#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include "Command.h"
#include "../../GameState/Board.h"


class CommandController {
public:
  explicit CommandController(Board *board, Command::BoardCoordinate initial_position)
    : board(board), initial_position(initial_position) {
  }

  void make_move(Command command) {
    if (!command.is_valid()) {
      return;
    }
    // only first current_move_length is kept from moves
    moves.erase(moves.begin() + current_move_length, moves.end());

    // append command to moves, update current_move_length
    moves.push_back(command);
    current_move_length++;

    // change the board state
    update_board(command);
  }
  void undo() {
    if (current_move_length > 0) {
      current_move_length--;
      update_board(moves.at(current_move_length).undo_command());
    }
  }
  void redo() {
    if (current_move_length < moves.size()) {
      update_board(moves.at(current_move_length));
      current_move_length++;
    }
  }

  Command::BoardCoordinate get_player_position() {
    if (current_move_length == 0) {
      return initial_position;
    } else {
      return moves.at(current_move_length - 1).get_next().front();
    }
  }

private:
  Board* board = nullptr;
  Command::BoardCoordinate initial_position;

  std::vector<Command> moves {};
  int current_move_length = 0;

  void update_board(Command command) {
    board->act_movement(command);
  }
};



#endif //COMMANDCONTROLLER_H

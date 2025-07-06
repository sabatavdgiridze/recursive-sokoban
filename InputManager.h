#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Board.h"
#include "EventBus.h"


class InputManager : public Notified {
public:
  InputManager(Board *board, const std::vector<std::pair<int, int>> &coords)
    : board(board),
      coords(coords) {
  }

  void notify(std::string message) override {

  }

  void update() {
    if (IsKeyPressed(KEY_LEFT)) {
      coords = board->move(coords, Board::Direction::LEFT);
    } else if (IsKeyPressed(KEY_RIGHT)) {
      coords = board->move(coords, Board::Direction::RIGHT);
    } else if (IsKeyPressed(KEY_UP)) {
      coords = board->move(coords, Board::Direction::UP);
    } else if (IsKeyPressed(KEY_DOWN)) {
      coords = board->move(coords, Board::Direction::DOWN);
    }
  }

private:
  Board* board;
  std::vector<std::pair<int, int>> coords;
};



#endif //INPUTMANAGER_H

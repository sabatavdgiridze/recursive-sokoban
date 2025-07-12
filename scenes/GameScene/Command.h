#ifndef COMMAND_H
#define COMMAND_H

#include <algorithm>
#include <vector>


class Command {
public:
  using BoardCoordinate = std::vector<std::pair<int, int>>;
  using Position = std::vector<BoardCoordinate>;
  using Move = std::pair<Position, Position>;

  Command(const Position &previous, const Position &next)
    : previous(previous),
      next(next) {
  }

  void add_outer_board(std::pair<int, int> offset) {
    for (auto& board_coord : previous) {
      board_coord.insert(board_coord.begin(), offset);
    }
    for (auto& board_coord: next) {
      board_coord.insert(board_coord.begin(), offset);
    }
  }

  void prefix_with_sequence(std::vector<BoardCoordinate> sequence) {
    for (int i = 0; i < sequence.size(); i++) {
      BoardCoordinate coordinate = (i < sequence.size() - 1) ? sequence.at(i + 1) : previous.at(0);
      previous.insert(previous.begin(), sequence.at(i));
      next.insert(next.begin(), coordinate);
    }
  }

  Position get_previous() const {
    return previous;
  }

  Position get_next() const {
    return next;
  }

  Move get_move() const {
    return {previous, next};
  }

  bool is_valid() const {
    return previous.size() > 0;
  }

  Command undo_command() {
    auto undo_next(next);
    auto undo_previous(previous);

    std::reverse(undo_next.begin(), undo_next.end());
    std::reverse(undo_previous.begin(), undo_previous.end());

    return Command(undo_next, undo_previous);
  }
private:
  Position previous;
  Position next;
};



#endif //COMMAND_H

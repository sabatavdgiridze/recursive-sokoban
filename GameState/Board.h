#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "../Chaikin.h"
#include "../GameCamera.h"
#include "raymath.h"
#include "../scenes/GameScene/Command.h"

class Board {
public:
  enum class Type {
    EMPTY,
    OBSTACLE,
    BOX,
    PLAYER,
    BOARD,
    GOAL
  };

  class TypeChecker {
  private:
    std::pmr::unordered_set<Type> types;
  public:
    TypeChecker(std::initializer_list<Type> typeList) : types(typeList) {}

    bool contains(Type type) const {
      return types.find(type) != types.end();
    }
    bool operator()(Type type) const {
      return contains(type);
    }
  };

  static const TypeChecker movableObjects;
  static const TypeChecker transparentObjects;

  enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  Board(std::pair<char, std::vector<std::string>> board_data, Vector2 position, float length, std::map<char, Board*> &boards) : positon(position), length(length) {
    char name = board_data.first;
    std::vector<std::string> lines = board_data.second;

    n = lines.size();
    reset_board(n);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (lines[i][j] == '#') {
          put(i, j, {Type::OBSTACLE, nullptr});
        } else if (lines[i][j] == '*') {
          put(i, j, {Type::EMPTY, nullptr});
        } else if (lines[i][j] == '@') {
          put(i, j, {Type::BOX, nullptr});
        } else if (lines[i][j] == 'P') {
          put(i, j, {Type::PLAYER, nullptr});
        } else if (lines[i][j] == 'G') {
          put(i, j, {Type::EMPTY, nullptr});
          goal_layer[i][j] = true;
        } else {
          put(i, j, {Type::BOARD, boards.at(lines[i][j])});
        }
      }
    }
    boards[name] = this;
  }


  Board(int n, Vector2 position, float length) : n(n), positon(position), length(length) {
    reset_board(n);
  }

  void reset_board(int n) {
    board.resize(n);
    goal_layer.resize(n);

    for (int i = 0; i < n; i++) {
      board.at(i).resize(n);
      goal_layer.at(i).resize(n, false);
    }
  }

  void put(int row, int col, std::pair<Type, Board*> cell) {
    board.at(row).at(col) = cell;
  }

  using Edge = std::pair<std::pair<int, int>, std::pair<int, int>>;


  class NestedBorders {
  public:
    std::vector<std::vector<Vector2>> obstacle_borders;
    std::vector<NestedBorders*> nested_board_borders;
    std::vector<bool> is_filled;

    void transform_all_points(Vector2 offset, float scale) {
      for (auto& border : obstacle_borders) {
        for (auto& point : border) {
          point = offset + Vector2Scale(point, scale);
        }
      }

      for (auto& nested_border : nested_board_borders) {
        // Recursively transform nested borders
        nested_border->transform_all_points(offset, scale);
      }
    }

    std::vector<std::pair<std::vector<Vector2>, bool>> flatten_with_fill() const {
      std::vector<std::pair<std::vector<Vector2>, bool>> result;

      for (const auto& border : obstacle_borders) {
        result.emplace_back(border, false);
      }

      for (size_t i = 0; i < nested_board_borders.size(); ++i) {
        auto nested_flattened = nested_board_borders[i]->flatten_with_fill();
        for (const auto& nested_pair : nested_flattened) {
          result.emplace_back(nested_pair.first, is_filled[i] || nested_pair.second);
        }
      }

      return result;
    }
  };


  NestedBorders* get_borders() {
    NestedBorders* borders = new NestedBorders();

    std::set<Edge> edges;

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        int x = col;
        int y = n - 1 - row;
        auto cell = board.at(row).at(col);
        if (cell.first == Type::OBSTACLE) {
          std::vector<std::pair<int, int>> points;

          points.emplace_back(x, y);
          points.emplace_back(x + 1, y);
          points.emplace_back(x + 1, y + 1);
          points.emplace_back(x, y + 1);

          add_edges(points, edges);
        } else if (cell.first == Type::BOARD) {
          auto nested_borders = cell.second->get_borders();
          nested_borders->transform_all_points(Vector2{(float)x, (float)y}, 1.0 / cell.second->n);
          borders->nested_board_borders.push_back(nested_borders);
          borders->is_filled.push_back(goal_layer[row][col]);

        }
      }
    }

    int edges_n = edges.size();
    int current_n = 0;
    while (current_n < edges_n) {
      std::vector<Edge> current_border{*edges.begin()};
      while (!(current_border.back().second == current_border.front().first)) {
        for (Edge edge : edges) {
          if (edge.first == current_border.back().second) {
            current_border.push_back(edge);
            break;
          }
        }
      }
      for (Edge edge : current_border) {
        edges.erase(edge);
      }
      current_n += current_border.size();
      borders->obstacle_borders.push_back(Chaikin::subdivide(loop_points(current_border), get_right_turns(loop_points(current_border)), 4));    }
    return borders;
  }

  std::vector<std::vector<Vector2>> to_world(std::vector<std::vector<Vector2>> borders) {
    std::vector<std::vector<Vector2>> in_world;
    for (auto border : borders) {
      std::vector<Vector2> transformed;
      for (auto point : border) {
        transformed.push_back(positon + Vector2Scale(point, length / n));
      }
      in_world.push_back(transformed);
    }
    return in_world;
  }

  std::vector<std::pair<std::vector<Vector2>, bool>> draw_obstacles() {
    auto borders = get_borders();
    auto flattened = borders->flatten_with_fill();

    // Transform to world coordinates while preserving fill info
    std::vector<std::pair<std::vector<Vector2>, bool>> result;
    for (const auto& border_pair : flattened) {
      auto transformed = to_world({border_pair.first})[0];
      result.emplace_back(transformed, border_pair.second);
    }

    return result;
  }

  std::vector<std::pair<std::vector<Vector2>, bool>> draw_objects(Type type) {
    auto objects_with_flags = draw_objects_rec(type);

    // Extract just the vectors for to_world
    std::vector<std::vector<Vector2>> borders;
    for (const auto& obj : objects_with_flags) {
      borders.push_back(obj.first);
    }

    // Transform using existing to_world method
    auto transformed_borders = to_world(borders);

    // Recombine with the bool flags
    std::vector<std::pair<std::vector<Vector2>, bool>> result;
    for (size_t i = 0; i < transformed_borders.size(); ++i) {
      result.push_back({transformed_borders[i], objects_with_flags[i].second});
    }

    return result;
  }

  std::vector<Vector2> create_center_rectangle(float x, float y, float margin) {
    return {
          {x + margin, y + margin},
          {x + 1 - margin, y + margin},
          {x + 1 - margin, y + 1 - margin},
          {x + margin, y + 1 - margin}
    };
  }


  std::vector<std::pair<std::vector<Vector2>,bool>> draw_objects_rec(Type type) {
    std::vector<std::pair<std::vector<Vector2>, bool>> borders;

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        int x = col;
        int y = n - 1 - row;
        auto cell = board.at(row).at(col);
        bool is_goal_below = goal_layer[row][col];
        if (type == Type::GOAL && cell.first ==Type::EMPTY && is_goal_below) {
          std::vector<Vector2> loop = create_center_rectangle(x, y, 0.33f);
          borders.push_back({loop, true});
        } else if (cell.first == type && type != Type::GOAL ) {
          std::vector<Vector2> loop;
          loop = create_center_rectangle(x, y, 0.0f);
          borders.push_back({loop, is_goal_below});
        } else if (cell.first == Type::BOARD) {
          std::vector<std::pair<std::vector<Vector2>,bool>> inner_borders = cell.second -> draw_objects_rec(type);
          for (auto& [loop, filled] : inner_borders) {
            for (auto& point : loop) {
              point = Vector2{(float)x, (float)y} + Vector2Scale(point, 1.0 / cell.second -> n);
            }
            borders.push_back({loop, filled});
          }
        }
      }
    }
    return borders;
  }


  void draw(GameCamera* camera) {
    camera->draw(draw_objects(Type::GOAL), PURPLE);
    camera->draw(draw_obstacles(), GRAY);
    camera->draw(draw_objects(Type::BOX), GOLD);
    camera->draw(draw_objects(Type::PLAYER), MAROON);
  }

  Vector2* get_position_ref() {
    return &positon;
  }
  float* get_length() {
    return &length;
  }

  Command make_sequence_command(const std::vector<Command::BoardCoordinate>& sequence) {
    Command::Position previous;
    Command::Position next;

    int idx = 0;
    while (idx + 1 < sequence.size()) {
      previous.push_back(sequence.at(idx));
      next.push_back(sequence.at(idx + 1));

      idx++;
    }
    return Command(previous, next);
  }

  void act_movement(Command command) {
    if (!command.is_valid()) {
      return;
    }

    auto [previous, next] = command.get_move();
    int n = previous.size();
    for (int i = n - 1; i >= 0; i--) {
      auto b_next = immediate_board(next.at(i));
      auto b = immediate_board(previous.at(i));

      auto [row_next, col_next] = next.at(i).back();
      auto [row, col] = previous.at(i).back();

      b_next->board[row_next][col_next] = b->board[row][col];
      b->board[row][col] = {Type::EMPTY, nullptr};
    }
  }

  Command move(std::vector<std::pair<int, int>> coords, Direction dir) {
    std::vector<std::vector<std::pair<int, int>>> sequence {coords};
    while (!(next(sequence.back(), dir).second == Type::OBSTACLE || transparentObjects(next(sequence.back(), dir).second))) {
      sequence.push_back(next(sequence.back(), dir).first);
    }
    Type type = next(sequence.back(), dir).second;
    if (type == Type::OBSTACLE) {
      // here we try to squizze, for this we go through
      for (int i = 1; i < sequence.size(); i++) {
        auto cell = at(sequence.at(i));
        if (cell.first == Type::BOARD) {
          auto pos = cell.second->get_entry(dir);
          auto neighbour = cell.second->board[pos.first][pos.second];
          if (transparentObjects(neighbour.first)) {
            std::vector<std::vector<std::pair<int, int>>> sub_sequence(sequence.begin(), sequence.begin() + i + 1);
            sub_sequence.at(i).push_back(pos);
            return make_sequence_command(sub_sequence);
          }
          if (movableObjects(neighbour.first)) {
            auto displaced_neighbour_command = cell.second->move({pos}, dir);
            if (displaced_neighbour_command.is_valid()) {
              displaced_neighbour_command.add_outer_board(sequence.at(i).front());
              // the displacement happened in the inner board
              std::vector<std::vector<std::pair<int, int>>> sub_sequence(sequence.begin(), sequence.begin() + i);
              displaced_neighbour_command.prefix_with_sequence(sub_sequence);

              return displaced_neighbour_command;
            }
          }
        }
      }
      return Command({}, {});
    } else {
      sequence.push_back(next(sequence.back(), dir).first);
      return make_sequence_command(sequence);
    }
  }

  std::vector<std::pair<int,int>> find_player_coords_rec() const {
    for (int r = 0; r < n; r++) {
      for (int c = 0; c < n; c++) {
        auto cell = board[r][c];
        if (cell.first == Type::PLAYER) {
          return {{r, c}};
        }
        if (cell.first == Type::BOARD) {
          auto sub = cell.second->find_player_coords_rec();
          if (!sub.empty()) {
            sub.insert(sub.begin(), {r, c});
            return sub;
          }
        }
      }
    }
    return {};
  }

  void set_position(Vector2 position, float length) {
    this->positon = position;
    this->length = length;
  }

private:
  int n;
  Vector2 positon;
  float length;

  std::vector<std::vector<std::pair<Type, Board*>>> board;
  std::vector<std::vector<bool>> goal_layer;

  void add_edges(std::vector<std::pair<int, int>> points, std::set<Edge> &edges) {
    for (int i = 0; i < points.size(); i++) {
      Edge edge = {points.at(i), points.at((i+1) % points.size())};
      Edge reverse_edge = {edge.second, edge.first};
      if (edges.contains(reverse_edge)) {
        edges.erase(reverse_edge);
      } else {
        edges.insert(edge);
      }
    }
  }
  std::vector<std::pair<int, int>> loop_points(std::vector<Edge> edges) {
    std::vector<std::pair<int, int>> points;
    for (Edge edge : edges) {
      points.push_back(edge.second);
    }
    return points;
  }

  std::vector<int> get_right_turns(std::vector<std::pair<int, int>> points) {
    std::vector<int> right_turns;
    for (int i = 0; i < points.size(); i++) {
      int before = (i - 1 + points.size()) % points.size();
      int after = (i + 1) % points.size();

      std::pair<int, int> in {points.at(i).first - points.at(before).first, points.at(i).second - points.at(before).second};
      std::pair<int, int> out {points.at(after).first - points.at(i).first, points.at(after).second - points.at(i).second};

      if (out.first * in.second - in.first * out.second > 0) {
        right_turns.push_back(i);
      }

    }
    return right_turns;
  }

  std::pair<Type, Board*> at(std::vector<std::pair<int, int>> coords) {
    Board* b = immediate_board(coords);
    return b->board.at(coords.back().first).at(coords.back().second);
  }

  Board* immediate_board(std::vector<std::pair<int, int>> coords) {
    int n = coords.size();

    Board* current = this;
    for (int i = 0; i < n - 1; i++) {
      auto idxs = coords.at(i);
      current = current->board.at(idxs.first).at(idxs.second).second;
    }
    return current;
  }

  std::pair<int, int> displace(std::pair<int, int> current, Direction dir) {
    auto row = current.first;
    auto col = current.second;

    if (dir == Direction::UP) {
      return {row - 1, col};
    }

    if (dir == Direction::DOWN) {
      return {row + 1, col};
    }

    if (dir == Direction::LEFT) {
      return {row, col - 1};
    }

    if (dir == Direction::RIGHT) {
      return {row, col + 1};
    }
  }

  bool inside(std::pair<int, int> current) {
    auto row = current.first;
    auto col = current.second;

    if (!((row >= 0 && row <= n - 1) && (col >= 0 && col <= n - 1))) {
      return false;
    }
    return true;
  }

  std::pair<std::vector<std::pair<int, int>>, Type> next(std::vector<std::pair<int, int>> coords, Direction dir) {
    auto b = immediate_board(coords);
    auto displaced = displace(coords.back(), dir);

    std::vector<std::pair<int, int>> res(coords.begin(), coords.end() - 1);
    if (!b->inside(displaced)) {
      if (!res.empty()) {
        return next(res, dir);
      }
      return {{}, Type::OBSTACLE};
    }

    if (b->at({displaced}).first == Type::OBSTACLE) {
      return {{}, Type::OBSTACLE};
    }
    res.push_back(displaced);
    if (b->at({displaced}).first == Type::EMPTY) {
      return {res, Type::EMPTY};
    }
    return {res, b->at({displaced}).first};
  }

  std::pair<int, int> get_entry(Direction dir) {
    int mid = (n - 1) / 2;

    if (dir == Direction::DOWN) {
      return {0, mid};
    } else if (dir == Direction::UP) {
      return {n - 1, mid};
    } else if (dir == Direction::RIGHT) {
      return {mid, 0};
    } else if (dir == Direction::LEFT) {
      return {mid, n-1};
    }
  }

};







#endif //BOARD_H

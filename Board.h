#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "Chaikin.h"
#include "GameCamera.h"
#include "raymath.h"

class Board {
public:
  enum class Type {
    EMPTY,
    OBSTACLE,
    BOX,
    PLAYER,
    BOARD
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
    for (int i = 0; i < n; i++) {
      board.at(i).resize(n);
    }
  }

  void put(int row, int col, std::pair<Type, Board*> cell) {
    board.at(row).at(col) = cell;
  }

  using Edge = std::pair<std::pair<int, int>, std::pair<int, int>>;

  std::vector<std::vector<Vector2>> get_borders() {
    std::vector<std::vector<Vector2>> borders;

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
          std::vector<std::vector<Vector2>> res = cell.second -> get_borders();
          for (auto& loop : res) {
            for (auto& point : loop) {
              point = Vector2{(float)x, (float)y} + Vector2Scale(point, 1.0 / cell.second -> n);
            }
            borders.push_back(loop);
          }

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
      borders.push_back(Chaikin::subdivide(loop_points(current_border), get_right_turns(loop_points(current_border)), 4));
    }
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

  std::vector<std::vector<Vector2>> draw_obstacles() {
    auto borders = get_borders();
    return to_world(borders);
  }

  std::vector<std::vector<Vector2>> draw_objects(Type type) {
    return to_world(draw_objects_rec(type));
  }

  std::vector<std::vector<Vector2>> draw_objects_rec(Type type) {
    std::vector<std::vector<Vector2>> borders;

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        int x = col;
        int y = n - 1 - row;
        auto cell = board.at(row).at(col);
        if (cell.first == type) {
          std::vector<Vector2> loop;

          loop.emplace_back(x, y);
          loop.emplace_back(x + 1, y);
          loop.emplace_back(x + 1, y + 1);
          loop.emplace_back(x, y + 1);

          borders.push_back(loop);
        } else if (cell.first == Type::BOARD) {
          std::vector<std::vector<Vector2>> inner_borders = cell.second -> draw_objects_rec(type);
          for (auto& loop : inner_borders) {
            for (auto& point : loop) {
              point = Vector2{(float)x, (float)y} + Vector2Scale(point, 1.0 / cell.second -> n);
            }
            borders.push_back(loop);
          }
        }
      }
    }
    return borders;
  }

  void draw_polygons(GameCamera* camera, std::vector<std::vector<Vector2>> borders, Color color) {
    for (auto border : borders) {
      for (int i = 0; i < border.size(); i++) {
        auto f = camera->transform(border.at(i));
        auto s = camera->transform(border.at((i + 1) % border.size()));
        DrawLineEx(f, s, 2.0f, color);

      }
    }
  }

  void draw(GameCamera* camera) {
    draw_polygons(camera, draw_obstacles(), GRAY);
    draw_polygons(camera, draw_objects(Type::BOX), GOLD);
    draw_polygons(camera, draw_objects(Type::PLAYER), MAROON);
  }

  Vector2* get_position_ref() {
    return &positon;
  }
  float* get_length() {
    return &length;
  }

  std::vector<std::pair<int, int>> act_movement(Direction dir, std::vector<std::vector<std::pair<int, int>>> sequence) {
    std::vector<std::pair<int, int>> res;
    for (int i = sequence.size() - 2; i >= 0; i--) {
      auto next_position = sequence.at(i+1);
      if (i == 0) {
        res = next_position;
      }
      auto [row_next, col_next] = next_position.back();

      auto [row, col] = sequence.at(i).back();

      auto b_next = immediate_board(next_position);
      auto b = immediate_board(sequence.at(i));

      b_next->board[row_next][col_next] = b->board[row][col];
      b->board[row][col] = {Type::EMPTY, nullptr};
    }
    return res;
  }

  std::vector<std::pair<int, int>> move(std::vector<std::pair<int, int>> coords, Direction dir) {
    std::vector<std::vector<std::pair<int, int>>> sequence {coords};
    while (!(next(sequence.back(), dir).second == Type::OBSTACLE || next(sequence.back(), dir).second == Type::EMPTY)) {
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
          if (neighbour.first == Type::EMPTY) {
            std::vector<std::vector<std::pair<int, int>>> sub_sequence(sequence.begin(), sequence.begin() + i + 1);
            sub_sequence.at(i).push_back(pos);
            return act_movement(dir, sub_sequence);
          }
          if (movableObjects(neighbour.first)) {
            auto displaced_neighbour = cell.second->move({pos}, dir);
            if (displaced_neighbour.front() != pos) {
              // the displacement happened in the inner board
              std::vector<std::vector<std::pair<int, int>>> sub_sequence(sequence.begin(), sequence.begin() + i + 1);
              sub_sequence.at(i).push_back(pos);
              return act_movement(dir, sub_sequence);
            }
          }
        }
      }
      return coords;
    } else {
      sequence.push_back(next(sequence.back(), dir).first);
      return act_movement(dir, sequence);
    }
  }

private:
  int n;
  Vector2 positon;
  float length;

  std::vector<std::vector<std::pair<Type, Board*>>> board;

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
    // assuming that n is odd
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

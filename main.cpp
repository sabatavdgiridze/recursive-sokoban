#include <fstream>

#include "Animation.h"
#include "Board.h"
#include "InputManager.h"
#include "raylib.h"

std::string readFile(std::string path) {
  std::ifstream in {path};
  std::string text {std::istreambuf_iterator<char>(in), {}};
  return text;
}

std::vector<std::pair<char, std::vector<std::string>>> parse_boards(std::string input) {
  std::istringstream iss(input);
  std::string line;

  std::vector<std::pair<char, std::vector<std::string>>> boards;
  while (std::getline(iss, line)) {
    if (line.empty()) {
      continue;
    }
    if (line.size() == 1) {
      char name = line[0];
      std::vector<std::string> rows;
      while (std::getline(iss, line) && !line.empty()) {
        rows.push_back(line);
      }
      boards.emplace_back(name, rows);
    }
  }
  std::reverse(boards.begin(), boards.end());
  return boards;
}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  auto boards = parse_boards(readFile("../board.txt"));
  Board* board = nullptr;
  std::map<char, Board* > boards_map;
  for (auto board_rep : boards) {
    board = new Board(board_rep, Vector2{0, 0}, 600, boards_map);
  }

  Animation<Vector2> animation_1(Vector2{0, 0}, Vector2{0, 100}, board->get_position_ref(), 2, new EaseOut());
  Animation<float> animation_2(600, 400, board->get_length(), 2, new EaseOut());

  std::vector<std::pair<int, int>> initial_coords {{4, 2}};
  InputManager input_manager(board, initial_coords);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    animation_1.update(GetFrameTime());
    animation_2.update(GetFrameTime());
    input_manager.update();
    board->draw(Vector2{300, 300}, Vector2{400, 400}, Vector2{1, 0}, Vector2{0, -1});
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
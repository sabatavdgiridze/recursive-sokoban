#include <fstream>

#include "Animation.h"
#include "Board.h"
#include "GameDataReader.h"
#include "InputManager.h"
#include "raylib.h"


std::vector<Board*> create_boards(std::string file_path) {

}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  auto* board = GameDataReader::create_boards("../boards").at(1);
  Animation<Vector2> animation_1(Vector2{0, 0}, Vector2{0, 100}, board->get_position_ref(), 2, new EaseOut());
  Animation<float> animation_2(600, 400, board->get_length(), 2, new EaseOut());

  std::vector<std::pair<int, int>> initial_coords {{6, 0}};
  InputManager input_manager(board, initial_coords);
  auto* camera = new GameCamera(
    Vector2{300, 300},
    Vector2{400, 400},
    Vector2{1, 0},
    Vector2{0, -1}
  );

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    animation_1.update(GetFrameTime());
    animation_2.update(GetFrameTime());
    input_manager.update();
    board->draw(camera);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
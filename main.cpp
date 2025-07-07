#include <fstream>

#include "Board.h"
#include "GameDataReader.h"
#include "raylib.h"
#include "Scene.h"


std::vector<Board*> create_boards(std::string file_path) {

}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  std::vector<Board*> boards = GameDataReader::create_boards("../boards");
  Scene scene(boards);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    scene.update(GetFrameTime());
    scene.draw();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
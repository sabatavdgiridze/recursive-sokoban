#include <fstream>

#include "Board.h"
#include "GameDataReader.h"
#include "LevelScene.h"
#include "raylib.h"
#include "Scene.h"
#include "SceneManager.h"


std::vector<Board*> create_boards(std::string file_path) {

}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  std::vector<Board*> boards = GameDataReader::create_boards("../boards");
  SceneManager scene_manager(new LevelScene(boards));

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    scene_manager.update(GetFrameTime());
    scene_manager.draw();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
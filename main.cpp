#include <fstream>

#include "GameState/Board.h"
#include "GameDataReader.h"
#include "scenes/LevelScene.h"
#include "raylib.h"
#include "scenes/InitialScene.h"
#include "scenes/Scene.h"
#include "scenes/SceneManager.h"


std::vector<Board*> create_boards(std::string file_path) {

}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  std::vector<Board*> boards = GameDataReader::create_boards("../boards");

  SceneManager scene_manager;
  // scene_manager.scenes.push(new LevelScene(boards));
  scene_manager.scenes.push(new InitialScene());
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
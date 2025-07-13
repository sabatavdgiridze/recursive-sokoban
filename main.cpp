#include <fstream>

#include "GameState/Board.h"
#include "GameDataReader.h"
#include "scenes/LevelScene.h"
#include "raylib.h"
#include "fonts/FontLoader.h"
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
  FontLoader* font_loader = new FontLoader();
  font_loader->load("../fonts/font_data.json");

  scene_manager.scenes.push(new InitialScene(font_loader));
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
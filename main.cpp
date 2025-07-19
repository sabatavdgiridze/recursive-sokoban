#include <fstream>

#include "raylib.h"
#include "fonts/FontLoader.h"
#include "scenes/InitialScene.h"
#include "scenes/Scene.h"
#include "scenes/SceneManager.h"



int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  SceneManager scene_manager;
  FontLoader* font_loader = new FontLoader();
  font_loader->load("../fonts/font_data.json");

  scene_manager.scenes.push(new InitialScene(font_loader));

  SetExitKey(KEY_NULL);
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
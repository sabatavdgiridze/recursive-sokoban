#include <fstream>

#include "raylib.h"
#include "fonts/FontLoader.h"
#include "music/SoundtrackPlayer.h"
#include "scenes/InitialScene.h"
#include "scenes/abstract_scene/Scene.h"
#include "scenes/SceneManager.h"



int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "raylib example - basic window");
  SetTargetFPS(60);

  SoundtrackPlayer soundtrack;

  soundtrack.load("../music/soundtrack.ogg");
  soundtrack.play();

  SceneManager scene_manager;
  FontLoader* font_loader = new FontLoader();
  font_loader->load("../fonts/font_data.json");

  scene_manager.scenes.push(new InitialScene(font_loader));

  SetExitKey(KEY_NULL);
  while (!WindowShouldClose())
  {
    soundtrack.update();
    scene_manager.update(GetFrameTime());

    BeginDrawing();
      ClearBackground(RAYWHITE);
      scene_manager.draw();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
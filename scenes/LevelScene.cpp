#include "LevelScene.h"
#include "InitialScene.h"

Operation* LevelScene::update(float d_t) {
  animation_manager->update(d_t);
  if (is_active) {
    if (IsKeyPressed(KEY_LEFT)) {
      move(-1);
    } else if (IsKeyPressed(KEY_RIGHT)) {
      move(1);
    } else if (IsKeyPressed(KEY_ENTER)) {
      return new PopAllOperation(new GameScene(boards.at(curr_idx)));
    } else if (IsKeyPressed(KEY_ESCAPE)) {
      return new PopAllOperation(new InitialScene(FontLoader::getInstance()));
    }
  }

  return nullptr;
}
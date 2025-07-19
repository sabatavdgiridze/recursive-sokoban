#include "GameScene.h"

#include "../InitialScene.h"
#include "../../GameDataReader.h"

Operation* GameScene::update(float d_t) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    FontLoader* font_loader = new FontLoader();
    font_loader->load("../fonts/font_data.json");
    return new PopAllOperation(new InitialScene(font_loader));
  }

  if (input_manager) {
    input_manager->update();
  }
  return new NoOperation();
}
#include "SettingsScene.h"

Operation* SettingsScene::update(float d_t) {
  slider->update(d_t, camera);
  return new NoOperation();
}

void SettingsScene::draw() {
  slider->draw(camera);
}

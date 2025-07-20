#include "GameModalScene.h"

void EnteringState::enter(GameModalScene *scene) {
  scene->animation_manager->push_animation(
    new Animation<float>(
      scene->modal.rectangle.rect.y,
      scene->modal.rectangle.rect.y - 600,
      scene->modal.rectangle.get_y(),
      1,
      new EaseOut(),
      nullptr
    )
  );
}

void EnteringState::exit(GameModalScene *scene) {

}

Operation *EnteringState::update(GameModalScene *scene, float d_t) {
  scene->animation_manager->update(d_t);
  return new NoOperation();
}

void EnteringState::draw(GameModalScene *scene) {
  scene->modal.draw(scene->camera);
}

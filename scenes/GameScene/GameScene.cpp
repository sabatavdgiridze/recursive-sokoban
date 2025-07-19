#include "GameScene.h"

#include "../InitialScene.h"
#include "../../GameDataReader.h"

void PlayingState::enter(GameScene* scene) {
  scene->animation_manager->push_animation(
  new Animation<Vector2>(
    Vector2{-100, 0},
    Vector2{0, 0},
    scene->board->get_position_ref(),
    1,
    new EaseOut(),
    nullptr
  )
);
}

void PlayingState::exit(GameScene* scene) {

}

Operation *PlayingState::update(GameScene* scene, float d_t) {
  if (scene->animation_manager) {
    scene->animation_manager->update(d_t);
  }

  if (scene->animation_manager->get_size() == 0) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      scene->changeState(new PauseState());
    }

    if (scene->input_manager) {
      scene->input_manager->update();
    }
  }
  return new NoOperation();
}

void PlayingState::draw(GameScene* scene) {
  scene->board->draw(scene->camera);
}

void PauseState::enter(GameScene *scene) {
  scene->animation_manager->push_animation(
    new Animation<Vector2>(
      Vector2{0, 0},
      Vector2{-100, 0},
      scene->board->get_position_ref(),
      1,
      new EaseOut(),
      nullptr
    )
  );
}

void PauseState::exit(GameScene *scene) {

}

Operation * PauseState::update(GameScene *scene, float d_t) {
  if (scene->animation_manager) {
    scene->animation_manager->update(d_t);
  }

  if (scene->animation_manager->get_size() == 0) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      scene->changeState(new PlayingState());
    }
  }
  return new NoOperation();
}

void PauseState::draw(GameScene *scene) {
  scene->board->draw(scene->camera);
}

#include "GameScene.h"

#include "../GameModalScene.h"
#include "../InitialScene.h"
#include "../../GameDataReader.h"

void PlayingState::enter(GameScene* scene) {
  scene->animation_manager->push_animation(
    new Animation<Vector2>(
      Vector2{-400, 0},
      Vector2{0, 0},
      scene->board->get_position_ref(),
      1,
      new EaseOut(),
      nullptr
    )
  );

  scene->animation_manager->push_animation(
    new Animation<float>(
      2,
      1,
      &scene->camera->ratio,
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
      if (scene->input_manager->is_game_won()) {
        return new PushOperation(new GameModalScene());
      }
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
      Vector2{-400, 0},
      scene->board->get_position_ref(),
      1,
      new EaseOut(),
      nullptr
    )
  );

  scene->animation_manager->push_animation(
    new Animation<float>(
      1,
      2,
      &scene->camera->ratio,
      1,
      new EaseOut(),
      nullptr
    )
  );

  for (UIButton* button : scene->buttons) {
    scene->animation_manager->push_animation(
      new Animation<float>(
        700,
        350,
        button->ui.get_x(),
        0.5,
        new EaseOut(),
        nullptr
      )
    );
  }
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
    for (UIButton* button : scene->buttons) {
      button->update(d_t, scene->camera);
    }
  }
  return new NoOperation();
}

void PauseState::draw(GameScene *scene) {
  scene->board->draw(scene->camera);
  for (UIButton* button : scene->buttons) {
    button->draw(scene->camera);
  }
}

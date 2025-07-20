#ifndef GAMEMODALSCENE_H
#define GAMEMODALSCENE_H

#include "abstract_scene/FSMScene.h"
#include "../../UI.h"
#include "../ui_components/Modal.h"
class GameModalScene;

class EnteringState : public SceneState<GameModalScene> {
public:
  EnteringState(GameModalScene *scene) {
    EnteringState::enter(scene);
  }
  void enter(GameModalScene *scene) override;

  void exit(GameModalScene *scene) override;

  Operation * update(GameModalScene *scene, float d_t) override;

  void draw(GameModalScene *scene) override;
};

class ChooseState : public SceneState<GameModalScene> {
public:
  void enter(GameModalScene *scene) override;

  void exit(GameModalScene *scene) override;

  Operation * update(GameModalScene *scene, float d_t) override;

  void draw(GameModalScene *scene) override;
};

class GameModalScene : public FSMScene<GameModalScene> {
public:
  GameCamera* camera = nullptr;
  AnimationManager* animation_manager = new AnimationManager(nullptr);
  Modal modal;

  GameModalScene() {
    camera = new GameCamera();
    camera->origin = {250, 250};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
    camera->ratio = 1;

    UI screen({0, 0, 500, 1000});
    modal = Modal(screen.grid({1, 4, 1}, {3, 1}).at(1, 1));

    current_state = new EnteringState(this);
  }
};



#endif //GAMEMODALSCENE_H

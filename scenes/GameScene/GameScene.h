#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../abstract_scene/FSMScene.h"
#include "../abstract_scene/Scene.h"

class GameScene;

class PlayingState : public SceneState<GameScene> {
public:
  void enter(GameScene* scene) override;

  void exit(GameScene* scene) override;

  Operation * update(GameScene* scene, float d_t) override;

  void draw(GameScene* scene) override;
};


class PauseState : public SceneState<GameScene> {
public:
  void enter(GameScene* scene) override;

  void exit(GameScene* scene) override;

  Operation * update(GameScene* scene, float d_t) override;

  void draw(GameScene* scene) override;
private:

};


class GameScene : public FSMScene<GameScene> {
public:
  Board* board;
  GameCamera* camera = nullptr;
  InputManager* input_manager = nullptr;
  AnimationManager* animation_manager = new AnimationManager(nullptr);

  explicit GameScene(Board* board) : board(board) {
    board->set_position({0, 0}, 500);
    input_manager = new InputManager(board, board->find_player_coords_rec());

    camera = new GameCamera();
    camera->origin = {250, 250};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
    camera->ratio = 1;

    current_state = new PlayingState();
  }

};



#endif //GAMESCENE_H

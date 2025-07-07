#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"


class GameScene : public AbstractScene {
  Board* board;
  GameCamera* camera = nullptr;

public:
  explicit GameScene(Board* board) : board(board) {
    board->set_position({0, 0}, 500);
    camera = new GameCamera();
    camera->origin = {250, 250};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
  }

  AbstractScene * update(float d_t) override {
    return nullptr;
  }

  void draw() override {
    board->draw(camera);
  };
};


#endif //GAMESCENE_H

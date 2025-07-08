#ifndef INITIALSCENE_H
#define INITIALSCENE_H
#include "Scene.h"
#include "../UI.h"


class InitialScene : public AbstractScene {
public:
  InitialScene() {
    camera = new GameCamera();
    camera->origin = {400, 400};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
  }

  Operation* update(float d_t) override {
    return new NoOperation();
  };
  void draw() override {
    UI screen({0, 0, 800, 800});
    UI row =  screen.pad(50).row({1, 3, 1});

    auto a = row.at(0, 0);
    auto b = row.at(1, 0);
    auto c = row.at(2, 0);

  }
private:
  GameCamera* camera = nullptr;
};



#endif //INITIALSCENE_H

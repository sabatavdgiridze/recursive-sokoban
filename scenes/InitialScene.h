#ifndef INITIALSCENE_H
#define INITIALSCENE_H
#include "Scene.h"
#include "../UI.h"
#include "../Buttons/UIButton.h"


class InitialScene : public AbstractScene {
public:
  InitialScene() {
    camera = new GameCamera();
    camera->origin = {400, 400};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};

    animation_manager->push_animation(
      new Animation<float>(
        1,
        2,
        &camera->ratio,
        1,
        new EaseOut(),
        nullptr
      )
    );

    UI screen({0, 0, 800, 800});
    UI col = screen.pad(50).col({3, 1, 3});

    auto bottom = col.at(0, 0);
    auto top = col.at(0, 2);

    buttons.push_back(new UIButton(bottom));
    buttons.push_back(new UIButton(top));
  }

  Operation* update(float d_t) override {
    animation_manager->update(d_t);

    for (UIButton* button : buttons) {
      button->update(d_t);
    }

    return new NoOperation();
  };
  void draw() override {

    for (UIButton* button : buttons) {
      button->draw(camera);
    }

  }
private:
  GameCamera* camera = nullptr;
  std::vector<UIButton*> buttons;
  AnimationManager* animation_manager = new AnimationManager(nullptr);
};



#endif //INITIALSCENE_H

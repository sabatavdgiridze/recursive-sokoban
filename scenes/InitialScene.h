#ifndef INITIALSCENE_H
#define INITIALSCENE_H

#include "LevelScene.h"
#include "SettingsScene.h"
#include "../GameDataReader.h"
#include "abstract_scene/Scene.h"
#include "../UI.h"
#include "../Buttons/UIButton.h"
#include "../fonts/FontLoader.h"


class InitialScene : public AbstractScene {
public:
  InitialScene(FontLoader* font_loader) {
    camera = new GameCamera();
    camera->origin = {400, 400};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};

    animation_manager->push_animation(
      new Animation<float>(
        1,
        1.5,
        &camera->ratio,
        1,
        new EaseOut(),
        nullptr
      )
    );

    UI screen({0, 0, 1000, 800});
    UI col = screen.pad(50).col({3, 2, 3});

    auto bottom = col.at(0, 0);
    auto top = col.at(0, 2);

    buttons.push_back(new UIButton(top, "LEVELS", font_loader, []() -> Operation* {
      return new PopAllOperation(new LevelScene(GameDataReader::create_boards("../boards")));
    }));
    buttons.push_back(new UIButton(bottom, "SETTINGS", font_loader, []() -> Operation* {
      return new PopAllOperation(new SettingsScene());
    }));
  }

  Operation* update(float d_t) override {
    animation_manager->update(d_t);

    for (UIButton* button : buttons) {
      Operation* operation_ptr = button->update(d_t, camera);
      if (operation_ptr != nullptr) {
        return operation_ptr;
      }
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

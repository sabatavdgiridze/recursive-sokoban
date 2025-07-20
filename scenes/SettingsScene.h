#ifndef SETTINGSSCENE_H
#define SETTINGSSCENE_H

#include "../music/SoundtrackPlayer.h"
#include "../ui_components/Slider.h"
#include "abstract_scene/Scene.h"

class SettingsScene : public AbstractScene {
public:
  Operation* update(float d_t) override;
  void draw() override;

  SettingsScene() {
    slider = new Slider(Vector2{20.0f, 400.0f}, 10.0f, 50.0f, 10, [](int index) {
      std::cout<<index;
      SoundtrackPlayer::getInstance()->setVolume(index / 10.0f);
    });

    camera = new GameCamera();
    camera->origin = {400, 400};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
    camera->ratio = 1;
  }

private:
  GameCamera* camera = nullptr;
  Slider* slider;
};



#endif //SETTINGSSCENE_H

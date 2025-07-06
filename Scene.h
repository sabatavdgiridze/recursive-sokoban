#ifndef SCENE_H
#define SCENE_H

#include "AnimationManager.h"
#include "InputManager.h"


class Scene {
public:
  GameCamera* camera;
  InputManager* input_manager;
  AnimationManager* animation_manager;
  EventBus* event_bus;

  Scene()
  : camera(camera),
    input_manager(input_manager) {
    event_bus = new EventBus();
    animation_manager = new AnimationManager(event_bus);
    if (input_manager) {
      event_bus->add_listener(input_manager);
    }
  }
};



#endif //SCENE_H

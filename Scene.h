#ifndef SCENE_H
#define SCENE_H

#include "AnimationManager.h"
#include "InputManager.h"

class AbstractScene {
public:
  virtual ~AbstractScene() = default;

  virtual AbstractScene* update(float d_t) = 0;
  virtual void draw() = 0;
};


#endif //SCENE_H

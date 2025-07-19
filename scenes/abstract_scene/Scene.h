#ifndef SCENE_H
#define SCENE_H

#include "../Operation.h"
#include "../../AnimationManager.h"
#include "../../InputManager.h"

class AbstractScene {
public:
  virtual ~AbstractScene() = default;

  virtual Operation* update(float d_t) = 0;
  virtual void draw() = 0;
};


#endif //SCENE_H

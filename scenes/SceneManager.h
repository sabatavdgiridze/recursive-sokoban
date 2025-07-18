#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <iostream>
#include <stack>

#include "Operation.h"
#include "Scene.h"


class SceneManager {
public:
  void update(float d_t) {
    if (!scenes.empty()) {
      Operation* operation = scenes.top()->update(d_t);
      if (operation) {
        operation->apply(this);
      }
    }
  }

  void draw() {
    if (!scenes.empty()) {
      scenes.top()->draw();
    }
  }

  std::stack<AbstractScene*> scenes;
};



#endif //SCENEMANAGER_H

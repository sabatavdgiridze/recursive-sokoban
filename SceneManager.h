#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Scene.h"


class SceneManager {
private:
  AbstractScene* current = nullptr;
public:
  explicit SceneManager(AbstractScene* scene) : current(scene) {}

  void update(float d_t) {
    if (current) {
      AbstractScene* next = current->update(d_t);
      if (next) {
        current = next;
      }
    }
  }

  void draw() {
    if (current) {
      current->draw();
    }
  }
};



#endif //SCENEMANAGER_H

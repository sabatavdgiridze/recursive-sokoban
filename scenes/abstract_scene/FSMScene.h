#ifndef FSMSCENE_H
#define FSMSCENE_H

#include "Scene.h"

template<typename SceneType>
class SceneState {
public:
  virtual ~SceneState() = default;

  virtual void enter(SceneType* scene) {}
  virtual void exit(SceneType* scene) {}
  virtual Operation* update(SceneType* scene, float d_t) = 0;
  virtual void draw(SceneType* scene) = 0;
};

template<typename SceneType>
class FSMScene : public AbstractScene{
public:
  void draw() override {
    if (current_state) {
      current_state->draw(static_cast<SceneType*>(this));
    }
  }
  Operation* update(float d_t) override {
    if (current_state) {
      return current_state->update(static_cast<SceneType*>(this), d_t);
    }

    return new NoOperation();
  }

  void changeState(SceneState<SceneType>* new_state) {
    if (current_state) {
      current_state->exit(static_cast<SceneType*>(this));
    }
    if (new_state) {
      new_state->enter(static_cast<SceneType*>(this));
      current_state = new_state;
    }
  }
protected:
  SceneState<SceneType>* current_state = nullptr;
};



#endif //FSMSCENE_H

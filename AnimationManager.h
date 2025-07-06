#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
#include <vector>

#include "Animation.h"
#include "EventBus.h"


class AnimationManager {
public:
  explicit AnimationManager(EventBus* bus_ptr) : bus_ptr(bus_ptr) {}

  template<typename T>
  void push_animation(Animation<T>* animation_ptr);
  void update(float d_t);
private:
  std::vector<AnimationBase*> animations;
  EventBus* bus_ptr;
};

template<typename T>
void AnimationManager::push_animation(Animation<T> *animation_ptr) {
  bool was_empty = animations.empty();
  animations.push_back(animation_ptr);
  if (was_empty && bus_ptr) {
    bus_ptr->broadcast("INPUT_INACTIVE");
  }
}



#endif //ANIMATIONMANAGER_H

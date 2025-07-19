#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
#include <vector>

#include "Animation.h"
#include "EventBus.h"


class AnimationManager {
public:
  explicit AnimationManager(EventBus* bus_ptr) : bus_ptr(bus_ptr) {}

  void push_animation(AnimationBase* animation_ptr);
  void update(float d_t);
  int get_size();
private:
  std::vector<AnimationBase*> animations;
  EventBus* bus_ptr;
};




#endif //ANIMATIONMANAGER_H

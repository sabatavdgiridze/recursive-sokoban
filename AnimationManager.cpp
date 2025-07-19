#include "AnimationManager.h"

void AnimationManager::update(float d_t) {
  for (int i = 0; i < animations.size(); i++) {
    animations.at(i)->update(d_t);
  }
  int initial_size = animations.size();

  int complete_right = -1;
  for (int i = 0; i < animations.size(); i++) {
    if (!animations.at(i)->get_is_finished()) {
      std::swap(animations.at(i), animations.at(complete_right + 1));
      complete_right++;
    }
  }
  animations.erase(animations.begin() + (complete_right + 1), animations.end());

  if (initial_size > 0 && animations.empty() && bus_ptr) {
    bus_ptr->broadcast("INPUT_ACTIVE");
  }
}

void AnimationManager::push_animation(AnimationBase* animation_ptr) {
  bool was_empty = animations.empty();
  animations.push_back(animation_ptr);
  if (was_empty && bus_ptr) {
    bus_ptr->broadcast("INPUT_INACTIVE");
  }
}

int AnimationManager::get_size() {
  return animations.size();
}

#ifndef ANIMATION_H
#define ANIMATION_H
#include <functional>

class Tweener {
public:
  virtual ~Tweener() = default;
  virtual float get(float t) = 0;
};

class EaseOut : public Tweener {
public:
  float get(float t) override {
    return 1 - (1 - t) * (1 - t);
  }
};


class AnimationBase {
public:
  virtual ~AnimationBase() = default;
  virtual void update(float d_t) = 0;
  virtual bool get_is_finished() = 0;
};

template<typename T>
class Animation : public AnimationBase {
public:

  Animation(
    T first,
    T last,
    T* current,
    float duration,
    Tweener* tweener,
    std::function<void()> on_complete
  )
  : first(first),
    last(last),
    current(current),
    duration(duration),
    tweener(tweener),
    on_complete(on_complete) {
    *current = first;
  }

  void update(float d_t) override {
    t += d_t;
    if (t < duration) {
      *current = first * (1 - tweener->get(t / duration)) + last * tweener->get(t / duration);
    } else {
      *current = last;
      is_finished = true;
      if (on_complete) {
        on_complete();
      }
    }
  }

  bool get_is_finished() override {
    return is_finished;
  }

private:
  float t = 0, duration;
  float is_finished = false;
  Tweener* tweener;
  T first, last;
  T* current;

  std::function<void()> on_complete;
};

class DelayedAnimation : public AnimationBase {
public:
  DelayedAnimation(AnimationBase* animation, float delay_time)
    : animation(animation), delay_time(delay_time) {}

  void update(float d_t) override {
    delay_timer += d_t;

    if (delay_timer >= delay_time) {
      // Delay is over, update the wrapped animation
      animation->update(d_t);
    }
  }

  bool get_is_finished() override {
    if (delay_timer < delay_time) {
      return false; // Still in delay phase
    }
    return animation->get_is_finished();
  }

  ~DelayedAnimation() {
    delete animation;
  }

private:
  AnimationBase* animation;
  float delay_time;
  float delay_timer = 0.0f;
};

class ParallelAnimation : public AnimationBase {
public:
  ParallelAnimation(std::vector<AnimationBase*> animations)
    : animations(animations) {}

  void update(float d_t) override {
    for (auto* animation : animations) {
      if (!animation->get_is_finished()) {
        animation->update(d_t);
      }
    }
  }

  bool get_is_finished() override {
    for (auto* animation : animations) {
      if (!animation->get_is_finished()) {
        return false; // At least one animation is still running
      }
    }
    return true; // All animations finished
  }

  ~ParallelAnimation() {
    for (auto* animation : animations) {
      delete animation;
    }
  }

private:
  std::vector<AnimationBase*> animations;
};

#endif //ANIMATION_H

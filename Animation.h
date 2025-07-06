#ifndef ANIMATION_H
#define ANIMATION_H

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
class Animation : AnimationBase {
public:
  Animation(T first, T last, T* current, float duration, Tweener* tweener) : first(first), last(last), current(current), duration(duration), tweener(tweener) {
    *current = first;
  }

  void update(float d_t) override {
    t += d_t;
    if (t < duration) {
      *current = first * (1 - tweener->get(t / duration)) + last * tweener->get(t / duration);
    } else {
      *current = last;
      is_finished = true;
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
};



#endif //ANIMATION_H

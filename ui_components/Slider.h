#ifndef SLIDER_H
#define SLIDER_H

#include <functional>

#include "../GameCamera.h"
#include "../../UI.h"

class Slider {
private:
  std::vector<UI> selectors;
  int current_value = 1;
  int n;

  std::function<void(int)> callback;
public:
  void draw(GameCamera* camera);
  void update(float d_t, GameCamera* camera);

  Slider(Vector2 initial_pos, float padding, float size, int n, std::function<void(int)> callback) : n(n), callback(callback) {
    for (int i = 0; i < n; i++) {
      selectors.push_back(UI({initial_pos.x + (padding + size) * i, initial_pos.y, size, size}));
    }
  }
};



#endif //SLIDER_H

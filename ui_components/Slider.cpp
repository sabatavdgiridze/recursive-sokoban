#include "Slider.h"

#include "../Chaikin.h"

void Slider::draw(GameCamera *camera) {
  for (int i = 1; i <= n; i++) {
    camera -> draw({
      {
        Chaikin < Vector2 > ::subdivide(selectors.at(i - 1).to_ccw_beveled_points(2, 20), {}, 4),
          i <= current_value
      }
    }, GRAY);
  }
}

void Slider::update(float d_t, GameCamera *camera) {
  Vector2 mouse_screen = GetMousePosition();
  Vector2 mouse_world = camera->inverse_transform(mouse_screen);

  for (int i = 0; i < n; i++) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_world, selectors.at(i))) {
      current_value = i + 1;
      callback(current_value);
    }
  }
}

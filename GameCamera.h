#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "raylib.h"


class GameCamera {
public:
  GameCamera() = default;

  GameCamera(
    const Vector2 &origin,
    const Vector2 &screen_pos,
    const Vector2 &x_axis,
    const Vector2 &y_axis)
    : origin(origin),
      screen_pos(screen_pos),
      x_axis(x_axis),
      y_axis(y_axis),
      ratio(1) {
  }

  void draw_polygons(std::vector<std::vector<Vector2>> borders, Color color) {
    for (auto border : borders) {
      for (int i = 0; i < border.size(); i++) {
        auto f = transform(border.at(i));
        auto s = transform(border.at((i + 1) % border.size()));
        DrawLineEx(f, s, 2.0f, color);

      }
    }
  }

  Vector2 transform(Vector2 world_pos);

  Vector2 origin{};
  Vector2 screen_pos{};
  Vector2 x_axis{};
  Vector2 y_axis{};

  float ratio;
};



#endif //CAMERA_H

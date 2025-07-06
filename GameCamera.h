#ifndef CAMERA_H
#define CAMERA_H

#include "raymath.h"


class GameCamera {
public:
  GameCamera(
    const Vector2 &origin,
    const Vector2 &screen_pos,
    const Vector2 &x_axis,
    const Vector2 &y_axis)
    : origin(origin),
      screen_pos(screen_pos),
      x_axis(x_axis),
      y_axis(y_axis) {
  }

  Vector2 transform(Vector2 world_pos);

  Vector2 origin;
  Vector2 screen_pos;
  Vector2 x_axis;
  Vector2 y_axis;
};



#endif //CAMERA_H

#include "GameCamera.h"

#include "raymath.h"

Vector2 GameCamera::transform(Vector2 world_pos) {
  float a = x_axis.x;
  float b = y_axis.x;
  float c = x_axis.y;
  float d = y_axis.y;

  float det = a * d - b * c;
  Vector2 local = world_pos - origin;

  Vector2 out;
  out.x =  d * local.x - b * local.y;
  out.y = -c * local.x + a * local.y;

  return Vector2Scale(out, 1.0 / (det * ratio)) + screen_pos;
}

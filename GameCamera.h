#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "raylib.h"
#include "geometric_operations/Triangulation.h"


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
  void draw(std::vector<std::pair<std::vector<Vector2>, bool>> borders, Color color) {
    for (auto [border, filled] : borders) {
      if (filled) {
        fill_polygons(border, color);
      } else {
        draw_polygons({border}, color);
      }
    }
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

  void fill_polygons(std::vector<Vector2> border, Color color) {
    std::vector<std::vector<Vector2>> triangles = Triangulation::triangulate_multiple({border});

    for (const auto& triangle : triangles) {
      if (triangle.size() == 3) {
        Vector2 p1 = transform(triangle[0]);
        Vector2 p2 = transform(triangle[1]);
        Vector2 p3 = transform(triangle[2]);
        DrawTriangle(p1, p2, p3, color);
      }
    }
  }

  Vector2 transform(Vector2 world_pos);
  Vector2 inverse_transform(Vector2 screen_pos);

  Vector2 origin{};
  Vector2 screen_pos{};
  Vector2 x_axis{};
  Vector2 y_axis{};

  float ratio;
};



#endif //CAMERA_H

#ifndef UI_H
#define UI_H

#include <cmath>
#include <vector>

#include "raylib.h"


struct UI {
  Rectangle rect {};
  std::vector<float> cols {}, rows{};

  UI() {}
  UI(Rectangle rect) : rect(rect) {}

  UI grid(std::initializer_list<float> cols, std::initializer_list<float> rows) {
    UI copy = *this;
    copy.cols = cols;
    copy.rows = rows;

    return copy;
  }

  UI grid(int num_cols, int num_rows) {
    UI copy = *this;
    copy.cols = std::vector<float>(num_cols, 1.0f);
    copy.rows = std::vector<float>(num_rows, 1.0f);

    return copy;
  }

  UI row(std::initializer_list<float> weights) {
    return grid(weights, {1});
  }

  UI col(std::initializer_list<float> weights) {
    return grid({1}, weights);
  }

  UI at(int col, int row) {
    float total_w = 0;
    float total_h = 0;

    float at_w = 0;
    float at_h = 0;

    for (int i = 0; i < cols.size(); i++) {
      total_w += cols[i];
      if (i < col) {
        at_w += cols[i];
      }
    }
    for (int i = 0; i < rows.size(); i++) {
      total_h += rows[i];
      if (i < row) {
        at_h += rows[i];
      }
    }
    return UI({
      rect.x + rect.width * (at_w / total_w),
      rect.y + rect.height * (at_h / total_h),
      rect.width * (cols[col] / total_w),
      rect.height * (rows[row] / total_h)
    });
  }

  UI pad(float padding) {
    return UI({
      rect.x + padding,
      rect.y + padding,
      rect.width - 2 * padding,
      rect.height - 2 * padding
    });
  }

  operator Rectangle() {
    return rect;
  }

  float* get_x() {
    return &rect.x;
  }

  float* get_y() {
    return &rect.y;
  }

  std::vector<Vector2> to_ccw_points() const {
    return {
      {rect.x, rect.y},
      {rect.x + rect.width, rect.y},
      {rect.x + rect.width, rect.y + rect.height},
      {rect.x, rect.y + rect.height}
    };
  }

  std::vector<Vector2> to_ccw_beveled_points(int corner_points = 2, float point_distance = 50.0f) const {
    std::vector<Vector2> points;

    // Define the four corners in CCW order
    Vector2 corners[4] = {
      {rect.x, rect.y}, // Top-left
      {rect.x + rect.width, rect.y}, // Top-right
      {rect.x + rect.width, rect.y + rect.height}, // Bottom-right
      {rect.x, rect.y + rect.height} // Bottom-left
    };

    // For each corner, we need to add points along the two adjacent edges
    for (int i = 0; i < 4; i++) {
      Vector2 current_corner = corners[i];
      Vector2 prev_corner = corners[(i + 3) % 4]; // Previous corner (CCW)
      Vector2 next_corner = corners[(i + 1) % 4]; // Next corner (CCW)

      // Calculate normalized direction vectors
      Vector2 incoming_dir = {
        current_corner.x - prev_corner.x,
        current_corner.y - prev_corner.y
      };
      Vector2 outgoing_dir = {
        next_corner.x - current_corner.x,
        next_corner.y - current_corner.y
      };

      // Normalize directions
      float incoming_len = sqrtf(incoming_dir.x * incoming_dir.x + incoming_dir.y * incoming_dir.y);
      float outgoing_len = sqrtf(outgoing_dir.x * outgoing_dir.x + outgoing_dir.y * outgoing_dir.y);

      if (incoming_len > 0) {
        incoming_dir.x /= incoming_len;
        incoming_dir.y /= incoming_len;
      }
      if (outgoing_len > 0) {
        outgoing_dir.x /= outgoing_len;
        outgoing_dir.y /= outgoing_len;
      }

      // Add points before the corner (moving backward along incoming edge)
      for (int j = corner_points; j >= 1; j--) {
        Vector2 point = {
          current_corner.x - incoming_dir.x * point_distance * j,
          current_corner.y - incoming_dir.y * point_distance * j
        };
        points.push_back(point);
      }

      // Add the corner point itself
      points.push_back(current_corner);

      // Add points after the corner (moving forward along outgoing edge)
      for (int j = 1; j <= corner_points; j++) {
        Vector2 point = {
          current_corner.x + outgoing_dir.x * point_distance * j,
          current_corner.y + outgoing_dir.y * point_distance * j
        };
        points.push_back(point);
      }
    }

    return points;
  }
};



#endif //UI_H

#ifndef UI_H
#define UI_H

#include <vector>

#include "raylib.h"


struct UI {
  Rectangle rect;
  std::vector<float> cols, rows;

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
};



#endif //UI_H

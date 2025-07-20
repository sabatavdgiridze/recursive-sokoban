#ifndef CHAIKIN_H
#define CHAIKIN_H

#include <algorithm>
#include <vector>

#include "raylib_common.h"

template<typename PointType>
class Chaikin {
public:
  static std::vector<PointType> subdivide(
    std::vector<PointType> in_points,
    std::vector<int> fixed_idxs,
    int iterations
    ) {
      std::vector<PointType> points;
      for (PointType point : in_points) {
        points.push_back(point);
      }
      while (iterations--) {
        std::vector<PointType> new_points;
        std::vector<int> new_fixed_idxs;

        for (int i = 0; i < points.size(); i++) {
          int j = (i + 1) % points.size();

          PointType first = points.at(i) * 0.75f + points.at(j) * 0.25f;
          PointType second = points.at(i) * 0.25f + points.at(j) * 0.75f;

          new_points.push_back(first);
          if (std::find(fixed_idxs.begin(), fixed_idxs.end(), j) != fixed_idxs.end()) {
            new_points.push_back(points.at(j));
            new_fixed_idxs.push_back(new_points.size() - 1);
          } else {
            new_points.push_back(second);
          }
        }
        points = new_points;
        fixed_idxs = new_fixed_idxs;
      }
      return points;
  }

};

template<>
class Chaikin<std::pair<int, int>> {
public:
  static std::vector<Vector2> subdivide(
    std::vector<std::pair<int, int>> in_points,
    std::vector<int> fixed_idxs,
    int iterations
  ) {
    std::vector<Vector2> points;
    for (auto point : in_points) {
      points.push_back(Vector2{(float)point.first, (float)point.second});
    }

    return Chaikin<Vector2>::subdivide(points, fixed_idxs, iterations);
  }
};



#endif //CHAIKIN_H

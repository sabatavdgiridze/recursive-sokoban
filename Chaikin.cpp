#include "Chaikin.h"

#include "raymath.h"

std::vector<Vector2> Chaikin::subdivide(std::vector<std::pair<int, int> > in_points, std::vector<int> fixed_idxs, int iterations)
{
  std::vector<Vector2> points;
  for (std::pair<int, int> point : in_points) {
    points.push_back(Vector2{(float) point.first, (float) point.second});
  }
  while (iterations--) {
    std::vector<Vector2> new_points;
    std::vector<int> new_fixed_idxs;

    for (int i = 0; i < points.size(); i++) {
      int j = (i + 1) % points.size();

      Vector2 first = Vector2Scale(points.at(i), 0.75) + Vector2Scale(points.at(j), 0.25);
      Vector2 second = Vector2Scale(points.at(i), 0.25) + Vector2Scale(points.at(j), 0.75);

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

#ifndef CHAIKIN_H
#define CHAIKIN_H

#include <algorithm>
#include <vector>
#include "raylib.h"

class Chaikin {
public:
  static std::vector<Vector2> subdivide(
    std::vector<std::pair<int, int>> in_points,
    std::vector<int> fixed_idxs,
    int iterations
  );
};



#endif //CHAIKIN_H

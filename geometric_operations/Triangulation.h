#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <vector>
#include "raylib.h"

class Triangulation {
public:
    // Main triangulation function - assumes CCW points
    static std::vector<std::vector<Vector2>> triangulate(const std::vector<Vector2>& ccw_polygon);

    // Triangulate multiple polygons
    static std::vector<std::vector<Vector2>> triangulate_multiple(const std::vector<std::vector<Vector2>>& polygons);

private:
    // Helper functions
    static bool point_in_triangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c);
    static float cross_product(Vector2 a, Vector2 b, Vector2 c);
    static bool is_ear(const std::vector<Vector2>& polygon, int prev, int curr, int next);
    static bool is_ccw(const std::vector<Vector2>& polygon);
};
#endif // TRIANGULATION_H
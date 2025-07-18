#include "Triangulation.h"

#include <cmath>

std::vector<std::vector<Vector2>> Triangulation::triangulate(const std::vector<Vector2>& ccw_polygon) {
    std::vector<std::vector<Vector2>> triangles;

    if (ccw_polygon.size() < 3) return triangles;
    if (ccw_polygon.size() == 3) {
        triangles.push_back(ccw_polygon);
        return triangles;
    }

    // Create indices for ear clipping
    std::vector<int> indices;
    for (int i = 0; i < ccw_polygon.size(); i++) {
        indices.push_back(i);
    }

    // Ear clipping algorithm
    while (indices.size() > 3) {
        bool ear_found = false;

        for (int i = 0; i < indices.size(); i++) {
            int prev = (i - 1 + indices.size()) % indices.size();
            int curr = i;
            int next = (i + 1) % indices.size();

            if (is_ear(ccw_polygon, indices[prev], indices[curr], indices[next])) {
                // Found an ear, create triangle
                std::vector<Vector2> triangle = {
                    ccw_polygon[indices[prev]],
                    ccw_polygon[indices[curr]],
                    ccw_polygon[indices[next]]
                };
                triangles.push_back(triangle);

                // Remove the ear vertex
                indices.erase(indices.begin() + curr);
                ear_found = true;
                break;
            }
        }

        if (!ear_found) {
            // Fallback: create a triangle fan from first vertex
            for (int i = 1; i < indices.size() - 1; i++) {
                std::vector<Vector2> triangle = {
                    ccw_polygon[indices[0]],
                    ccw_polygon[indices[i]],
                    ccw_polygon[indices[i + 1]]
                };
                triangles.push_back(triangle);
            }
            break;
        }
    }

    // Add the final triangle
    if (indices.size() == 3) {
        std::vector<Vector2> triangle = {
            ccw_polygon[indices[0]],
            ccw_polygon[indices[1]],
            ccw_polygon[indices[2]]
        };
        triangles.push_back(triangle);
    }

    return triangles;
}

std::vector<std::vector<Vector2>> Triangulation::triangulate_multiple(const std::vector<std::vector<Vector2>>& polygons) {
    std::vector<std::vector<Vector2>> all_triangles;

    for (const auto& polygon : polygons) {
        std::vector<std::vector<Vector2>> triangles = triangulate(polygon);
        all_triangles.insert(all_triangles.end(), triangles.begin(), triangles.end());
    }

    return all_triangles;
}

bool Triangulation::point_in_triangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) {
    // Using barycentric coordinates
    float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    if (std::abs(denom) < 1e-10) return false; // Degenerate triangle

    float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
    float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
    float gamma = 1.0f - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

float Triangulation::cross_product(Vector2 a, Vector2 b, Vector2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool Triangulation::is_ear(const std::vector<Vector2>& polygon, int prev, int curr, int next) {
    Vector2 a = polygon[prev];
    Vector2 b = polygon[curr];
    Vector2 c = polygon[next];

    // Check if the triangle is oriented counter-clockwise (convex)
    if (cross_product(a, b, c) <= 0) {
        return false; // Reflex angle, not an ear
    }

    // Check if any other vertex is inside this triangle
    for (int i = 0; i < polygon.size(); i++) {
        if (i == prev || i == curr || i == next) continue;

        if (point_in_triangle(polygon[i], a, b, c)) {
            return false; // Another vertex is inside, not an ear
        }
    }

    return true; // Valid ear
}

bool Triangulation::is_ccw(const std::vector<Vector2>& polygon) {
    if (polygon.size() < 3) return false;

    // Calculate signed area
    float area = 0;
    for (int i = 0; i < polygon.size(); i++) {
        int j = (i + 1) % polygon.size();
        area += (polygon[j].x - polygon[i].x) * (polygon[j].y + polygon[i].y);
    }

    return area < 0; // Negative area means CCW
}

#ifndef EDGEMERGER_H
#define EDGEMERGER_H

#include <cstdio>
#include <vector>
#include <set>
#include <functional>

#include "Chaikin.h"
#include "raymath.h"

class EdgeMerger {
public:
  using Edge = std::pair<std::pair<int, int>, std::pair<int, int> >;
  using CellChecker = std::function<bool(int row, int col)>;

  struct GridInfo {
    int rows;
    int cols;
    CellChecker is_solid;
  };

  static std::vector<std::vector<Vector2> > get_merged_borders(const GridInfo &grid) {
    std::vector<std::vector<Vector2> > borders;
    std::set<Edge> edges;

    for (int row = 0; row < grid.rows; row++) {
      for (int col = 0; col < grid.cols; col++) {
        if (grid.is_solid(row, col)) {
          int x = col;
          int y = row;

          std::vector<std::pair<int, int> > cell_corners = {
            {x, y},
            {x + 1, y},
            {x + 1, y + 1},
            {x, y + 1}
          };

          add_edges(cell_corners, edges);
        }
      }
    }

    int total_edges = edges.size();
    int processed_edges = 0;

    while (processed_edges < total_edges) {
      if (edges.empty()) break;

      std::vector<Edge> current_border;
      Edge first_edge = *edges.begin();
      current_border.push_back(first_edge);
      edges.erase(first_edge);
      processed_edges++;

      while (current_border.back().second != current_border.front().first) {
        bool found_next = false;
        auto best_it = edges.end();
        double min_cross_product = std::numeric_limits<double>::max();

        for (auto it = edges.begin(); it != edges.end(); ++it) {
          if (it->first == current_border.back().second) {
            if (!found_next) {
              best_it = it;
              found_next = true;
            } else {
              // Calculate cross product to determine turn direction
              Vector2 current_dir = Vector2(current_border.back().second.first, current_border.back().second.second) - Vector2(current_border.back().first.first, current_border.back().first.second);
              Vector2 next_dir = Vector2(it->second.first, it->second.second) - Vector2(it->first.first, it->first.second);
              double cross = current_dir.x * next_dir.y - current_dir.y * next_dir.x;

              if (cross < min_cross_product) {
                min_cross_product = cross;
                best_it = it;
              }
            }
          }
        }

        if (found_next) {
          current_border.push_back(*best_it);
          edges.erase(best_it);
          processed_edges++;
        } else {
          break;
        }
      }

      std::vector<Vector2> loop_points = edges_to_points(current_border);
      borders.push_back(loop_points);
    }

    return borders;
  }

  static std::vector<std::vector<Vector2> > get_merged_borders(
    const std::vector<std::vector<bool> > &bool_grid) {
    GridInfo grid_info = {
      static_cast<int>(bool_grid.size()),
      static_cast<int>(bool_grid.empty() ? 0 : bool_grid[0].size()),
      [&bool_grid](int row, int col) {
        return bool_grid[row][col];
      }
    };

    return get_merged_borders(grid_info);
  }

  static std::vector<std::vector<Vector2> > get_transformed_borders(
    const std::vector<std::vector<bool> > &bool_grid,
    Vector2 origin,
    float cell_width,
    float cell_height) {

    auto borders = uniteBordersUntilPossible(get_merged_borders(bool_grid));
    clean_borders(borders);

    printf("size: %d\n", borders.size());


    for (auto& border : borders) {
      std::vector<std::pair<int, int>> int_points;
      for (const auto& point : border) {
        int_points.push_back({(int)point.x, (int)point.y});
      }

      std::vector<int> fixed_idxs;
      border = Chaikin::subdivide(int_points, fixed_idxs, 2);
    }

    for (auto &border: borders) {
      for (auto &point: border) {
        point = Vector2{
          origin.x + point.x * cell_width,
          origin.y + point.y * cell_height
        };
      }
    }

    return borders;
  }

  struct CommonPointResult {
      bool found;
      int index1;
      int index2;
  };

  static CommonPointResult findCommonPoint(const std::vector<Vector2>& border1,
                                   const std::vector<Vector2>& border2) {
      for (int i = 0; i < border1.size(); ++i) {
          for (int j = 0; j < border2.size(); ++j) {
              if (border1[i].x == border2[j].x && border1[i].y == border2[j].y) {
                  return {true, i, j};
              }
          }
      }
      return {false, -1, -1};
  }

  static std::vector<std::vector<Vector2>> uniteBordersUntilPossible(std::vector<std::vector<Vector2>> borders) {
      bool merged = true;

      while (merged) {
          merged = false;

          for (int i = 0; i < borders.size() && !merged; ++i) {
              for (int j = i + 1; j < borders.size() && !merged; ++j) {

                  CommonPointResult result = findCommonPoint(borders[i], borders[j]);

                  if (result.found) {
                      std::vector<Vector2> mergedBorder = mergeBordersAtPoint(
                          borders[i], borders[j], result.index1, result.index2);

                      std::vector<std::vector<Vector2>> newBorders;
                      newBorders.reserve(borders.size() - 1);

                      for (int k = 0; k < borders.size(); ++k) {
                          if (k != i && k != j) {
                              newBorders.push_back(std::move(borders[k]));
                          }
                      }

                      newBorders.push_back(std::move(mergedBorder));

                      borders = std::move(newBorders);
                      merged = true;
                  }
              }
          }
      }

      return borders;
  }

private:
  static void add_edges(const std::vector<std::pair<int, int> > &points, std::set<Edge> &edges) {
    for (int i = 0; i < points.size(); i++) {
      Edge edge = {points[i], points[(i + 1) % points.size()]};
      Edge reverse_edge = {edge.second, edge.first};

      if (edges.contains(reverse_edge)) {
        edges.erase(reverse_edge);
      } else {
        edges.insert(edge);
      }
    }
  }
  static void clean_borders(std::vector<std::vector<Vector2>>& borders) {
    for (auto& border : borders) {
      if (border.size() <= 2) continue;

      std::vector<Vector2> cleaned_border;
      cleaned_border.push_back(border[0]);

      for (size_t i = 1; i < border.size() - 1; ++i) {
        Vector2 dir1 = border[i] - border[i-1];
        Vector2 dir2 = border[i+1] - border[i];

        double len1 = sqrt(dir1.x * dir1.x + dir1.y * dir1.y);
        double len2 = sqrt(dir2.x * dir2.x + dir2.y * dir2.y);

        if (len1 > 0 && len2 > 0) {
          dir1.x /= len1;
          dir1.y /= len1;
          dir2.x /= len2;
          dir2.y /= len2;

          double dot_product = dir1.x * dir2.x + dir1.y * dir2.y;
          if (abs(dot_product - 1.0) > 1e-9) {
            cleaned_border.push_back(border[i]);
          }
        } else {
          cleaned_border.push_back(border[i]);
        }
      }

      cleaned_border.push_back(border.back());
      border = cleaned_border;
    }
  }


  static std::vector<Vector2> edges_to_points(const std::vector<Edge> &edges) {
    std::vector<Vector2> points;
    for (const Edge &edge: edges) {
      points.push_back(Vector2{
        static_cast<float>(edge.first.first),
        static_cast<float>(edge.first.second)
      });
    }
    return points;
  }

  static void shift_border_to_start(std::vector<Vector2>& border, int startIndex) {
    if (border.empty() || startIndex < 0 || startIndex >= border.size()) {
      return;
    }

    std::rotate(border.begin(), border.begin() + startIndex, border.end());
  }

  static std::vector<Vector2> mergeBordersAtPoint(std::vector<Vector2> border1,
                                        std::vector<Vector2> border2,
                                        int index_1,
                                        int index_2) {


    shift_border_to_start(border1, index_1);
    shift_border_to_start(border2, index_2);

    std::vector<Vector2> mergedBorder;
    mergedBorder.reserve(border1.size() + border2.size() - 1);

    mergedBorder.insert(mergedBorder.end(), border1.begin(), border1.end());
    mergedBorder.insert(mergedBorder.end(), border2.begin() + 1, border2.end());

    return mergedBorder;
  }
};

#endif //EDGEMERGER_H

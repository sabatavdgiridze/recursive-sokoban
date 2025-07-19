#include "UIButton.h"

#include "../EdgeMerger.h"

void UIButton::draw(GameCamera* camera) {
  std::vector<std::vector<bool>> text_grid(rows, std::vector<bool>(cols, false));

  int row_offset = 1;
  int cursor_col = 1;
  for (char c : text) {
    int width = font_loader->get_character_width(c);
    auto grid = font_loader->characters.at(c);

    for (int col = 0; col < grid.at(0).size(); col++) {
      for (int row = grid.size() - 1; row >= 0; row--) {
        if (grid.at(grid.size() - row - 1).at(col)) {
          text_grid[row_offset + row][cursor_col + col] = true;
        }
      }
    }
    cursor_col+= width + 1;
  }

  UI button_grid = ui.grid(cols, rows);
  float cell_width = button_grid.rect.width / cols;
  float cell_height = button_grid.rect.height / rows;

  auto borders = EdgeMerger::get_transformed_borders(
      text_grid,
      Vector2{button_grid.rect.x, button_grid.rect.y},
      cell_width,
      cell_height
  );

  if (!inside) {
    camera->draw_polygons(borders, GRAY);
    return;
  }

  for (auto border : borders) {
    camera->fill_polygons(border, GRAY);
  }
}

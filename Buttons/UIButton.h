#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "../GameCamera.h"
#include "../UI.h"
#include "../fonts/FontLoader.h"


class UIButton {
public:
  explicit UIButton(UI ui, std::string text, FontLoader* font_loader) : ui(ui), text(text), font_loader(font_loader) {
    cols = 80;
    rows = 30;
  }

  void draw(GameCamera* camera) {
    camera->draw_polygons({ui.to_ccw_points()}, GRAY);

    int row_offset = 1;
    int cursor_col = 1;
    for (char c : text) {
      int width = font_loader->get_character_width(c);
      auto grid = font_loader->characters.at(c);

      for (int col = 0; col < grid.at(0).size(); col++) {
        for (int row = grid.size() - 1; row >= 0; row--) {
          if (grid.at(row).at(col)) {
            camera->draw_polygons({ui.grid(cols, rows).at(cursor_col + col, row_offset + (grid.size() - row - 1)).to_ccw_points()}, GRAY);
          }
        }
      }
      cursor_col+= width + 1;
    }
  }

  void update(float d_t) {

  }
private:
  UI ui;
  int cols;
  int rows;
  std::string text;
  FontLoader* font_loader;
};



#endif //UIBUTTON_H

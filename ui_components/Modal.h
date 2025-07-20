#ifndef MODAL_H
#define MODAL_H

#include "../Chaikin.h"
#include "../GameCamera.h"
#include "../../UI.h"

class Modal {
public:
  UI rectangle;
  UI main, first_button_ui, second_button_ui;

  Modal() {}

  Modal(UI rectangle) : rectangle(rectangle) {
    generate_layout(rectangle);
  }

  void generate_layout(UI rectangle) {
    main = rectangle.grid({1}, {1, 2, 8}).at(0, 2);

    UI panel = rectangle.grid({1}, {1, 3}).at(0, 0);
    UI button_grid = panel.grid({10, 1, 10}, {1});
    first_button_ui = button_grid.at(0, 0);
    second_button_ui = button_grid.at(2, 0);
  }


  void draw(GameCamera* camera) {
    generate_layout(rectangle);

    camera->draw({{Chaikin<Vector2>::subdivide(main.to_ccw_beveled_points(2, 20), {}, 2), true}}, WHITE);
    camera->draw({{Chaikin<Vector2>::subdivide(first_button_ui.to_ccw_beveled_points(2, 20), {}, 4), true}}, WHITE);
    camera->draw({{Chaikin<Vector2>::subdivide(second_button_ui.to_ccw_beveled_points(2, 20), {}, 4), true}}, WHITE);

    camera->draw({{Chaikin<Vector2>::subdivide(main.to_ccw_beveled_points(2, 20), {}, 2), false}}, GRAY);
    camera->draw({{Chaikin<Vector2>::subdivide(first_button_ui.to_ccw_beveled_points(2, 20), {}, 4), false}}, GRAY);
    camera->draw({{Chaikin<Vector2>::subdivide(second_button_ui.to_ccw_beveled_points(2, 20), {}, 4), false}}, GRAY);
  }
};



#endif //MODAL_H

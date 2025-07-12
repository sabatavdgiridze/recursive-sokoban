#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "../GameCamera.h"
#include "../UI.h"


class UIButton {
public:
  explicit UIButton(UI ui) : ui(ui) {}

  void draw(GameCamera* camera) {
    camera->draw_polygons({ui.to_ccw_points()}, GRAY);
  }

  void update(float d_t) {

  }
private:
  UI ui;
};



#endif //UIBUTTON_H

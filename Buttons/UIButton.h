#ifndef UIBUTTON_H
#define UIBUTTON_H


#include "../GameCamera.h"
#include "../UI.h"
#include "../fonts/FontLoader.h"


class UIButton {
public:
  explicit UIButton(UI ui, std::string text, FontLoader* font_loader) : ui(ui), text(text), font_loader(font_loader) {
    cols = 100;
    rows = 30;
  }

  void draw(GameCamera* camera);

  void update(float d_t, GameCamera* camera) {
    Vector2 mouse_screen = GetMousePosition();

    Vector2 mouse_world = camera->inverse_transform(mouse_screen);

    Rectangle ui_rect = ui;
    if (CheckCollisionPointRec(mouse_world, ui_rect) && inside == false) {
      inside = true;
      ui.rect.x += 50;
    }
    if (!CheckCollisionPointRec(mouse_world, ui_rect) && inside == true) {
      inside = false;
      ui.rect.x -= 50;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_world, ui_rect)) {

    }
  }
private:
  UI ui;
  int cols;
  int rows;
  bool inside = false;
  std::string text;
  FontLoader* font_loader;
};



#endif //UIBUTTON_H

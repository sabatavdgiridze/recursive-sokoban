#ifndef UIBUTTON_H
#define UIBUTTON_H


#include "../GameCamera.h"
#include "../UI.h"
#include "../fonts/FontLoader.h"
#include "../scenes/Operation.h"


class Operation;

class UIButton {
public:
  explicit UIButton(UI ui, std::string text, FontLoader* font_loader, std::function<Operation*()> callback) : ui(ui), text(text), font_loader(font_loader), callback(callback) {
    cols = 100;
    rows = 30;
  }

  void draw(GameCamera* camera);

  Operation* update(float d_t, GameCamera* camera) {
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
      return callback();
    }
    return nullptr;
  }


private:
  UI ui;
  int cols;
  int rows;
  bool inside = false;
  std::string text;
  FontLoader* font_loader;

  std::function<Operation*()> callback;
};



#endif //UIBUTTON_H

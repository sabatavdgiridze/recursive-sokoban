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

    if (CheckCollisionPointRec(mouse_world, ui) && inside == false) {
      inside = true;
    }
    if (!CheckCollisionPointRec(mouse_world, ui) && inside == true) {
      inside = false;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_world, ui)) {
      return callback();
    }
    return nullptr;
  }
  UI ui;

private:
  int cols;
  int rows;
  bool inside = false;
  std::string text;
  FontLoader* font_loader;

  std::function<Operation*()> callback;
};



#endif //UIBUTTON_H

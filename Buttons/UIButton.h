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

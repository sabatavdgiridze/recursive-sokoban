#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../abstract_scene/FSMScene.h"
#include "../abstract_scene/Scene.h"

#include "../../UI.h"
#include "../../Buttons/UIButton.h"

class GameScene;

class PlayingState : public SceneState<GameScene> {
public:
  void enter(GameScene* scene) override;

  void exit(GameScene* scene) override;

  Operation * update(GameScene* scene, float d_t) override;

  void draw(GameScene* scene) override;
};


class PauseState : public SceneState<GameScene> {
public:
  void enter(GameScene* scene) override;

  void exit(GameScene* scene) override;

  Operation * update(GameScene* scene, float d_t) override;

  void draw(GameScene* scene) override;
private:

};


class GameScene : public FSMScene<GameScene> {
public:
  Board* board;
  GameCamera* camera = nullptr;
  InputManager* input_manager = nullptr;
  AnimationManager* animation_manager = new AnimationManager(nullptr);

  std::vector<UIButton*> buttons;


  explicit GameScene(Board* board) : board(board) {
    board->set_position({0, 0}, 500);
    input_manager = new InputManager(board, board->find_player_coords_rec());

    camera = new GameCamera();
    camera->origin = {250, 250};
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};
    camera->ratio = 1;

    current_state = new PlayingState();

    UI screen({700, 0, 500, 500});
    UI col = screen.pad(50).col({3, 2, 3});

    auto bottom = col.at(0, 0);
    auto top = col.at(0, 2);

    FontLoader* font_loader = new FontLoader();
    font_loader->load("../fonts/font_data.json");

    buttons.push_back(new UIButton(top, "LEVELS", font_loader, []() -> Operation* {
      // return new PopAllOperation(new LevelScene(GameDataReader::create_boards("../boards")));
      return new NoOperation();
    }));
    buttons.push_back(new UIButton(bottom, "SETTINGS", font_loader, []() -> Operation* {
      return new NoOperation();
    }));

  }

};



#endif //GAMESCENE_H

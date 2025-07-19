#ifndef LEVELSCENE_H
#define LEVELSCENE_H


#include "GameScene/GameScene.h"
#include "abstract_scene/Scene.h"


class LevelScene : public AbstractScene, public Notified {
public:
  GameCamera* camera = nullptr;
  AnimationManager* animation_manager = nullptr;
  EventBus* event_bus = nullptr;

  std::vector<Board*> boards;
  std::vector<Vector2> centers;
  int curr_idx = 0;

  bool is_active = true;

  LevelScene(std::vector<Board*> boards) : boards(boards) {

    float length = 500;
    float padding = 200;

    for (int i = 0; i < boards.size(); i++) {
      boards.at(i)->set_position(Vector2{(length + padding) * i, 0}, length);
      centers.push_back(Vector2{(length + padding) * i + length / 2, length / 2});
    }

    event_bus = new EventBus();
    event_bus->add_listener(this);
    animation_manager = new AnimationManager(event_bus);

    init();
  }

  void notify(std::string message) override {
    if (message == "INPUT_ACTIVE") {
      is_active = true;
    }
    if (message == "INPUT_INACTIVE") {
      is_active = false;
    }
  }

  void set_camera(int idx) {
    auto on_complete = [this, idx] () {
      curr_idx = idx;
    };
    animation_manager->push_animation(
      new Animation<Vector2>(
        centers.at(curr_idx),
        centers.at(idx),
        &camera->origin,
        1,
        new EaseOut(),
        on_complete
      )
    );
  }

  void draw() override {
    for (Board* board : boards) {
      board->draw(camera);
    }
  }

  Operation* update(float d_t) override {
    animation_manager->update(d_t);
    if (is_active) {
      if (IsKeyPressed(KEY_LEFT)) {
        move(-1);
      } else if (IsKeyPressed(KEY_RIGHT)) {
        move(1);
      } else if (IsKeyPressed(KEY_ENTER)) {
        return new PopAllOperation(new GameScene(boards.at(curr_idx)));
      }
    }

    return nullptr;
  }

  void move(int dir) {
    int new_idx = curr_idx + dir;

    if (new_idx < 0) {
      new_idx = 0;
    }

    if (new_idx > boards.size() - 1) {
      new_idx = boards.size() - 1;
    }
    set_camera(new_idx);
  }

  void init() {
    camera = new GameCamera();
    camera->screen_pos = {400,400};
    camera->x_axis     = {1,0};
    camera->y_axis     = {0,-1};

    float total_width = (500 + 200) * (boards.size() - 1) + 500;
    Vector2 overview_center = {total_width / 2, 250};
    float overview_ratio = total_width * 1.5 / 800.0f;

    camera->origin = overview_center;
    camera->ratio = 10;

    auto on_complete = [this] () {
      curr_idx = 0;
      is_active = true;
    };

    is_active = false;
    animation_manager->push_animation(
      new DelayedAnimation(
        new ParallelAnimation(
          {
            new Animation<Vector2>(
              overview_center,
              centers.at(0),
              &camera->origin,
              2.0f,
              new EaseOut(),
              nullptr
            ),
            new Animation<float>(
              overview_ratio,
              1.0f,
              &camera->ratio,
              2.0f,
              new EaseOut(),
              on_complete
            )
          }
        ), 1.0f));
  }
};



#endif //LEVELSCENE_H

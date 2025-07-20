#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <iostream>
#include <stack>

#include "Operation.h"
#include "abstract_scene/Scene.h"


class SceneManager {
public:
  SceneManager() {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    scene_texture = LoadRenderTexture(screen_width, screen_height);
    blur_shader = LoadShader(0, "../shaders/blur.frag");

    if (blur_shader.id == 0) {
      TraceLog(LOG_ERROR, "failed to load shader from shaders/blur.frag");
    } else {
      TraceLog(LOG_INFO, "shader id: %d", blur_shader.id);
    }
  }

  void update(float d_t) {
    if (!scenes.empty()) {
      Operation* operation = scenes.top()->update(d_t);
      if (operation) {
        operation->apply(this);
      }
    }
  }

  void draw() {
    std::vector<AbstractScene*> scene_list;
    std::stack<AbstractScene*> temp_scenes = scenes;
    while (!temp_scenes.empty()) {
      scene_list.insert(scene_list.begin(), temp_scenes.top());
      temp_scenes.pop();
    }

    for (int i = 0; i < scene_list.size() - 1; i++) {
      BeginTextureMode(scene_texture);
        ClearBackground(BLANK);
        scene_list[i]->draw();
      EndTextureMode();

      BeginShaderMode(blur_shader);
        DrawTextureRec(
          scene_texture.texture,
          {0, 0, (float) scene_texture.texture.width, -(float) scene_texture.texture.height},
          {0, 0},
          WHITE
        );
      EndShaderMode();
    }

    if (!scenes.empty()) {
      scenes.top()->draw();
    }
  }

  std::stack<AbstractScene*> scenes;
private:
  RenderTexture2D scene_texture;
  Shader blur_shader;
};



#endif //SCENEMANAGER_H

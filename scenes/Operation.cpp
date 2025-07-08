#include "Operation.h"
#include "SceneManager.h"

void PopOperation::apply(SceneManager *scene_manager) {
  if (!scene_manager->scenes.empty()) {
    scene_manager->scenes.pop();
  }
}

void PushOperation::apply(SceneManager *scene_manager) {
  scene_manager->scenes.push(scene);
}

void PopAllOperation::apply(SceneManager *scene_manager) {
  while (!scene_manager->scenes.empty()) {
    scene_manager->scenes.pop();
  }
  scene_manager->scenes.push(scene);
}
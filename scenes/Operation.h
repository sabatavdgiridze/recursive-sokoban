#ifndef OPERATION_H
#define OPERATION_H

class SceneManager;
class AbstractScene;

class Operation {
public:
  virtual ~Operation() = default;
  virtual void apply(SceneManager* scene_manager) = 0;
};

class NoOperation : public Operation {
public:
  void apply(SceneManager *scene_manager) override {

  }
};

class PopOperation : public Operation {
public:
  void apply(SceneManager *scene_manager) override;
};

class PushOperation : public Operation {
public:
  explicit PushOperation(AbstractScene *scene)
    : scene(scene) {
  }

  void apply(SceneManager *scene_manager) override;
private:
  AbstractScene* scene;
};

class PopAllOperation: public Operation {
public:
  explicit PopAllOperation(AbstractScene *scene)
    : scene(scene) {
  }

  void apply(SceneManager *scene_manager) override;

private:
  AbstractScene* scene;
};



#endif //OPERATION_H

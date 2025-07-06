#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <string>
#include <vector>

class Notified {
public:
  virtual ~Notified() = default;
  virtual void notify(std::string message) = 0;
};

class EventBus {
public:
  void add_listener(Notified* listener) {
    if (listener != nullptr) {
      notified_list.push_back(listener);
    }
  }
  void broadcast(std::string message) {
    for (Notified* listener : notified_list) {
      if (listener != nullptr) {
        listener->notify(message);
      }
    }
  }
private:
  std::vector<Notified*> notified_list;
};



#endif //EVENTBUS_H

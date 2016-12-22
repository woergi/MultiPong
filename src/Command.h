#pragma once

#include <functional>
#include <queue>
#include <cassert>

class SceneNode;

enum class Category {
  Scene,
  PlayField,
  PlayerPaddle,
  PlayerObj,
  Ball
};

struct Command {
  Category category;
  int id;
  std::function<void(SceneNode&)> action;
};

class CommandQueue {
  public:
    inline void push(const Command& cmd) { m_queue.push(cmd); }
    inline Command pop() { 
      assert(!isEmpty()); 
      Command c = m_queue.front(); 
      m_queue.pop(); 
      return c; 
    }
    bool isEmpty() { return m_queue.empty(); }

  private:
    std::queue<Command> m_queue;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&)> derivedAction(Function fn) {
  return [=] (SceneNode& node) {
    // Check if cast is safe
    assert(dynamic_cast<GameObject*>(&node) != nullptr);

    // Downcast node and invoke function on it
    fn(static_cast<GameObject&>(node)); };
}


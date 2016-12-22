#pragma once

#include<memory>
#include<vector>
#include<unordered_map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ResourceHolder.h"

class StateStack;
class Player;

enum class PlayfieldSide : short;

enum class StateID : short {
  None,
  World,
  Menu_Main,
  Menu_Pause,
  Menu_GameOver
};

class State {
  public:
    using Ptr = std::unique_ptr<State>;
    struct Context {
      using FontHolder = ResourceHolder<FontID, sf::Font>;

      sf::RenderWindow* window;
      std::vector<std::shared_ptr<Player>>* players;
      FontHolder* fonts;

      uint8_t numBalls;
      PlayfieldSide teamWinSide;
    };

    State(StateStack& stack, Context& ctx) : m_stack(stack), m_ctx(ctx) { }
    virtual ~State() { }

    virtual void draw() = 0;
    virtual bool update() = 0;
    virtual bool handleEvent(const sf::Event& ev) = 0;

  protected:
    void requestStackPush(StateID id);
    void requestStackPop();
    void requestStateClear();

    Context & getContext() const { return m_ctx; }

  private:
    StateStack& m_stack;
    Context& m_ctx;
};

class StateStack {
  public:
    StateStack(StateStack const &) = delete;
    StateStack& operator= (StateStack const &) = delete;

    enum class Action {
      Push,
      Pop,
      Clear
    };

    explicit StateStack(State::Context ctx) : m_ctx(ctx) { }
    template<typename T> void registerState(StateID stateID) {
      m_factory[stateID] = [this] () {
        return State::Ptr(new T(*this, m_ctx));
      };
    }

    void update();
    void draw();
    void handleEvent(const sf::Event& ev);

    void pushState(StateID id);
    void popState();
    void clearStates();

    bool isEmpty() const { return m_stack.empty(); }

  private:
    State::Ptr createState(StateID id);
    void applyPendingChanges();

    struct PendingChange {
      Action action;
      StateID id;
    };

    std::vector<State::Ptr> m_stack;
    std::vector<PendingChange> m_pendingChange;
    State::Context m_ctx;

    std::unordered_map<StateID, std::function<State::Ptr()>> m_factory;
};


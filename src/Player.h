#pragma once
#include <cassert>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <unordered_map>

#include "Command.h"
#include "Scene.h"
#include "Ball.h"
#include "World.h"

namespace sf {
class Event;
}

class CommandQueue;

class Player {
  public:
    enum class Action : char {
      MOVE_UP,
      MOVE_DOWN,

      Count
    };

    Player(int);

    void update(CommandQueue&);
    void processEvent(sf::Event const &);

    inline void assignKey(Action action, sf::Keyboard::Key key) {
      m_keyBinding[key] = action;
    }

    inline sf::Keyboard::Key getAssignedKey(Action action) const {
      for (auto ele : m_keyBinding)
        if (ele.second == action)
          return ele.first;
      return sf::Keyboard::Unknown;
    }

    inline int getPlayerNum() { return m_playerID; }

   private:
    constexpr static uint16_t PlayerSpeed = 400;

    int m_playerID;
    std::unordered_map<sf::Keyboard::Key, Action> m_keyBinding;
    std::unordered_map<Action, Command> m_actionMap;
};

class PlayerPaddle : public SceneNode {
  public:
    PlayerPaddle(int playerID, PlayfieldSide side, sf::Vector2f startPos);

    constexpr static uint16_t Width = 10;
    constexpr static uint16_t Height = Ball::Radius * 4;
    constexpr static uint16_t Padding = 5;

    sf::FloatRect getGlobalBounds() const { return getWorldTransform().transformRect(m_playerPaddle.getGlobalBounds()); }
    PlayfieldSide getPlayfieldSide() const { return m_side; }

    void destroy() { m_destroyed = true; }

  private:
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const override final;
    virtual void updateCurrent();

    sf::RectangleShape m_playerPaddle;
    PlayfieldSide m_side;
    bool m_destroyed;
};


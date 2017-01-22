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

    enum class InputType : char {
      BEGIN,
      Keyboard,
      Network,
      END
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

    inline std::string getInputTypeStr() const {
       switch (m_inputType) {
         case InputType::Keyboard:
           return "Keyboard";
         case InputType::Network:
           return "Network";
      }
      assert(false);
      return "Unknown";
    }

    inline InputType getInputType() const {
      return m_inputType;
    }
    
    inline void togglePrevInputType() {
      m_inputType = static_cast<InputType>(static_cast<char>(m_inputType) - 1  <= static_cast<char>(InputType::BEGIN) ?
        static_cast<char>(InputType::END) - 1 : static_cast<char>(m_inputType) - 1);
    }

    inline void toggleNextInputType() {
      m_inputType = static_cast<InputType>(static_cast<char>(m_inputType) + 1 >= static_cast<char>(InputType::END) ?
        static_cast<char>(InputType::BEGIN) + 1 : static_cast<char>(m_inputType) + 1);
    }

    inline int getPlayerNum() { return m_playerID; }

    inline static void incPlayerSpeed() { 
      PlayerSpeed += PlayerSpeedStepWidth; 
      if (PlayerSpeed > MaxPlayerSpeed)
        PlayerSpeed = MaxPlayerSpeed;
    }
    inline static void decPlayerSpeed() { 
      PlayerSpeed -= PlayerSpeedStepWidth; 
      if (PlayerSpeed <= MinPlayerSpeed)
        PlayerSpeed = MinPlayerSpeed;
    }
    inline static uint8_t getNormalizedPlayerSpeed() { 
      int res = 100. / (MaxPlayerSpeed - MinPlayerSpeed) * (PlayerSpeed-MinPlayerSpeed);
      return res < 0 ? 0 : static_cast<uint8_t>(res);
    }

   private:
    static uint16_t PlayerSpeed;
    static constexpr uint16_t MinPlayerSpeed = 20;
    static constexpr uint16_t MaxPlayerSpeed = 480;
    static constexpr uint16_t PlayerSpeedStepWidth = (MaxPlayerSpeed-MinPlayerSpeed)/100;

    int m_playerID;
    std::unordered_map<sf::Keyboard::Key, Action> m_keyBinding;
    std::unordered_map<Action, Command> m_actionMap;
    InputType m_inputType;
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
    bool isDesroyed() { return m_destroyed; }

  private:
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const override final;
    virtual void updateCurrent();

    sf::RectangleShape m_playerPaddle;
    PlayfieldSide m_side;
    bool m_destroyed;
};


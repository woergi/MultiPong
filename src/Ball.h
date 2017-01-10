#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.h"

class PlayerPaddle;

class Ball : public SceneNode {
  public:
    Ball(int id);
    virtual ~Ball() { }

    static constexpr uint16_t Radius = 10;
    static uint16_t Speed;
    static constexpr uint16_t MinBallSpeed = 20.f;
    static constexpr uint16_t MaxBallSpeed = 480.f;

    bool collideWithPaddle(const PlayerPaddle* paddle);

    sf::FloatRect getGlobalBounds() const;

    inline void destroy() { m_destroyed = true; }
    inline bool isDestroyed() { return m_destroyed; }

    inline static void incBallSpeed() { 
      Speed += 20; 
      if (Speed > MaxBallSpeed)
        Speed = MaxBallSpeed;
    }
    inline static void decBallSpeed() { 
      Speed -= 20; 
      if (Speed <= MinBallSpeed)
        Speed = MinBallSpeed;
    }
    inline static uint8_t getNormalizedBallSpeed() { 
      int res = 100. / (MaxBallSpeed - MinBallSpeed) * (Speed-MinBallSpeed);
      return res < 0 ? 0 : static_cast<uint8_t>(res);
    }
  private:
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const override final;
    virtual void updateCurrent();

    sf::CircleShape m_ball;
    float m_ballAngle;
    bool m_destroyed;
};


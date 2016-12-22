#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.h"

class PlayerPaddle;

class Ball : public SceneNode {
  public:
    Ball(int id);
    virtual ~Ball() { }

    static constexpr uint16_t Radius = 10;
    static constexpr uint16_t Speed = 400.f;

    bool collideWithPaddle(const PlayerPaddle* paddle);

    sf::FloatRect getGlobalBounds() const;

    inline void destroy() { m_destroyed = true; }
    inline bool isDestroyed() { return m_destroyed; }

  private:
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const override final;
    virtual void updateCurrent();

    sf::CircleShape m_ball;
    float m_ballAngle;
    bool m_destroyed;
};


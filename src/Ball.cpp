
#include "Ball.h"
#include "World.h"
#include "Utils.h"
#include "Player.h"

#include <cmath>

Ball::Ball(int id) : SceneNode(Category::Ball, id), m_destroyed(false) {
  m_ball.setRadius(Radius - 3);
  m_ball.setOutlineThickness(3);
  m_ball.setOutlineColor(sf::Color::Black);
  m_ball.setFillColor(sf::Color::White);
  m_ball.setOrigin(Radius / 2, Radius / 2);
  m_ball.setPosition(World::PlayfieldWidth/2.f, World::PlayfieldHeight/2.f);
  m_ballAngle = 0.f;
  do {
    // Make sure the ball initial angle is not too much vertical
    m_ballAngle = (std::rand() % 360) * 2 * PI / 360;
  } while (std::abs(std::cos(m_ballAngle)) < 0.7f);
}

void Ball::drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const {
  if (!m_destroyed)
    target.draw(m_ball, states);
}

void Ball::updateCurrent() {
  float factor = Speed * World::TimePerFrame.asSeconds();
  m_ball.move(std::cos(m_ballAngle) * factor, std::sin(m_ballAngle) * factor);

  if (m_ball.getPosition().y < 0) {
    m_ballAngle = -m_ballAngle;
    m_ball.setPosition(m_ball.getPosition().x, Radius + 0.1f);
  }
  else if (m_ball.getPosition().y >= World::PlayfieldHeight) {
    m_ballAngle = -m_ballAngle;
    m_ball.setPosition(m_ball.getPosition().x, m_ball.getPosition().y-Radius - 0.1f);
  }
}

sf::FloatRect Ball::getGlobalBounds() const { 
  return getWorldTransform().transformRect(m_ball.getGlobalBounds()); 
}

bool Ball::collideWithPaddle(const PlayerPaddle* paddle) {
  if (paddle->getGlobalBounds().intersects(getGlobalBounds())) {
    m_ballAngle = PI - m_ballAngle - std::rand() % 20 * PI / 180;

    if (paddle->getPlayfieldSide() == PlayfieldSide::Left) {
      m_ball.setPosition(2*Radius + 0.1f, m_ball.getPosition().y);
    }
    else if (paddle->getPlayfieldSide() == PlayfieldSide::Right) {
      m_ball.setPosition(World::PlayfieldWidth - 2*Radius - 0.1f,
                         m_ball.getPosition().y);
    }
    return true;
  }
  return false;
}


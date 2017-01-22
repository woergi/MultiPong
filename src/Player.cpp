#include "Player.h"
#include "World.h"
#include "Scene.h"

#include <cassert>

uint16_t Player::PlayerSpeed = (Player::MaxPlayerSpeed-Player::MinPlayerSpeed) / 2 + Player::MinPlayerSpeed;

Player::Player(int playerID) : m_playerID(playerID), m_inputType(InputType::Keyboard) {
  auto moveLambda = [](float speed, PlayerPaddle& p)
      {p.move(sf::Vector2f(0.f, speed) * World::TimePerFrame.asSeconds());};

  m_actionMap[Action::MOVE_DOWN] = Command{Category::PlayerPaddle, m_playerID,
    derivedAction<PlayerPaddle>([=](PlayerPaddle& p) { moveLambda(PlayerSpeed, p); }) };
  m_actionMap[Action::MOVE_UP] = Command{Category::PlayerPaddle, m_playerID,
    derivedAction<PlayerPaddle>([=](PlayerPaddle& p) { moveLambda(-PlayerSpeed, p); }) };
}

void Player::update(CommandQueue& cmdQueue) {
  for (auto binding : m_keyBinding) {
    assert(m_actionMap.find(binding.second) != m_actionMap.end());

    if (sf::Keyboard::isKeyPressed(binding.first)) {
      cmdQueue.push(m_actionMap[binding.second]);
    }
  }
}

void Player::processEvent(sf::Event const & event) {
}

PlayerPaddle::PlayerPaddle(int playerID, PlayfieldSide side, sf::Vector2f startPos) 
: SceneNode(Category::PlayerPaddle, playerID), m_side(side), m_destroyed(false) {
  sf::Vector2f paddleSize(Width, Height);
  m_playerPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
  m_playerPaddle.setOutlineThickness(1);
  m_playerPaddle.setOutlineColor(sf::Color::Red);
  m_playerPaddle.setFillColor(sf::Color(200, 50, 100));
  this->move(startPos.x, startPos.y);
}

void PlayerPaddle::drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const {
  if (!m_destroyed)
    target.draw(m_playerPaddle, states);
}

void PlayerPaddle::updateCurrent() {
  if (getPosition().y < 0)
    setPosition(getPosition().x, 0);
  if (getPosition().y + Height > World::PlayfieldHeight)
    setPosition(getPosition().x, World::PlayfieldHeight-Height);
}


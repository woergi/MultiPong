
#include "Menu.h"
#include "World.h"

Menu::Menu(StateStack& stack, Context& ctx) : State(stack, ctx) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  m_text.setFont(getContext().fonts->get(FontID::Sansation));
  m_text.setCharacterSize(32);
  m_text.setString("Press any key to start");
  sf::FloatRect textRect = m_text.getLocalBounds();
  m_text.setOrigin(textRect.left + textRect.width/2.0f,
                 textRect.top  + textRect.height/2.0f);
  m_text.setPosition(m_background.getPosition().x + m_background.getSize().x / 2,
      m_background.getPosition().y + m_background.getSize().y / 2);
}

void Menu::draw() {
  getContext().window->draw(m_background);
  getContext().window->draw(m_text);
}

bool Menu::update() {
  return false;
}

bool Menu::handleEvent(const sf::Event& ev) {
  if (ev.key.code == sf::Keyboard::Escape) {
    getContext().window->close();
  }

  if (ev.type == sf::Event::KeyPressed) {
    requestStackPop();
    return true;
  }
  return false;
}

Pause::Pause(StateStack& stack, Context& ctx) 
  : State(stack, ctx), m_showText(true) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  m_text.setFont(getContext().fonts->get(FontID::Sansation));
  m_text.setCharacterSize(32);
  m_text.setString("Pause");
  sf::FloatRect textRect = m_text.getLocalBounds();
  m_text.setOrigin(textRect.left + textRect.width/2.0f,
                 textRect.top  + textRect.height/2.0f);
  m_text.setPosition(m_background.getPosition().x + m_background.getSize().x / 2,
      m_background.getPosition().y + m_background.getSize().y / 2);
}

void Pause::draw() {
  getContext().window->draw(m_background);
  if (m_showText)
    getContext().window->draw(m_text);
}

bool Pause::update() {
  if (m_textEffectTime >= sf::seconds(0.5f)) {
    m_showText = !m_showText;
    m_textEffectTime = sf::Time::Zero;
  }
  else {
    m_textEffectTime += World::TimePerFrame;
  }

  return false;
}

bool Pause::handleEvent(const sf::Event& ev) {
  if (ev.key.code == sf::Keyboard::Escape) {
    getContext().window->close();
  }

  if (ev.type == sf::Event::KeyPressed) {
    requestStackPop();
    return true;
  }
  return false;
}

GameOver::GameOver(StateStack& stack, Context& ctx) : State(stack, ctx) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  std::string msg;
  if (ctx.teamWinSide == PlayfieldSide::None)
    msg = "Ran out of balls ... So nobody won :D"
          "\n Press any key to start a new game";
  else
    msg = "Team <" + std::to_string(static_cast<short>(ctx.teamWinSide)) 
          + "> wins\n Press any key to start a new game";
  m_text.setFont(getContext().fonts->get(FontID::Sansation));
  m_text.setCharacterSize(32);
  m_text.setString(msg);
  sf::FloatRect textRect = m_text.getLocalBounds();
  m_text.setOrigin(textRect.left + textRect.width/2.0f,
                 textRect.top  + textRect.height/2.0f);
  m_text.setPosition(m_background.getPosition().x + m_background.getSize().x / 2,
      m_background.getPosition().y + m_background.getSize().y / 2);
}

void GameOver::draw() {
  getContext().window->draw(m_background);
  getContext().window->draw(m_text);
}

bool GameOver::update() {
  return false;
}

bool GameOver::handleEvent(const sf::Event& ev) {
  if (ev.key.code == sf::Keyboard::Escape) {
    getContext().window->close();
  }

  if (ev.type == sf::Event::KeyPressed) {
    requestStateClear();
    requestStackPush(StateID::World);
    return true;
  }
  return false;
}



#include "Menu.h"
#include "World.h"
#include "Player.h"
#include "MusicPlayer.h"

auto numPlayerEntry = [](const State::Context& ctx) { return "Number of Players:  < " + std::to_string(ctx.players->size()) + " >"; };
auto selPlayerEntry = [](int selPlayer) { return "Player < " + std::to_string(selPlayer) + " >"; };
auto selInputTypeEntry = [](const std::shared_ptr<Player>& player) { return "InputType < " + player->getInputTypeStr() + " >"; };
auto numBallEntry = [](const State::Context& ctx) { return "Number of Balls:  < " + std::to_string(ctx.numBalls) + " >"; };
auto paddleSpeedEntry = [](const State::Context& ctx) { return "Paddle Speed: < " + std::to_string(Player::getNormalizedPlayerSpeed()) + "% >"; };
auto ballSpeedEntry = [](const State::Context& ctx) { return "Ball Speed: < " + std::to_string(Ball::getNormalizedBallSpeed()) + "% >"; };

auto playerKey = [](const std::shared_ptr<Player>& player, Player::Action action) { 
  sf::Keyboard::Key key = player->getAssignedKey(action);
  if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
    return static_cast<char>('A' + key);
  return static_cast<char>(' ');
};

Menu_Main::Menu_Main(StateStack& stack, Context& ctx) : State(stack, ctx), m_selState(SelectedElement::Start) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  sf::Text txt;

  txt.setFont(getContext().fonts->get(FontID::Sansation));
  txt.setCharacterSize(32);
  sf::FloatRect textRect = txt.getLocalBounds();
  txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
  txt.setPosition(m_background.getPosition().x + 32,
      m_background.getPosition().y + m_background.getSize().y / 2);

  txt.setString("Start Game");
  m_menuEntries[SelectedElement::Start] = txt;

  txt.setString("Quit");
  m_menuEntries[SelectedElement::Quit] = txt;

  txt.setString("Options");
  m_menuEntries[SelectedElement::Options] = txt;

  int entryCnt = 0;
  for (auto & element : m_menuEntries) {
    element.second.move(0, 48 * entryCnt++);
  }
}

void Menu_Main::draw() {
  getContext().window->draw(m_background);
  for (auto & element : m_menuEntries) {
    if (element.first == m_selState)
      element.second.setFillColor(sf::Color::Red);
    else
      element.second.setFillColor(sf::Color::White);
    getContext().window->draw(element.second);
  }
}

bool Menu_Main::update() {
  return false;
}

bool Menu_Main::handleEvent(const sf::Event& ev) {
  if (ev.type == sf::Event::KeyPressed) {
    if (ev.key.code == sf::Keyboard::Down) {
    unsigned nextVal = static_cast<unsigned>(m_selState) + 1;
    if (nextVal >= static_cast<unsigned>(SelectedElement::END))
      m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::BEGIN) + 1);
    else
      m_selState = static_cast<SelectedElement>(nextVal);
    }
    else if (ev.key.code == sf::Keyboard::Up) {
      unsigned prevVal = static_cast<unsigned>(m_selState) - 1;
      if (prevVal == static_cast<unsigned>(SelectedElement::BEGIN))
        m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::END) - 1);
      else
        m_selState = static_cast<SelectedElement>(prevVal);
    }
    else if (ev.key.code == sf::Keyboard::Return) {
      switch (m_selState) {
        case SelectedElement::Start:
          requestStateClear();
          requestStackPush(StateID::World);
          getContext().musicPlayer->play(Music::GameTheme);
          return true;
        case SelectedElement::Options:
          requestStackPop();
          requestStackPush(StateID::Menu_Options);
          break;
        case SelectedElement::Quit:
          getContext().window->close();
          break;
      }
    }
  }

  return false;
}

Menu_Options::Menu_Options(StateStack& stack, Context& ctx) : State(stack, ctx), m_selState(SelectedElement::PlayerNum) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  sf::Text txt;

  txt.setFont(getContext().fonts->get(FontID::Sansation));
  txt.setCharacterSize(32);
  sf::FloatRect textRect = txt.getLocalBounds();
  txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
  txt.setPosition(m_background.getPosition().x + 32,
      m_background.getPosition().y + m_background.getSize().y / 2);

  txt.setString(numPlayerEntry(getContext()));
  m_menuEntries[SelectedElement::PlayerNum] = txt;

  txt.setString(numBallEntry(getContext()));
  m_menuEntries[SelectedElement::BallNum] = txt;

  txt.setString(paddleSpeedEntry(getContext()));
  m_menuEntries[SelectedElement::PaddleSpeed] = txt;

  txt.setString(ballSpeedEntry(getContext()));
  m_menuEntries[SelectedElement::BallSpeed] = txt;

  txt.setString("Controls");
  m_menuEntries[SelectedElement::Controls] = txt;

  int entryCnt = 0;
  for (auto & element : m_menuEntries) {
    element.second.move(0, 48 * entryCnt++);
  }
}

void Menu_Options::draw() {
  getContext().window->draw(m_background);
  for (auto & element : m_menuEntries) {
    if (element.first == m_selState)
      element.second.setFillColor(sf::Color::Red);
    else
      element.second.setFillColor(sf::Color::White);
    getContext().window->draw(element.second);
  }
}

bool Menu_Options::update() {
  return false;
}

bool Menu_Options::handleEvent(const sf::Event& ev) {
  if (ev.type == sf::Event::KeyPressed) {
    if (ev.key.code == sf::Keyboard::Down) {
    unsigned nextVal = static_cast<unsigned>(m_selState) + 1;
    if (nextVal >= static_cast<unsigned>(SelectedElement::END))
      m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::BEGIN) + 1);
    else
      m_selState = static_cast<SelectedElement>(nextVal);
    }
    else if (ev.key.code == sf::Keyboard::Up) {
      unsigned prevVal = static_cast<unsigned>(m_selState) - 1;
      if (prevVal == static_cast<unsigned>(SelectedElement::BEGIN))
        m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::END) - 1);
      else
        m_selState = static_cast<SelectedElement>(prevVal);
    }
    else if (ev.key.code == sf::Keyboard::Left) {
      if (m_selState == SelectedElement::PlayerNum && getContext().players->size() > 2) {
        getContext().players->pop_back();
        m_menuEntries[SelectedElement::PlayerNum].setString(numPlayerEntry(getContext()));
      }
      else if (m_selState == SelectedElement::BallNum && getContext().numBalls > 1) {
        --getContext().numBalls;
        m_menuEntries[SelectedElement::BallNum].setString(numBallEntry(getContext()));
      }
      else if (m_selState == SelectedElement::PaddleSpeed) {
        Player::decPlayerSpeed();
        m_menuEntries[SelectedElement::PaddleSpeed].setString(paddleSpeedEntry(getContext()));
      }
      else if (m_selState == SelectedElement::BallSpeed) {
        Ball::decBallSpeed();
        m_menuEntries[SelectedElement::BallSpeed].setString(ballSpeedEntry(getContext()));
      }
    }
    else if (ev.key.code == sf::Keyboard::Right) {
      if (m_selState == SelectedElement::PlayerNum) {
        getContext().players->emplace_back(std::shared_ptr<Player>(new Player(getContext().players->size()+1)));
        m_menuEntries[SelectedElement::PlayerNum].setString(numPlayerEntry(getContext()));
      }
      else if (m_selState == SelectedElement::BallNum) {
        ++getContext().numBalls;
        m_menuEntries[SelectedElement::BallNum].setString(numBallEntry(getContext()));
      }
      else if (m_selState == SelectedElement::PaddleSpeed) {
        Player::incPlayerSpeed();
        m_menuEntries[SelectedElement::PaddleSpeed].setString(paddleSpeedEntry(getContext()));
      }
      else if (m_selState == SelectedElement::BallSpeed) {
        Ball::incBallSpeed();
        m_menuEntries[SelectedElement::BallSpeed].setString(ballSpeedEntry(getContext()));
      }
    }
    else if (ev.key.code == sf::Keyboard::Return) {
      switch (m_selState) {
        case SelectedElement::Controls:
          requestStackPop();
          requestStackPush(StateID::Menu_Controls);
          break;
      }
    }
    else if (ev.key.code == sf::Keyboard::Escape) {
      requestStackPop();
      requestStackPush(StateID::Menu_Main);
    }
  }

  return false;
}

Menu_Controls::Menu_Controls(StateStack& stack, Context& ctx) : State(stack, ctx), m_selState(SelectedElement::PlayerNum), m_selPlayer(0) {
  m_background.setSize(sf::Vector2f(World::PlayfieldWidth, World::PlayfieldHeight));
  m_background.setOutlineThickness(3);
  m_background.setOutlineColor(sf::Color::Blue);
  m_background.setFillColor(sf::Color(0, 0, 0, 150));
  m_background.setPosition(World::HPadding, World::HeaderHeight);

  sf::Text txt;

  txt.setFont(getContext().fonts->get(FontID::Sansation));
  txt.setCharacterSize(32);
  sf::FloatRect textRect = txt.getLocalBounds();
  txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
  txt.setPosition(m_background.getPosition().x + 32,
      m_background.getPosition().y + m_background.getSize().y / 2);

  txt.setString(selPlayerEntry(m_selPlayer));
  m_menuEntries[SelectedElement::PlayerNum] = txt;

  txt.setString(selInputTypeEntry((*ctx.players)[m_selPlayer]));
  m_menuEntries[SelectedElement::InputType] = txt;

  txt.setString(std::string("Up: ") + playerKey((*ctx.players)[m_selPlayer], Player::Action::MOVE_UP));
  m_keyboardEntries[KeyboardElement::MoveUp] = txt;
  txt.setString(std::string("Down: ") + playerKey((*ctx.players)[m_selPlayer], Player::Action::MOVE_DOWN));
  m_keyboardEntries[KeyboardElement::MoveDown] = txt;

  txt.setString(" ");
  m_networkEntries[NetworkElement::Ip] = txt;
  m_networkEntries[NetworkElement::Port] = txt;

  int entryCnt = 0;
  for (auto & element : m_menuEntries) {
    element.second.move(0, 48 * entryCnt++);
  }

  int subEntryCnt = 0;
  for (auto & element : m_keyboardEntries) {
    element.second.move(0, 48 * (entryCnt + subEntryCnt++));
  }
  subEntryCnt = 0;
  for (auto & element : m_networkEntries) {
    element.second.move(0, 48 * (entryCnt + subEntryCnt++));
  }
}

void Menu_Controls::draw() {
  getContext().window->draw(m_background);
  for (auto & element : m_menuEntries) {
    if (element.first == m_selState)
      element.second.setFillColor(sf::Color::Red);
    else
      element.second.setFillColor(sf::Color::White);
    getContext().window->draw(element.second);
  }

  switch ((*getContext().players)[m_selPlayer]->getInputType()) {
    case Player::InputType::Keyboard:
      for (auto & element : m_keyboardEntries) {
        getContext().window->draw(element.second);
      }
      break;
    case Player::InputType::Network:
      for (auto & element : m_networkEntries) {
        getContext().window->draw(element.second);
      }
      break;
  }
}

bool Menu_Controls::update() {
  return false;
}

bool Menu_Controls::handleEvent(const sf::Event& ev) {
  if (ev.type == sf::Event::KeyPressed) {
    if (ev.key.code == sf::Keyboard::Down) {
    unsigned nextVal = static_cast<unsigned>(m_selState) + 1;
    if (nextVal >= static_cast<unsigned>(SelectedElement::END))
      m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::BEGIN) + 1);
    else
      m_selState = static_cast<SelectedElement>(nextVal);
    }
    else if (ev.key.code == sf::Keyboard::Up) {
      unsigned prevVal = static_cast<unsigned>(m_selState) - 1;
      if (prevVal == static_cast<unsigned>(SelectedElement::BEGIN))
        m_selState = static_cast<SelectedElement>(static_cast<unsigned>(SelectedElement::END) - 1);
      else
        m_selState = static_cast<SelectedElement>(prevVal);
    }
    else if (ev.key.code == sf::Keyboard::Left) {
      if (m_selState == SelectedElement::PlayerNum) {
        m_selPlayer = --m_selPlayer < 0 ? getContext().players->size() - 1 : m_selPlayer;
        m_menuEntries[SelectedElement::PlayerNum].setString(selPlayerEntry(m_selPlayer));
      }
      else if (m_selState == SelectedElement::InputType) {
        (*getContext().players)[m_selPlayer]->togglePrevInputType();
      }

    }
    else if (ev.key.code == sf::Keyboard::Right) {
      if (m_selState == SelectedElement::PlayerNum) {
        m_selPlayer = ++m_selPlayer % getContext().players->size();
        m_menuEntries[SelectedElement::PlayerNum].setString(selPlayerEntry(m_selPlayer));
      }
      else if (m_selState == SelectedElement::InputType) {
        (*getContext().players)[m_selPlayer]->toggleNextInputType();
      }
    }
    else if (ev.key.code == sf::Keyboard::Return) {
      //TODO new key mapping layer / dependent on input type
    }
    else if (ev.key.code == sf::Keyboard::Escape) {
      requestStackPop();
      requestStackPush(StateID::Menu_Options);
    }
  }

  m_menuEntries[SelectedElement::InputType].setString(selInputTypeEntry((*getContext().players)[m_selPlayer]));
  m_keyboardEntries[KeyboardElement::MoveUp].setString(std::string("Up: ") + playerKey((*getContext().players)[m_selPlayer], Player::Action::MOVE_UP));
  m_keyboardEntries[KeyboardElement::MoveDown].setString(std::string("Down: ") + playerKey((*getContext().players)[m_selPlayer], Player::Action::MOVE_DOWN));

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
    requestStateClear();
    requestStackPush(StateID::World);
    requestStackPush(StateID::Menu_Main);
    getContext().musicPlayer->stop();
  }
  else {
    if (ev.type == sf::Event::KeyPressed) {
      requestStackPop();
      getContext().musicPlayer->setPaused(false);
      return true;
    }
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
    msg = "Ran out of balls ... So nobody won :D";
  else
    msg = "Team <" + std::to_string(static_cast<short>(ctx.teamWinSide))  + "> wins";
  msg += "\n Press <Return> to start a new game";
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
    requestStateClear();
    requestStackPush(StateID::World);
    requestStackPush(StateID::Menu_Main);
  }
  else if (ev.key.code == sf::Keyboard::Return) {
    requestStateClear();
    requestStackPush(StateID::World);
    getContext().musicPlayer->play(Music::GameTheme);
    return true;
  }
  return false;
}


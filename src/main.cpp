
#include "World.h"
#include "Player.h"
#include "StateStack.h"
#include "Menu.h"

#include <memory>
#include <SFML/Window.hpp>

void processEvent(sf::RenderWindow& window, StateStack& states) {
    sf::Event event;
    while (window.pollEvent(event)) {
      states.handleEvent(event);
    }
}

int main(int argc, char* argv[]) {
  sf::RenderWindow window(sf::VideoMode(World::Width, World::Height), "MultiPong");
  std::vector<std::shared_ptr<Player>> players;
  ResourceHolder<FontID, sf::Font> m_resFont;

  for (int playerNum = 1; playerNum <= 4; ++playerNum)
    players.emplace_back(std::shared_ptr<Player>(new Player(playerNum)));

  players[0]->assignKey(Player::Action::MOVE_DOWN, sf::Keyboard::A);
  players[0]->assignKey(Player::Action::MOVE_UP, sf::Keyboard::S);
  players[2]->assignKey(Player::Action::MOVE_DOWN, sf::Keyboard::F);
  players[2]->assignKey(Player::Action::MOVE_UP, sf::Keyboard::D);

  players[1]->assignKey(Player::Action::MOVE_DOWN, sf::Keyboard::J);
  players[1]->assignKey(Player::Action::MOVE_UP, sf::Keyboard::K);
  players[3]->assignKey(Player::Action::MOVE_DOWN, sf::Keyboard::L);
  players[3]->assignKey(Player::Action::MOVE_UP, sf::Keyboard::O);

  m_resFont.load(FontID::Sansation, "resources/sansation.ttf");

  State::Context ctx = {&window, &players, &m_resFont, 0};
  StateStack states(ctx);
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  ctx.teamWinSide = PlayfieldSide::None;

  states.registerState<Menu>(StateID::Menu_Main);
  states.registerState<Pause>(StateID::Menu_Pause);
  states.registerState<GameOver>(StateID::Menu_GameOver);
  states.registerState<World>(StateID::World);

  states.pushState(StateID::World);
  states.pushState(StateID::Menu_Main);

  while (window.isOpen()) {
    processEvent(window, states);
    timeSinceLastUpdate += clock.restart();
    while (timeSinceLastUpdate > World::TimePerFrame) {
      timeSinceLastUpdate -= World::TimePerFrame;
      processEvent(window, states);
      states.update();
    }

    states.draw();
  }

  return 0;
}

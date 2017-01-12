
#include "World.h"
#include "Player.h"
#include "Ball.h"
#include "MusicPlayer.h"

#include <SFML/Window.hpp>
#include <iostream>

const sf::Time World::TimePerFrame = sf::seconds(1.f / 60.f);

World::World(StateStack& stack, Context& ctx) :  State(stack, ctx) {
  std::srand(std::time(0));

  SceneNode::Ptr backgroundLayer(new SceneNode(Category::Scene));
  m_sceneLayers[static_cast<short>(SceneLayer::Background)] = backgroundLayer.get();
  m_sceneGraph.attachChild(std::move(backgroundLayer));

  sf::Text headerMsg;
  headerMsg.setFont(getContext().fonts->get(FontID::Sansation));
  headerMsg.setCharacterSize(32);
  headerMsg.setString("Multi-Pong");
  headerMsg.setPosition(HPadding + HeaderWidth / 2.f - 70, HeaderHeight / 2.f - 24);
  getLayerRoot(SceneLayer::Background)->attachChild(
      SceneNode::Ptr(new StaticSceneElement<sf::Text>(std::move(headerMsg))));

  sf::RectangleShape playfield;
  playfield.setSize(sf::Vector2f(PlayfieldWidth, PlayfieldHeight));
  playfield.setOutlineThickness(3);
  playfield.setOutlineColor(sf::Color::Blue);
  playfield.setFillColor(sf::Color::Transparent);
  SceneNode::Ptr playFieldLayer(new StaticSceneElement<sf::RectangleShape>(std::move(playfield)));
  playFieldLayer->move(HPadding, HeaderHeight);
  m_sceneLayers[static_cast<short>(SceneLayer::PlayField)] = playFieldLayer.get();
  m_sceneGraph.attachChild(std::move(playFieldLayer));

  ctx.teamWinSide = PlayfieldSide::None;

  updatePlayer();
  for (int i = getContext().numBalls; i > 0; --i) {
    SceneNode::Ptr ball(new Ball(i));
    m_balls.push_back(static_cast<Ball*>(ball.get()));
    getLayerRoot(SceneLayer::PlayField)->attachChild(std::move(ball));
  }
}

void World::updatePlayer() {
  size_t numPlayers = 0;
  for (const auto p : *getContext().players) {
    sf::Vector2f playerPos;
    playerPos.x = PlayerPaddle::Padding + (PlayerPaddle::Padding + PlayerPaddle::Width) * numPlayers / 2;
    if (numPlayers % 2) {
      playerPos.x = PlayfieldWidth - playerPos.x;
    }
    playerPos.y = std::rand() % (PlayfieldHeight - PlayerPaddle::Height);
    PlayfieldSide side = numPlayers % 2 ? PlayfieldSide::Right : PlayfieldSide::Left;
    SceneNode::Ptr playerPaddle(new PlayerPaddle(p->getPlayerNum(), side, playerPos));
    m_palyerPaddles.push_back(static_cast<PlayerPaddle*>(playerPaddle.get()));
    getLayerRoot(SceneLayer::PlayField)->attachChild(std::move(playerPaddle));
    m_alivePlayers[side].insert(p->getPlayerNum());

    sf::Text playerLabel;
    playerLabel.setFont(getContext().fonts->get(FontID::Sansation));
    playerLabel.setCharacterSize(18);
    playerLabel.setString(std::to_string(p->getPlayerNum()));
    playerLabel.setPosition(HPadding + playerPos.x, HeaderHeight - 36);
    getLayerRoot(SceneLayer::Background)->attachChild(
        SceneNode::Ptr(new StaticSceneElement<sf::Text>(std::move(playerLabel))));

    ++numPlayers;
  }
}

bool World::handleEvent(const sf::Event& event) {
  switch (event.type) {
    case sf::Event::Closed:
      getContext().window->close();
      break;
    case sf::Event::KeyPressed:
      if (event.key.code == sf::Keyboard::Escape) {
        requestStateClear();
        requestStackPush(StateID::Menu_Main);
        getContext().musicPlayer->stop();
      }
      else if (event.key.code == sf::Keyboard::Space) {
        requestStackPush(StateID::Menu_Pause);
        getContext().musicPlayer->setPaused(true);
      }
      break;
  }

  for (auto const & p : *getContext().players)
    p->processEvent(event);
  return true;
}

bool World::update() {
  for (auto const & p : *getContext().players)
    p->update(m_cmdQueue);

  while(!m_cmdQueue.isEmpty())
    m_sceneGraph.onCommand(m_cmdQueue.pop());

  m_sceneGraph.update();

  for (std::vector<Ball*>::iterator it = m_balls.begin(); it != m_balls.end(); )
    (*it)->isDestroyed() ? it = m_balls.erase(it) : ++it;

  handleBallCollissions();
  checkDrawn();
  return true;
}

void World::draw() {
  getContext().window->draw(m_sceneGraph);
}

void World::handleBallCollissions() {
  Command rmPlayerPaddleCmd{Category::PlayerPaddle, 0,
    derivedAction<PlayerPaddle>([=](PlayerPaddle& p) { p.destroy(); }) };

  for (auto b : m_balls) {
    bool collideWitPlayer = false;
    Command rmBallCmd{Category::Ball, b->getID(),
      derivedAction<Ball>([=](Ball& b) { b.destroy(); }) };

    for (auto p : m_palyerPaddles) {
      if (!p->isDesroyed() && b->collideWithPaddle(p)) {
        collideWitPlayer = true;
        break;
      }
    }
    if (collideWitPlayer)
      continue;

    if (b->getGlobalBounds().left <= 0) {
      //Kill player on the left side
      auto& players = m_alivePlayers[PlayfieldSide::Left];
      if (!players.empty()) {
        auto playerIt = players.begin();
        std::advance(playerIt, std::rand() % players.size());
        rmPlayerPaddleCmd.id = *playerIt;
        m_cmdQueue.push(rmPlayerPaddleCmd);
        players.erase(playerIt);
      }
      if (players.empty())
        getContext().teamWinSide = PlayfieldSide::Right;

      m_cmdQueue.push(rmBallCmd);
    }
    else if (b->getGlobalBounds().left+b->getGlobalBounds().width >= Width) {
      //Kill player on the right side
      auto& players = m_alivePlayers[PlayfieldSide::Right];
      if (!players.empty()) {
        auto playerIt = players.begin();
        std::advance(playerIt, std::rand() % players.size());
        rmPlayerPaddleCmd.id = *playerIt;
        m_cmdQueue.push(rmPlayerPaddleCmd);
        players.erase(playerIt);
      }
      if (players.empty())
        getContext().teamWinSide = PlayfieldSide::Left;

      m_cmdQueue.push(rmBallCmd);
    }

    if (getContext().teamWinSide != PlayfieldSide::None) {
      getContext().musicPlayer->stop();
      requestStackPush(StateID::Menu_GameOver);
    }
  }
}

void World::checkDrawn() {
  if (m_balls.empty() 
      && !m_alivePlayers[PlayfieldSide::Left].empty()
      && !m_alivePlayers[PlayfieldSide::Right].empty()) {
    getContext().musicPlayer->stop();
    requestStackPush(StateID::Menu_GameOver);
  }
}


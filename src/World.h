#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "ResourceHolder.h"
#include "Scene.h"
#include "StateStack.h"

class Ball;
class PlayerPaddle;

enum class PlayfieldSide : short {
  Left,
  Right,
  Top,
  Bottom, 
  None
};

class World : public State {
  public:
    World(StateStack& stack, Context& ctx);

    World(const World &) = delete;
    World& operator= (const World&) = delete;

    static const sf::Time TimePerFrame;
    static constexpr uint16_t Height = 768;
    static constexpr uint16_t Width = 1024;
    static constexpr uint16_t HPadding = 10;
    static constexpr uint16_t HeaderHeight = 100;
    static constexpr uint16_t HeaderWidth = Width - 2*HPadding;
    static constexpr uint16_t FooterHeight = 10;
    static constexpr uint16_t FooterWidth = Width - 2*HPadding;
    static constexpr uint16_t PlayfieldHeight = Height - HeaderHeight - FooterHeight;
    static constexpr uint16_t PlayfieldWidth = Width - 2*HPadding;

  private:
    virtual bool handleEvent(const sf::Event&) override final;
    virtual bool update() override final;
    virtual void draw() override final;

    inline SceneNode* getLayerRoot(SceneLayer layer) { return m_sceneLayers[static_cast<short>(layer)]; }

    void updatePlayer();
    void handleBallCollissions();
    void checkDrawn();

    SceneNode m_sceneGraph;
    std::array<SceneNode*, static_cast<short>(SceneLayer::LayerCount)> m_sceneLayers;

    CommandQueue m_cmdQueue;

    std::unordered_map<PlayfieldSide, std::set<int>> m_alivePlayers;
    std::vector<Ball*> m_balls;
    std::vector<PlayerPaddle*> m_palyerPaddles;
};


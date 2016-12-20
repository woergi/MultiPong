#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "StateStack.h"

class Menu : public State {
  public:
    Menu(StateStack& stack, Context& ctx);
    virtual ~Menu() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    sf::RectangleShape m_background;
    sf::Text m_text;
};

class Pause : public State {
  public:
    Pause(StateStack& stack, Context& ctx);
    virtual ~Pause() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    sf::RectangleShape m_background;
    sf::Text m_text;
    bool m_showText;
    sf::Time m_textEffectTime;
};

class GameOver : public State {
  public:
    GameOver(StateStack& stack, Context& ctx);
    virtual ~GameOver() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    sf::RectangleShape m_background;
    sf::Text m_text;
};

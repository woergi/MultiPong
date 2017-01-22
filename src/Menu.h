#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "StateStack.h"

class Menu_Main : public State {
  public:
    Menu_Main(StateStack& stack, Context& ctx);
    virtual ~Menu_Main() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    enum class SelectedElement {
      BEGIN = 0,

      Start,
      Options,
      Quit,

      END
    };

    sf::RectangleShape m_background;

    SelectedElement m_selState;
    std::map<SelectedElement, sf::Text> m_menuEntries;
};

class Menu_Options : public State {
  public:
    Menu_Options(StateStack& stack, Context& ctx);
    virtual ~Menu_Options() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    enum class SelectedElement {
      BEGIN = 0,

      PlayerNum,
      BallNum,
      PaddleSpeed,
      BallSpeed,
      Controls,

      END
    };

    sf::RectangleShape m_background;

    SelectedElement m_selState;
    std::map<SelectedElement, sf::Text> m_menuEntries;
};

class Menu_Controls : public State {
  public:
    Menu_Controls(StateStack& stack, Context& ctx);
    virtual ~Menu_Controls() { }

  private:
    virtual void draw() override final;
    virtual bool update() override final;
    virtual bool handleEvent(const sf::Event& ev) override final;

    enum class SelectedElement {
      BEGIN = 0,

      PlayerNum,
      InputType,

      END
    };

    enum class KeyboardElement {
      BEGIN,
      MoveUp,
      MoveDown,
      END
    };

    enum class NetworkElement {
      BEGIN,
      Ip,
      Port,
      END
    };

    sf::RectangleShape m_background;

    SelectedElement m_selState;
    int m_selPlayer;
    std::map<SelectedElement, sf::Text> m_menuEntries;
    std::map<KeyboardElement, sf::Text> m_keyboardEntries;
    std::map<NetworkElement, sf::Text> m_networkEntries;
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

#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Command.h"

enum class SceneLayer : short {
  Background,
  PlayField,

  LayerCount
};

class SceneNode : public sf::Transformable, public sf::Drawable {
  public:
    using Ptr = std::unique_ptr<SceneNode>;

    SceneNode(Category c = Category::Scene, int id = 0);

    SceneNode(SceneNode const &) = delete;
    SceneNode& operator= (SceneNode const &) = delete;

    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);

    void update();
    void onCommand(const Command& cmd);

    sf::Transform getWorldTransform() const;
    sf::Vector2f getWorldPosition() const;

  private:
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override final;
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const { }

    virtual void updateCurrent() { }

    std::vector<Ptr> m_childs;
    SceneNode* m_parent;

    Category m_category;
    int m_id;
};

template<typename DrawObjType>
class StaticSceneElement : public SceneNode {
  public:
    StaticSceneElement(DrawObjType obj, Category c = Category::Scene, int id = 0)
      : SceneNode(c, id), m_obj(std::move(obj)) { }

  private:
    virtual void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const override final {
      target.draw(m_obj, states);
    }

    DrawObjType m_obj;
};


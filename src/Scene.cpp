
#include "Scene.h"

#include<cassert>

SceneNode::SceneNode(Category c, int id) 
  : m_parent(nullptr), m_category(c), m_id(id) {
}

void SceneNode::attachChild(Ptr child) {
  child->m_parent = this;
  m_childs.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
  auto found = std::find_if(m_childs.begin(), m_childs.end(),
                            [&](Ptr& p) -> bool { return p.get() == &node; });
  assert(found != m_childs.end());
  Ptr result = std::move(*found);
  result->m_parent = nullptr;
  m_childs.erase(found);
  return result;
}

void SceneNode::draw(sf::RenderTarget& target,sf::RenderStates states) const {
  states.transform *= getTransform();
  drawCurrent(target, states);
  for (auto const & c : m_childs)
    c->draw(target, states);
}

void SceneNode::update() {
  updateCurrent();
  for (auto const & c : m_childs)
    c->update();
}

void SceneNode::onCommand(const Command& cmd) {
  if (cmd.id == m_id && cmd.category == m_category)
    cmd.action(*this);

  for (auto& child : m_childs)
    child->onCommand(cmd);
}

sf::Transform SceneNode::getWorldTransform() const {
  sf::Transform transform = sf::Transform::Identity;
  for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
    transform = node->getTransform() * transform;
  return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const {
  return getWorldTransform() * sf::Vector2f();
}


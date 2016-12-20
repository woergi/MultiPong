
#include "StateStack.h"

#include<cassert>

void State::requestStackPush(StateID id) {
  m_stack.pushState(id);
}

void State::requestStackPop() {
  m_stack.popState();
}

void State::requestStateClear() {
  m_stack.clearStates();
}

void StateStack::update() {
  applyPendingChanges();

  for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    if (!(*itr)->update())
      return;
}

void StateStack::draw() {
  m_ctx.window->clear();
  for (auto& item : m_stack)
    item->draw();
  m_ctx.window->display();
}

void StateStack::handleEvent(const sf::Event& ev) {
  for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    if (!(*itr)->handleEvent(ev))
      return;
}

void StateStack::pushState(StateID id) {
  m_pendingChange.push_back(PendingChange{Action::Push, id});
}

void StateStack::popState() {
  m_pendingChange.push_back(PendingChange{Action::Pop, StateID::None});
}

void StateStack::clearStates() {
  m_pendingChange.push_back(PendingChange{Action::Clear, StateID::None});
}

State::Ptr StateStack::createState(StateID id) {
  assert(m_factory.find(id) != m_factory.end());
  return m_factory[id]();
}

void StateStack::applyPendingChanges() {
  for (auto change : m_pendingChange) {
    switch (change.action) {
      case Action::Pop:
        m_stack.pop_back();
        break;
      case Action::Clear:
        m_stack.clear();
        break;
      case Action::Push:
        m_stack.push_back(createState(change.id));
        break;
    }
  }
  m_pendingChange.clear();
}


/**
 * \file builder.cpp
 * \author Adam Marshall (ih8celery)
 */

#include "fsa_builder.h"

namespace fsa_builder {
  void StateMachineBuilder::add(const std::string& name, const StateType& type) {
    // check map
    auto state_iter = states.find(name);

    if (state_iter == states.end()) {
      auto state_ptr = std::make_shared<State>(name, type);
      states.insert({ name, state_ptr });

      if (!start_state) {
        start_state = state_ptr;
      }
    }
    else {
      throw bad_state("state already exists");
    }
  }

  void StateMachineBuilder::drop(const std::string& name) {
    auto state_iter = states.find(name);

    if (state_iter == states.end()) {
      throw bad_state("state does not exist");
    }
    else {
      if (*((*state_iter).second) == *start_state) {
        throw bad_state("cannot drop start state");
      }

      // remove transitions between other states and this one
      for (const auto& pair : states) {
        this->disconnect(pair.first, name);
      }

      // remove this state from map
      states.erase(name);
    }
  }

  void StateMachineBuilder::connect(const std::string& n1, const std::string& n2, const char& c) {
    std::shared_ptr<State> s1, s2;

    auto state_iter = states.find(n1);

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n1 + " does not exist");
    }

    s1 = (*state_iter).second;

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n2 + " does not exist");
    }

    s2 = (*state_iter).second;

    if (s1->match(c)) {
      throw bad_transition(std::string("transition from state with name ")
		      + n1 + " already exists");
    }
    else {
      s1->transitions.insert({ c, s2 });
    }
  }

  void StateMachineBuilder::connect(const std::string& n1, const std::string& n2, const std::vector<char>& chars) {
    std::shared_ptr<State> s1, s2;

    auto state_iter = states.find(n1);

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n1 + " does not exist");
    }

    s1 = (*state_iter).second;

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n2 + " does not exist");
    }

    s2 = (*state_iter).second;

    for (char c : chars) {
      if (s1->match(c)) {
        throw bad_transition(std::string("transition from state with name ")
		      + n1 + " already exists");
      }
      else {
        s1->transitions.insert({ c, s2 });
      }
    }
  }

  void StateMachineBuilder::disconnect(const std::string& n1, const std::string& n2) {
    std::shared_ptr<State> s1, s2;

    auto state_iter = states.find(n1);

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n1 + " does not exist");
    }

    s1 = (*state_iter).second;

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n2 + " does not exist");
    }

    s2 = (*state_iter).second;

    for (auto trans : s1->transitions) {
      if (*(trans.second) == *s2) {
        s1->transitions.erase(trans.first);
      }
    }
  }

  void StateMachineBuilder::disconnect(const std::string& n1, const std::string& n2, const char& c) {
    std::shared_ptr<State> s1, s2;

    auto state_iter = states.find(n1);

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n1 + " does not exist");
    }

    s1 = (*state_iter).second;

    if (state_iter == states.end()) {
      throw bad_transition(std::string("state with name ") + n2 + " does not exist");
    }

    s2 = (*state_iter).second;

    auto state_opt = s1->match(c);
    if (state_opt && *(state_opt.value()) == *s2) {
      s1->transitions.erase(c);
    }
    else {
      throw bad_transition(std::string("transition between states ")
		      + n1 + " and " + n2 + " does not exist");
    }
  }

  void StateMachineBuilder::clear() {
    states.clear();
    start_state = std::make_shared<State>(nullptr);
  }

  StateMachine StateMachineBuilder::build(std::istream* input) {
    StateMachine result = StateMachine(start_state, input);

    this->clear();

    return result;
  }
}

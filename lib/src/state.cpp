/**
 * \file state.cpp
 * \author Adam Marshall (ih8celery)
 */

#include "fsa_builder.h"

namespace fsa_builder {
  State::State(const std::string& name, const StateType& type):
	  name(name),
	  type(type) {}

  auto State::ERROR     = std::make_shared<State>("__ERROR__", StateType::ERROR);
  auto State::EOF_ERROR = std::make_shared<State>("__EOF_ERROR__", StateType::ERROR);

  std::string State::get_name() const noexcept { return name; }

  StateType State::get_type() const noexcept { return type; }

  std::optional<std::shared_ptr<State>> State::match(const char& in_char) const noexcept {
    auto state = transitions.find(in_char);

    if (state == transitions.end()) {
      return std::nullopt;
    }

    return std::make_optional<std::shared_ptr<State>>((*state).second);
  }

  std::ostream& operator<< (std::ostream& out, const StateType& state) {
    switch (state) {
    case StateType::STOP:
      out << "stop";
      break;
    case StateType::NORMAL:
      out << "normal";
      break;
    case StateType::ERROR:
      out << "error";
      break;
    }

    return out;
  }

  std::ostream& operator<< (std::ostream& out, const State& state) {
    out << "#State<" << state.get_type() << "," << state.get_name() << ">";

    return out;
  }

  State::operator bool () {
    return this->get_type() != StateType::ERROR;
  }

  bool operator== (const State& lstate, const State& rstate) {
    return (lstate.get_type() == rstate.get_type()
	     && lstate.get_name() == rstate.get_name());
  }

  bool operator< (const State& lstate, const State& rstate) {
    return (lstate.get_name() < rstate.get_name());
  }
}

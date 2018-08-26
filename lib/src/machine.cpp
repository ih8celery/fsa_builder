/**
 * \file machine.cpp
 * \author Adam Marshall (ih8celery)
 */

#include "fsa_builder.h"
#include <sstream>

namespace fsa_builder {
  StateMachine::StateMachine(std::shared_ptr<State> state):
	  input(&std::cin),
	  start_state(state) {}
  StateMachine::StateMachine(std::shared_ptr<State> state, std::istream* input):
	  input(input),
          start_state(state) {}

  void StateMachine::reset(std::istream* input) { this->input = input; }

  std::shared_ptr<State> StateMachine::get_start() const { return start_state; }

  Token StateMachine::read_token() {
    std::shared_ptr<State> state = start_state;
    std::stringstream token_value;

    // read characters and follow transitions
    char ch;
    while (*input >> ch) {
      token_value << ch;
      auto state_opt = state->match(ch);

      if (state_opt) {
	state = state_opt.value();
      }
      else {
	state = State::ERROR;
      }

      if (state->get_type() == StateType::STOP
	   || state->get_type() == StateType::ERROR) {
        break;
      }
    }

    if (state->get_type() == StateType::NORMAL) {
      state = State::EOF_ERROR;
    }

    return Token(state, token_value.str());
  }
}

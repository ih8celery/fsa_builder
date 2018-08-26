/**
 * \file token.cpp
 * \author Adam Marshall (ih8celery)
 */

#include "fsa_builder.h"

namespace fsa_builder {
  Token::Token(std::shared_ptr<State> state, const std::string& value):
	  state(state),
	  value(value) {}

  std::shared_ptr<State> Token::get_state() const noexcept { return state; }

  std::string Token::get_value() const noexcept { return value; }

  std::ostream& operator<< (std::ostream& out, const Token& token) {
    out << "#Token<" << (*token.get_state()) << "," << token.get_value() << ">";

    return out;
  }
}

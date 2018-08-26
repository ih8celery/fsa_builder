/**
 * \file fsa_builder.h
 * \author Adam Marshall (ih8celery)
 */

#ifndef _MOD_CPP_FSA_BUILDER

#define _MOD_CPP_FSA_BUILDER

#include <unordered_map>
#include <string>
#include <exception>
#include <optional>
#include <memory>
#include <iostream>
#include <vector>

namespace fsa_builder {
  enum class StateType {
    STOP, ERROR, NORMAL
  };

  std::ostream& operator<< (std::ostream&, const StateType&);

  class State {
    friend class StateMachineBuilder;
    friend std::ostream& operator<< (std::ostream&, const State&);
    friend bool operator== (const State&, const State&);
    friend bool operator< (const State&, const State&);

    public:
      static std::shared_ptr<State> ERROR;
      static std::shared_ptr<State> EOF_ERROR;

      operator bool ();

      State() = delete;
      State(const std::string&, const StateType& = StateType::NORMAL);

      std::string get_name() const noexcept;
      StateType get_type() const noexcept;

      std::optional<std::shared_ptr<State>> match(const char&) const noexcept;

    private:
      const std::string name;
      const StateType type;
      std::unordered_map<char, std::shared_ptr<State>> transitions;
  };

  std::ostream& operator<< (std::ostream&, const State&);
  bool operator== (const State&, const State&);
  bool operator< (const State&, const State&);
  
  class Token {
    friend std::ostream& operator<< (std::ostream&, const Token&);

    public:
      Token() = delete;
      Token(std::shared_ptr<State>, const std::string&);

      std::shared_ptr<State> get_state() const noexcept;
      std::string get_value() const noexcept;

    private:
      const std::shared_ptr<State> state;
      const std::string value;
  };

  std::ostream& operator<< (std::ostream&, const Token&);

  class StateMachine {
    friend class StateMachineBuilder;

    public:
      StateMachine() = delete;
      StateMachine(std::shared_ptr<State>);
      StateMachine(std::shared_ptr<State>, std::istream*);

      void reset(std::istream*);
      std::shared_ptr<State> get_start() const;
      Token read_token();

    private:
      std::istream* input;
      std::shared_ptr<State> start_state;
  };

  class StateMachineBuilder {
    public:
      void add(const std::string&, const StateType& = StateType::NORMAL);
      void drop(const std::string&);
      void connect(const std::string&, const std::string&, const char&);
      void connect(const std::string&, const std::string&, const std::vector<char>&);
      void disconnect(const std::string&, const std::string&);
      void disconnect(const std::string&, const std::string&, const char&);
      void clear();
      StateMachine build(std::istream* = &std::cin);

      StateMachineBuilder() = default;

    private:
      std::unordered_map<std::string, std::shared_ptr<State>> states;
      std::shared_ptr<State> start_state;
  };

  class bad_state_machine: public std::exception {
    public:
      bad_state_machine();
      bad_state_machine(const std::string&);

      const char * what() const noexcept;
    
    private:
      std::string message;
  };

  class bad_transition: public std::exception {
    public:
      bad_transition();
      bad_transition(const std::string&);

      const char * what() const noexcept;

    private:
      std::string message;
  };

  class bad_state: public std::exception {
    public:
      bad_state();
      bad_state(const std::string&);

      const char * what() const noexcept;

    private:
      std::string message;
  };
}

#endif

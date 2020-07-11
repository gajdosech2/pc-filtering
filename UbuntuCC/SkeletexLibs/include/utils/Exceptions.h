#pragma once

#include <stdexcept>



//! Extensions to the standard libraries.
namespace std_ext
{

  class NotImplementedError : public std::logic_error
  {
  public:
    NotImplementedError() : std::logic_error{ "Function not implemented yet." } {}
  };

  class UnhandledConditionError : public std::logic_error
  {
  public:
    UnhandledConditionError() : std::logic_error{ "Unhandled condition." } {}
  };

  class AuthenticationError : public std::runtime_error
  {
  public:
    AuthenticationError(const std::string &description) : std::runtime_error(description) {}
  };

}

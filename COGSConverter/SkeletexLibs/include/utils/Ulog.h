#pragma once
#include <list>
#include <typeinfo>
#include <Utils/API.h>
#include <Utils/LogEntry.h>
#include <Utils/StrongType.h>

namespace ulog
{
  using Category = utils::StrongType<std::string, class Entry>;

  static const Category DEFAULT_CATEGORY{ "Default" };



  class UTILS_API Entry : public utils::LogEntry
  {
  public:
    Entry(Type type, Category categ, const std::string &msg, const std::string &caller);
    [[nodiscard]] Category GetCategory() const;
    [[nodiscard]] std::string ToString(const std::string &time_format = "%Y-%m-%d %H:%M:%S") const override;
  private:
    Category category_;
  };



  //! Abstract class that can be inherited to create custom log message behavior.
  class UTILS_API LogHandler
  {
  public:
    //! Add message to the handler.
    virtual void AddMessage(ulog::Entry::Type type, ulog::Category categ, const std::string &msg, const std::string &caller) = 0;
  };

  //! Stores log messages in a pool to be processed later.
  class UTILS_API PoolLogHandler : public LogHandler
  {
  public:
    //! Add message to the handler.
    void AddMessage(ulog::Entry::Type type, ulog::Category categ, const std::string &msg, const std::string &caller) override;
    //! For every message stored in the pool, executes the specified function. Pool is empty afterwards.
    void ProcessEntries(std::function<void(const ulog::Entry &)> func);
  private:
    std::list<ulog::Entry> entries_; //!< Unprocessed log entries.
  };



  //! Replaces current log handler with the new one. When none specified, messages are logged to std::cerr.
  UTILS_API void InstallHandler(const std::shared_ptr<LogHandler> &sink_to_use);

  //! Creates a debug log message. Thread safe function.
  UTILS_API void Debug(const std::string &msg, const std::string &caller = "");

  //! Creates an info log message. Thread safe function.
  UTILS_API void Info(const std::string &msg, const std::string &caller = "");

  //! Creates a warning log message. Thread safe function.
  UTILS_API void Warn(const std::string &msg, const std::string &caller = "");

  //! Creates an error log message. Thread safe function.
  UTILS_API void Fail(const std::string &msg, const std::string &caller = "");

  //! Creates a critical log message. Thread safe function.
  UTILS_API void Crash(const std::string &msg, const std::string &caller = "");



  //! Creates a debug log message. Thread safe function.
  UTILS_API void DebugC(Category msg_category, const std::string &msg, const std::string &caller = "");

  //! Creates an info log message. Thread safe function.
  UTILS_API void InfoC(Category msg_category, const std::string &msg, const std::string &caller = "");

  //! Creates a warning log message. Thread safe function.
  UTILS_API void WarnC(Category msg_category, const std::string &msg, const std::string &caller = "");

  //! Creates an error log message. Thread safe function.
  UTILS_API void FailC(Category msg_category, const std::string &msg, const std::string &caller = "");

  //! Creates a critical log message. Thread safe function.
  UTILS_API void CrashC(Category msg_category, const std::string &msg, const std::string &caller = "");

}


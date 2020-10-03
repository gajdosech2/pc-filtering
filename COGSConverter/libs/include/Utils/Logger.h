/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <Utils/LogWriter.h>

namespace utils
{


  /*!
    \brief Class for log initialization.

    \note See LogEntry, LogReader and LogWriter.
  */
  class Logger
  {
  public:
    explicit Logger(const std::shared_ptr<LogWriter> log = nullptr) : log_(log == nullptr ? std::make_shared<LogWriter>() : log) {};
  protected:
    const std::shared_ptr<LogWriter> log_;
  };
}
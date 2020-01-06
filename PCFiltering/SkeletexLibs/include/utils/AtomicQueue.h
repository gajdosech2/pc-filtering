/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <queue>
#include <mutex>

namespace std_ext
{

  //! Queue which allows calling members from multiple threads without interference.
  template <typename T>
  class AtomicQueue
  {
  public:
    //! Get next record from queue.
    T &GetNext()
    {
      std::lock_guard<std::mutex> lock(mutex_);
      return this->queue_.front();
    }
    //! Get next record from queue and remove it from the queue.
    T Pop()
    {
      std::lock_guard<std::mutex> lock(mutex_);
      T value = this->queue_.front();
      this->queue_.pop();
      return value;
    }
    //! Add new record to queue.
    void Push(T value)
    {
      std::lock_guard<std::mutex> lock(mutex_);
      this->queue_.push(value);
    }
    //! Check, whether the queue is empty.
    bool IsEmpty()
    {
      std::lock_guard<std::mutex> lock(mutex_);
      bool check = this->queue_.empty();
      return check;
    }
  private:
    std::mutex mutex_;
    std::queue<T> queue_;
  };

}
/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <utils/ExtGLM.h>
#include <glm/gtx/spline.hpp>

#include <COGS/API.h>


namespace cogs
{

  template<typename Type>
  inline Type Interpolate(const Type val1, const Type val2, const float weight)
  {
    return glm::mix(val1, val2, weight);
  }

  inline glm::quat Interpolate(const glm::quat val1, const glm::quat val2, const float weight)
  {
    return glm::slerp(val1, val2, weight);
  }

  template<typename Type>
  inline glm::quat InterpolateCubic(const Type val1, const Type val2, const Type val3, const Type val4, const float weight)
  {
    return glm::cubic(val1, val2, val3, val4, weight);
  }


  /*!
    \brief
    Wrapper class for key frame animation
    offering effective interpolation between values in time.
  */
  template<typename Type>
  class Interpolator
  {
  public:
    //! Creates an Action with no keys.
    Interpolator() = default;

    //! Creates an Action from list of <time,value> pairs.
    Interpolator(const std::initializer_list<std::pair<float, Type>> &keys)
    {
      ClearAndSet(keys);
    }

    //! Deletes current all current key frames.
    void Clear()
    {
      keys_.clear();
    }

    //! Deletes current key array and sets up new array from list of <time,value> pairs.
    void ClearAndSet(const std::initializer_list<std::pair<float, Type>> &keys)
    {
      keys_.reserve(keys.size());
      for (const auto &k : keys)
      {
        keys_.emplace_back(k.first, k.second);
      }
      std::sort(
        keys_.begin(),
        keys_.end(),
        [](const Keyframe & a, const Keyframe & b) -> bool { return a.time < b.time; }
      );
    }

    //! Returns true when no key exist.
    bool IsEmpty() const
    {
      return keys_.empty();
    }

    //! Returns number of key frames.
    size_t GetKeyCount() const
    {
      return keys_.size();
    }

    //! Creates new key frame with specified value and inserts it into array.
    void InsertKey(const float time, Type value)
    {
      auto keybefore = GetKeyBefore(time) + 1;
      keys_.insert(keys_.begin() + keybefore, Keyframe(time, value));
    }

    /*!
      \brief
          Binary searches two keys nearest to specified time and interpolates between them.
      \return
          Zero when keys are empty.
          Value of first key if time is lower or equal to the time of first key.
          Value of last key if time is higher or equal to the time of first key.
          Otherwise, an interpolated value between two closest keys.
    */
    Type GetValueAtTime(const float time) const
    {
      if (keys_.empty())
      {
        throw std::logic_error("Requested value interpolation when no key frames are present in Action.");
      }
      if (keys_.front().time >= time)
      {
        return keys_.front().value;
      }
      if (keys_.back().time <= time)
      {
        return keys_.back().value;
      }
      int32_t low = GetKeyBefore(time);
      int32_t high = low + 1;
      auto param = (time - keys_[low].time) / (keys_[high].time - keys_[low].time);
      return Interpolate(keys_[low].value, keys_[high].value, param);
    }


    Type GetValueAtTimeCubic(const float time) const
    {
      if (keys_.empty())
      {
        throw std::logic_error("Requested value interpolation when no key frames are present in Action.");
      }
      if (keys_.front().time >= time)
      {
        return keys_.front().value;
      }
      if (keys_.back().time <= time)
      {
        return keys_.back().value;
      }
      int32_t low = GetKeyBefore(time);
      int32_t high = low + 1;
      int32_t lower = low - 1;
      int32_t higher = high + 1;

      if (low == 0)
      {
        lower = 0;
      }
      if (high == int32_t(keys_.size()) - 1)
      {
        higher = int32_t(keys_.size()) - 1;
      }

      auto param = (time - keys_[low].time) / (keys_[high].time - keys_[low].time);

      return InterpolateCubic(keys_[lower].value, keys_[low].value, keys_[high].value, keys_[higher].value, param);

    }

    //! Returns value of key on specified index (position in array ordered by time).
    Type GetKeyValue(const size_t key_index) const
    {
      return keys_.at(key_index).value;
    }

    //! Returns time of key on specified index.
    float GetTimeValue(const size_t key_index) const
    {
      return keys_.at(key_index).time;
    }

    //! Returns value of key on specified index (position in array ordered by time).
    void SetKeyValue(const size_t key_index, const Type value)
    {
      keys_.at(key_index).value = value;
    }

    //! Returns time of last existing key frame.
    float GetLastKeyTime() const
    {
      return keys_.empty() ? 0.0f : keys_.back().time;
    }

    /*!
      \brief
          Returns index of key at, or immediately before specified time.
      \return
          A highest key index for which key.time <= time.
          Returns -1 if keys are empty or specified time is lower then time of any key.
    */
    int32_t GetKeyBefore(const float time) const
    {
      if (keys_.empty())
      {
        return -1;
      }
      if (keys_.back().time <= time)
      {
        return static_cast<int32_t>(keys_.size()) - 1;
      }
      int32_t low = 0;
      int32_t high = static_cast<int32_t>(keys_.size()) - 1;
      while (low < high)
      {
        if (low + 1 == high)
        {
          return low;
        }
        int32_t mid = (low + high) / 2;
        if (time <= keys_[mid].time)
        {
          high = mid;
        }
        else
        {
          low = mid;
        }
      }
      return -1;
    }

    /*!
      \brief
          Reserves required memory for specified amount of key frames.
          Improves speed of adding new key frames.
    */
    void Reserve(size_t keyframe_count)
    {
      keys_.reserve(keyframe_count);
    }

    //! Releases allocated and not used memory for new frames.
    void ShrinkToFit()
    {
      keys_.shrink_to_fit();
    }

    void RemoveLastKey()
    {
      if (!IsEmpty())
      {
        keys_.pop_back();
      }
    }

  private:
    struct Keyframe
    {
      Keyframe(float t, Type v) : value(v), time(t) {};
      Type value;
      float time;
    };
    std::vector<Keyframe> keys_;
  };

}

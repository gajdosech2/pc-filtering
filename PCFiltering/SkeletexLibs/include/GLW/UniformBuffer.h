/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include <glbinding/gl/gl.h>
#include <glm/detail/func_common.hpp>

namespace glw
{

  /*!
    \brief
      Object to store global variables, arrays or structures and manipulate them.
      Type of elements inside the buffer must be defined as template type.
  */
  template<typename T>
  class UniformBuffer
  {
  public:

    //! Register new buffer to video memory.
    UniformBuffer();

    //! Free buffer from video memory.
    ~UniformBuffer();

    //! Get number of elements in currently allocated buffer.
    uint32_t Size();

    //! Allocate new buffer with specified number of elements, copies old buffer data to newly created one.
    void Resize(const uint32_t size);

    /*!
      Deletes all previously allocated data,
      resizes buffer to the size of provided data,
      and stores defined data inside buffer.
    */
    void Reset(const std::vector<T> &data);

    //! Bind this buffer onto specified shader location.
    void Bind(uint32_t index);

    /*!
      Set array of data elements into the buffer at defined position. Does not change the size of buffer.
      When data vector should be written outside buffer, outlaying data is ignored.
    */
    void SetData(const std::vector<T> &data, const uint32_t position = 0u);

    /*!
      Set single data element into the buffer at defined position. Does not change the size of buffer.
      When data should be written outside buffer, writing is ignored.
    */
    void SetData(const T &data, const uint32_t position = 0);

  private:
    uint32_t buffer_ = 0u;
    uint32_t actual_size_ = 0u;
  };

  template <typename T>
  using PUniformBuffer = std::shared_ptr<UniformBuffer<T>>;


  template<typename T>
  void glw::UniformBuffer<T>::Resize(const uint32_t new_size)
  {
    if (actual_size_ == new_size)
    {
      return;
    }

    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, buffer_);

    if (actual_size_ == 0u)
    {
      gl::glBufferData(gl::GLenum::GL_UNIFORM_BUFFER, new_size * sizeof(T), nullptr, gl::GLenum::GL_STATIC_DRAW);
      gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, 0);
      actual_size_ = new_size;
      return;
    }

    gl::GLuint new_buffer;
    gl::glGenBuffers(1, &new_buffer);
    assert(new_buffer > 0);

    gl::glBindBuffer(gl::GLenum::GL_COPY_WRITE_BUFFER, new_buffer);
    gl::glBufferData(gl::GLenum::GL_COPY_WRITE_BUFFER, new_size * sizeof(T), nullptr, gl::GLenum::GL_STATIC_DRAW);

    gl::glCopyBufferSubData(gl::GLenum::GL_UNIFORM_BUFFER, gl::GLenum::GL_COPY_WRITE_BUFFER, 0, 0, (glm::min)(actual_size_, new_size));

    gl::glBindBuffer(gl::GLenum::GL_COPY_WRITE_BUFFER, 0);
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, 0);

    gl::glDeleteBuffers(1, &buffer_);
    buffer_ = new_buffer;
    actual_size_ = new_size;
  }

  template<typename T>
  void glw::UniformBuffer<T>::Reset(const std::vector<T> &data)
  {
    actual_size_ = static_cast<uint32_t>(data.size());
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, buffer_);
    gl::glBufferData(gl::GLenum::GL_UNIFORM_BUFFER, actual_size_ * sizeof(T), nullptr, gl::GLenum::GL_STATIC_DRAW);

    if (!data.empty() && data.size() == actual_size_)
    {
      SetData(data, 0);
    }
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, 0);
  }

  template<typename T>
  UniformBuffer<T>::UniformBuffer()
  {
    gl::glGenBuffers(1, &buffer_);
    assert(buffer_ > 0);
  }

  template<typename T>
  UniformBuffer<T>::~UniformBuffer()
  {
    gl::glDeleteBuffers(1, &buffer_);
  }

  template<typename T>
  uint32_t glw::UniformBuffer<T>::Size()
  {
    return actual_size_;
  }

  template<typename T>
  void UniformBuffer<T>::Bind(uint32_t index)
  {
    gl::glBindBufferBase(gl::GLenum::GL_UNIFORM_BUFFER, index, buffer_);
  }

  template<typename T>
  void UniformBuffer<T>::SetData(const std::vector<T> &data, const uint32_t position)
  {
    assert(data.size() + position <= actual_size_);
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, buffer_);
    gl::glBufferSubData(gl::GLenum::GL_UNIFORM_BUFFER, position * sizeof(T), data.size() * sizeof(T), &data.front());
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, 0);
  }

  template<typename T>
  void glw::UniformBuffer<T>::SetData(const T &data, const uint32_t position)
  {
    assert(position + 1 <= actual_size_);
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, buffer_);
    gl::glBufferSubData(gl::GLenum::GL_UNIFORM_BUFFER, position * sizeof(T), sizeof(T), &data);
    gl::glBindBuffer(gl::GLenum::GL_UNIFORM_BUFFER, 0);
  }

}
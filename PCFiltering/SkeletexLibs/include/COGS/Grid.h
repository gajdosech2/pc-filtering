#pragma once
#include <algorithm>

#include <Utils/Exceptions.h>
#include <Utils/GeometryStructures.h>
#include <COGS/API.h>

namespace cogs
{

  /*!
    \brief
      Organizational structure of values in a 2 dimensional grid.
  */
  template <typename Type>
  class Grid
  {
  public:

    //! Creates grid with zero size.
    Grid() = default;

    //! Creates exact copy of provided grid.
    explicit Grid(const cogs::Grid<Type> &source)
    {
      width_ = source.width_;
      height_ = source.height_;
      data_ = source.data_;
    };

    //! Creates grid with specified width and height.
    Grid(const size_t width, const size_t height)
    {
      width_ = width;
      height_ = height;
      data_ = std::vector<std::vector<Type>>(width, std::vector<Type>(height));
    };

    //! Creates grid with specified width and height and initializes elements to init_value.
    Grid(const size_t width, const size_t height, const Type &init_value)
    {
      width_ = width;
      height_ = height;
      data_ = std::vector<std::vector<Type>>(width, std::vector<Type>(height, init_value));
    };

    //! Sets width and height of a grid, and initializes new elements to init_value.
    void Resize(const size_t width, const size_t height, const Type &init_value)
    {
      data_.resize(width);
      for (auto &column : data_)
      {
        column.resize(height, init_value);
      }
      width_ = width;
      height_ = height;
    };

    //! Sets width and height of a grid, and initializes new elements to zeros.
    void Resize(const size_t width, const size_t height)
    {
      data_.resize(width);
      for (auto &column : data_)
      {
        column.resize(height);
      }
      width_ = width;
      height_ = height;
    };

    //! Sets all elements of grid to zero values.
    void Clear()
    {
      data_.clear();
      width_ = 0;
      height_ = 0;
    };

    //! Sets all elements of grid to a defined value.
    void Fill(const Type value)
    {
      for (auto &column : data_)
      {
        std::fill(column.begin(), column.end(), value);
      }
    };

    //! Sets specified area in grid to a defined value.
    void Fill(const geom::URect &area, const Type value)
    {
      if (width_ == 0 || height_ == 0)
      {
        ulog::Warn("Nothing to fill. Grid size is zero.", "cogs::Grid::Fill");
        return;
      }

      const glm::uvec2 kMaxValue = glm::uvec2(width_, height_) - 1u;

      glm::uvec2 begin = (glm::min)(area.position, kMaxValue);
      glm::uvec2 end = (glm::min)(area.position2, kMaxValue);

      for (auto &column : data_)
      {
        std::fill(column.begin() + begin.y, column.begin() + end.y, value);
      }
    };

    //! Checks whether a combination of xy correspond to valid grid cell.
    [[nodiscard]] bool IsInBounds(const size_t x, const size_t y) const
    {
      return x < width_ && y < height_;
    }

    //! Returns a reference to the cell at position xy.
    [[nodiscard]] Type &At(const size_t x, const size_t y)
    {
#ifndef COGS_NO_ERROR_CHECKING
      if (!IsInBounds(x, y))
      {
        throw std_ext::OutOfBounds(x, y, "cogs::Grid::At");
      }
#endif
      return data_[x][y];
    };

    //! Returns a reference to the cell at position xy.
    [[nodiscard]] const Type &At(const size_t x, const size_t y) const
    {
#ifndef COGS_NO_ERROR_CHECKING
      if (!IsInBounds(x, y))
      {
        throw std_ext::OutOfBounds(x, y, "cogs::Grid::At");
      }
#endif
      return data_[x][y];
    };

    //! Returns a reference to the cell at position xy.
    [[nodiscard]] Type &operator()(const size_t x, const size_t y)
    {
#ifndef COGS_NO_ERROR_CHECKING
      if (!IsInBounds(x, y))
      {
        throw std_ext::OutOfBounds(x, y, "cogs::Grid::operator()");
      }
#endif
      return data_[x][y];
    };

    //! Returns current width of grid.
    [[nodiscard]] size_t GetWidth() const
    {
      return width_;
    };

    //! Returns current height of grid.
    [[nodiscard]] size_t GetHeight() const
    {
      return height_;
    };

    //! Flips all values in x direction.
    void FlipX()
    {
      for (auto &line : data_)
      {
        std::reverse(line.begin(), line.end());
      }
    }

    //! Flips all values in y direction.
    void FlipY()
    {
      std::reverse(data_.begin(), data_.end());
    }

  private:
    std::vector<std::vector<Type>> data_;
    size_t width_{ 0 };
    size_t height_{ 0 };
  };

}
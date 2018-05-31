#ifndef EULER_ANGLES_HPP
# define EULER_ANGLES_HPP
# pragma once

#include "matrix.hpp"

namespace vxl
{

enum struct ea
{
  XYZ,
  XZY,
  YXZ,
  YZX,
  ZXY,
  ZYX
};

template <typename T>
constexpr matrix<T, 4, 4> rot_matrix_x(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return {
    T(1), T(0), T(0), T(0),
    T(0), ca  ,-sa  , T(0),
    T(0), sa  , ca  , T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr matrix<T, 4, 4> rot_matrix_y(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return {
      ca, T(0), sa,   T(0),
    T(0), T(1), T(0), T(0),
     -sa, T(0), ca  , T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr matrix<T, 4, 4> rot_matrix_z(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return {
      ca,  -sa, T(0), T(0),
      sa,   ca, T(0), T(0),
    T(0), T(0), T(1), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::XYZ, matrix<T, 4, 4>>
to_matrix(T const ax, T const ay, T const az) noexcept
{
  return rot_matrix_x(ax) * rot_matrix_y(ay) * rot_matrix_z(az);
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::XZY, matrix<T, 4, 4>>
to_matrix(T const ax, T const az, T const ay) noexcept
{
  return rot_matrix_x(ax) * rot_matrix_z(ay) * rot_matrix_y(az);
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::YXZ, matrix<T, 4, 4>>
to_matrix(T const ay, T const ax, T const az) noexcept
{
  return rot_matrix_y(ax) * rot_matrix_x(ax) * rot_matrix_z(az);
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::YZX, matrix<T, 4, 4>>
to_matrix(T const ay, T const az, T const ax) noexcept
{
  return rot_matrix_y(ax) * rot_matrix_z(ay) * rot_matrix_x(az);
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::ZXY, matrix<T, 4, 4>>
to_matrix(T const az, T const ax, T const ay) noexcept
{
  return rot_matrix_z(ax) * rot_matrix_x(ax) * rot_matrix_y(az);
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::ZYX, matrix<T, 4, 4>>
to_matrix(T const az, T const ay, T const ax) noexcept
{
  return rot_matrix_z(ax) * rot_matrix_y(ay) * rot_matrix_x(az);
}

}

#endif // EULER_ANGLES_HPP

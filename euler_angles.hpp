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
constexpr auto rot_matrix_x(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return vxl::matrix<T, 4, 4>{
    T(1), T(0), T(0), T(0),
    T(0), ca  , -sa , T(0),
    T(0), sa  , ca  , T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr auto rot_matrix_y(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return vxl::matrix<T, 4, 4>{
      ca, T(0), sa,   T(0),
    T(0), T(1), T(0), T(0),
     -sa, T(0), ca  , T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr auto rot_matrix_z(T const a) noexcept
{
  T const ca(std::cos(a));
  T const sa(std::sin(a));

  return vxl::matrix<T, 4, 4>{
      ca,  -sa, T(0), T(0),
      sa,   ca, T(0), T(0),
    T(0), T(0), T(1), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::XYZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_x(a(0)) * rot_matrix_y(a(1)) * rot_matrix_z(a(2));
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::XZY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_x(a(0)) * rot_matrix_z(a(2)) * rot_matrix_y(a(1));
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::YXZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_y(a(1)) * rot_matrix_x(a(0)) * rot_matrix_z(a(2));
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::YZX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_y(a(1)) * rot_matrix_z(a(2)) * rot_matrix_x(a(0));
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::ZXY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_z(a(2)) * rot_matrix_x(a(0)) * rot_matrix_y(a(1));
}

template <enum ea E, typename T>
constexpr std::enable_if_t<E == ea::ZYX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  return rot_matrix_z(a(2)) * rot_matrix_y(a(1)) * rot_matrix_x(a(0));
}

}

#endif // EULER_ANGLES_HPP

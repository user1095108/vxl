#ifndef VXL_ROTATION_HPP
# define VXL_ROTATION_HPP
# pragma once

#include "cross.hpp"

#include "quat.hpp"

#include "sinf.hpp"

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

namespace detail
{

template <typename T>
constexpr inline auto rot_matrix_x(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    T(1), T(0), T(0), T(0),
    T(0), sc.second(0), -sc.first(0), T(0),
    T(0), sc.first(0), sc.second(0), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr inline auto rot_matrix_y(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    sc.second(1), T(0), sc.first(1), T(0),
    T(0), T(1), T(0), T(0),
    -sc.first(1), T(0), sc.second(1), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <typename T>
constexpr inline auto rot_matrix_z(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    sc.second(2), -sc.first(2), T(0), T(0),
    sc.first(2), sc.second(2), T(0), T(0),
    T(0), T(0), T(1), T(0),
    T(0), T(0), T(0), T(1)
  };
}

}

// Euler angle to rotation matrix conversions. WARNING: Blender XYZ is ZYX
// with angles negated, this is because Blender caters to artists.
//////////////////////////////////////////////////////////////////////////////
template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::XYZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_x(sc) * detail::rot_matrix_y(sc) *
    detail::rot_matrix_z(sc);
}

template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::XZY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_x(sc) * detail::rot_matrix_z(sc) *
    detail::rot_matrix_y(sc);
}

template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::YXZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_y(sc) * detail::rot_matrix_x(sc) *
    detail::rot_matrix_z(sc);
}

template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::YZX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_y(sc) * detail::rot_matrix_z(sc) *
    detail::rot_matrix_x(sc);
}

template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::ZXY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_z(sc) * detail::rot_matrix_x(sc) *
    detail::rot_matrix_y(sc);
}

template <enum ea E, typename T>
constexpr inline std::enable_if_t<E == ea::ZYX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(csincos(a));

  return detail::rot_matrix_z(sc) * detail::rot_matrix_y(sc) *
    detail::rot_matrix_x(sc);
}

// convert axis angle to quaternion
//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline quat<T> to_quat(vector<T, 3> const a, T const angle) noexcept
{
  using vector_type = typename vector_traits<T, 4>::vector_type;

  auto const p(csincos(vector<T, 1>{T(.5) * angle}));

  return {
    vector_type{p.first.data_, p.first.data_, p.first.data_, p.second.data_} *
    vector_type{a.data_[0], a.data_[1], a.data_[2], T(1)}
  };
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline vector<T, 3> rotated(quat<T> const& q, vector<T, 3> const& v) noexcept
{
  auto const vq(vec(q));

  auto const t(ccross(vq, v));

  auto const u(scalar_vector<T, 3>(q) * t.data_ + ccross(vq, t).data_);

  return {v.data_ + u + u};
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline vector<T, 3>& rotate(quat<T> const& q, vector<T, 3>& v) noexcept
{
  auto const vq(vec(q));

  auto const t(ccross(vq, v));

  auto const u(scalar_vector<T, 3>(q) * t.data_ + ccross(vq, t).data_);

  v.data_ += u + u;

  return v;
}

}

#endif // VXL_ROTATION_HPP

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

struct scale;

namespace detail
{

template <unsigned I, typename T>
inline constexpr auto rot_x(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    T(1), T(0), T(0), T(0),
    T(0), sc.second(I), -sc.first(I), T(0),
    T(0), sc.first(I), sc.second(I), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <unsigned I, typename T>
inline constexpr auto rot_y(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    sc.second(I), T(0), sc.first(I), T(0),
    T(0), T(1), T(0), T(0),
    -sc.first(I), T(0), sc.second(I), T(0),
    T(0), T(0), T(0), T(1)
  };
}

template <unsigned I, typename T>
inline constexpr auto rot_z(std::pair<vxl::vector<T, 3>,
  vxl::vector<T, 3>> const& sc) noexcept
{
  return vxl::matrix<T, 4, 4>{
    sc.second(I), -sc.first(I), T(0), T(0),
    sc.first(I), sc.second(I), T(0), T(0),
    T(0), T(0), T(1), T(0),
    T(0), T(0), T(0), T(1)
  };
}

}

// Euler angle to rotation matrix conversions.
//////////////////////////////////////////////////////////////////////////////
template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::XYZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_z<2>(sc) * detail::rot_y<1>(sc) * detail::rot_x<0>(sc);
}

template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::XZY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_y<2>(sc) * detail::rot_z<1>(sc) * detail::rot_x<0>(sc);
}

template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::YXZ, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_z<2>(sc) * detail::rot_x<1>(sc) * detail::rot_y<0>(sc);
}

template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::YZX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_x<2>(sc) * detail::rot_z<1>(sc) * detail::rot_y<0>(sc);
}

template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::ZXY, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_y<2>(sc) * detail::rot_x<1>(sc) * detail::rot_z<0>(sc);
}

template <enum ea E, typename T>
inline constexpr std::enable_if_t<E == ea::ZYX, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& a) noexcept
{
  auto const sc(sincos(a));

  return detail::rot_x<2>(sc) * detail::rot_y<1>(sc) * detail::rot_z<0>(sc);
}

template <typename E, typename T>
inline constexpr std::enable_if_t<std::is_same<E, scale>{}, matrix<T, 4, 4>>
to_matrix(vxl::vector<T, 3> const& s) noexcept
{
  return vxl::diag(vxl::vector<T, 4>{s(0), s(1), s(2), T(1)});
}

// convert axis angle to quaternion
//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline quat<T> to_quat(vector<T, 3> const a, T const angle) noexcept
{
  using vector_type = typename vector_traits<T, 4>::vector_type;

  auto const p(sincos(vector<T, 1>{T(.5) * angle}));

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

  auto const t(cross(vq, v));

  auto const u(scalar_vector<T, 3>(q) * t.data_ + cross(vq, t).data_);

  return {v.data_ + u + u};
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline vector<T, 3>& rotate(quat<T> const& q, vector<T, 3>& v) noexcept
{
  auto const vq(vec(q));

  auto const t(cross(vq, v));

  auto const u(scalar_vector<T, 3>(q) * t.data_ + cross(vq, t).data_);

  v.data_ += u + u;

  return v;
}

}

#endif // VXL_ROTATION_HPP

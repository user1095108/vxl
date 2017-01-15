#ifndef VXL_ROTATION_HPP
# define VXL_ROTATION_HPP
# pragma once

#include "cross.hpp"

#include "quat.hpp"

#include "sinf.hpp"

namespace vxl
{

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline quat<T> aa2q(vector<T, 3> const a, T const angle) noexcept
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
  using int_vector_type = typename vector_traits<T, 4>::int_vector_type;

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
  using int_vector_type = typename vector_traits<T, 4>::int_vector_type;

  auto const vq(vec(q));

  auto const t(ccross(vq, v));

  auto const u(scalar_vector<T, 3>(q) * t.data_ + ccross(vq, t).data_);

  v.data_ += u + u;

  return v;
}

}

#endif // VXL_ROTATION_HPP

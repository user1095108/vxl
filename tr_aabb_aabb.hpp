#ifndef VXL_TR_AABB_AABB_HPP
#define VXL_TR_AABB_AABB_HPP
#pragma once

#include "aabb.hpp"

#include "matrix.hpp"

namespace vxl
{

//////////////////////////////////////////////////////////////////////////////
template <typename T>
inline aabb_type<T> tr_mm_aabb_aabb(
  matrix<T, 4, 4> const& m,
  aabb_type<T> const& a,
  default_tag const = {}) noexcept
{
  aabb_type<T> b{{}};

  for (unsigned i{}; i != 3; ++i)
  {
    b[0].data_[i] = b[1].data_[i] = m(i, 3);

    auto const c(col(m, i));

    auto const a0i(a[0](i));
    auto const a1i(a[1](i));

    a[0](i) < a[1](i) ?
      b[0].data_ += ::vxl::cvector<T, 4>(a0i) * c.data_,
      b[1].data_ += ::vxl::cvector<T, 4>(a1i) * c.data_:
      b[0].data_ += ::vxl::cvector<T, 4>(a1i) * c.data_,
      b[1].data_ += ::vxl::cvector<T, 4>(a0i) * c.data_;
  }

  return b;
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
inline aabb_type<T> tr_mm_aabb_aabb(
  matrix<T, 4, 4> const& m,
  aabb_type<T> const& a,
  no_translation_tag const) noexcept
{
  aabb_type<T> b{{}};

  for (unsigned i{}; i != 3; ++i)
  {
    auto const c(col(m, i));

    auto const a0i(a[0](i));
    auto const a1i(a[1](i));

    a[0](i) < a[1](i) ?
      b[0].data_ += ::vxl::cvector<T, 4>(a0i) * c.data_,
      b[1].data_ += ::vxl::cvector<T, 4>(a1i) * c.data_:
      b[0].data_ += ::vxl::cvector<T, 4>(a1i) * c.data_,
      b[1].data_ += ::vxl::cvector<T, 4>(a0i) * c.data_;
  }

  return b;
}

}

#endif // VXL_TR_AABB_AABB_HPP

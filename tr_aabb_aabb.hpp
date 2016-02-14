/*
** The MIT License (MIT)
** 
** Copyright (c) 2014-2016 Janez Žemva
** 
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

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

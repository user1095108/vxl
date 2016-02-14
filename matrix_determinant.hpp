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

#ifndef VXL_DETERMINANT_HPP
# define VXL_DETERMINANT_HPP
# pragma once

#include "cross.hpp"

#include "dot.hpp"

#include "matrix.hpp"

#include "matrix_inverse.hpp"

namespace vxl
{

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline T det(::vxl::matrix<T, 2, 2> const& m) noexcept
{
//return
//  m(0, 0) * m(1, 1) -
//  m(0, 1) * m(1, 0);
#ifndef VXL_ROW_MAJOR
  auto const a(col<0>(m));
  auto const b(col<1>(m));
#else
  auto const a(row<0>(m));
  auto const b(row<1>(m));
#endif
  auto const prod(a * swizzled<3, 2>(a, b));

  return (prod - swizzled<1>(prod))(0);
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
//__attribute__ ((noinline))
constexpr inline T det(::vxl::matrix<T, 3, 3> const& m) noexcept
{
#ifndef VXL_ROW_MAJOR
  return cdot(col<0>(m), ccross(col<1>(m), col<2>(m)))(0);
#else
  return cdot(row<0>(m), ccross(row<1>(m), row<2>(m)))(0);
#endif // VXL_ROW_MAJOR
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
//__attribute__ ((noinline))
inline T det(::vxl::matrix<T, 4, 4> const& m) noexcept
{
  auto const A(make_matrix<T, 2, 2>(m(0, 0), m(0, 1), m(1, 0), m(1, 1)));
  auto const B(make_matrix<T, 2, 2>(m(0, 2), m(0, 3), m(1, 2), m(1, 3)));
  auto const C(make_matrix<T, 2, 2>(m(2, 0), m(2, 1), m(3, 0), m(3, 1)));
  auto const D(make_matrix<T, 2, 2>(m(2, 2), m(2, 3), m(3, 2), m(3, 3)));

  return det(A - B * inv(D) * C) * det(D);
}

}

#endif // VXL_DETERMINANT_HPP

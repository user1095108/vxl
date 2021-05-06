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
constexpr auto det(vxl::matrix<T, 2, 2> const& m) noexcept
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
#endif // VXL_ROW_MAJOR

  auto const prod(a * swizzled<3, 2>(a, b));

  return (prod - swizzled<1>(prod))(0);
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
//__attribute__ ((noinline))
constexpr auto det(vxl::matrix<T, 3, 3> const& m) noexcept
{
// get row or col, whatever is more efficient as det(A) == det(A^T)
#ifndef VXL_ROW_MAJOR
  return dot(col<0>(m), cross(col<1>(m), col<2>(m)))(0);
#else
  return dot(row<0>(m), cross(row<1>(m), row<2>(m)))(0);
#endif // VXL_ROW_MAJOR
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
//__attribute__ ((noinline))
constexpr auto det(vxl::matrix<T, 4, 4> const& m) noexcept
{
  auto const A(make_matrix<2, 2>(m(0, 0), m(0, 1), m(1, 0), m(1, 1)));
  auto const B(make_matrix<2, 2>(m(0, 2), m(0, 3), m(1, 2), m(1, 3)));
  auto const C(make_matrix<2, 2>(m(2, 0), m(2, 1), m(3, 0), m(3, 1)));
  auto const D(make_matrix<2, 2>(m(2, 2), m(2, 3), m(3, 2), m(3, 3)));

  return det(A - B * inv(D) * C) * det(D);
}

}

#endif // VXL_DETERMINANT_HPP

#ifndef VXL_ONB_HPP
# define VXL_ONB_HPP
# pragma once

#include "vector.hpp"

namespace vxl
{

// Graphics Tools---The jgt Editors' Choice
// Building an orthonormal basis from a unit vector
// Building an Orthonormal Basis from a 3D Unit Vector Without Normalization
// http://blog.selfshadow.com/2011/10/17/perp-vectors/
// http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts
//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
constexpr inline vector<T, 2> cortho(vector<T, 2> const& v,
  default_tag const = {}) noexcept
{
  return {-v(1), v(0)};
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
constexpr inline vector<T, 3> cortho(vector<T, 3> const& v,
  default_tag const = {}) noexcept
{
  using int_value_type = typename vector_traits<float, 3>::int_value_type;
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto const tmp(cabs(v));

/*
  return tmp(0) < tmp(1) ?
    vector<T, 3>{T(0), -v(2), v(1)} :
    vector<T, 3>{-v(1), v(0), T(0)};
*/

  // if abs(x) < abs(y), then the largest components are either y or z
  return {
    select(
      vector_type{T(0), -v(2), v(1)},
      vector_type{-v(1), v(0), T(0)},
      cvector<int_value_type, 3>(-(tmp(0) < tmp(1)))
    )
  };
}

}

#endif // VXL_ONB_HPP

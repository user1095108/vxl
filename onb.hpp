/*
** This is free and unencumbered software released into the public domain.
**
** Anyone is free to copy, modify, publish, use, compile, sell, or
** distribute this software, either in source code form or as a compiled
** binary, for any purpose, commercial or non-commercial, and by any
** means.
**
** In jurisdictions that recognize copyright laws, the author or authors
** of this software dedicate any and all copyright interest in the
** software to the public domain. We make this dedication for the benefit
** of the public at large and to the detriment of our heirs and
** successors. We intend this dedication to be an overt act of
** relinquishment in perpetuity of all present and future rights to this
** software under copyright law.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
** OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
** OTHER DEALINGS IN THE SOFTWARE.
**
** For more information, please refer to <http://unlicense.org/>
*/

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

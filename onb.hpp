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

#ifndef VXL_ONB_HPP
# define VXL_ONB_HPP
# pragma once

#include <math.h>

#include "vector.hpp"

namespace vxl
{

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
constexpr inline vector<T, 2> cortho(vector<T, 2> const& v,
  default_tag const = {}) noexcept
{
  return vector<T, 2>{-v(1), v(0)};
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
constexpr inline vector<T, 3> cortho(vector<T, 3> const& v,
  default_tag const = {}) noexcept
{
  auto const tmp(cabs(v));

  return tmp(0) > tmp(2) ?
    vector<T, 3>{-v(1), v(0), T(0)} :
    vector<T, 3>{T(0), -v(2), v(1)};
}

namespace detail
{

namespace onb
{

//////////////////////////////////////////////////////////////////////////////
template <typename T, unsigned N, ::std::size_t ...Is, ::std::size_t ...Js>
constexpr inline typename ::vxl::vector_traits<T, N>::int_vector_type
cimin(::vxl::vector<T, N> const& v, ::std::index_sequence<Is...> const,
  ::std::index_sequence<Js...> const) noexcept
{
  using int_value_type = typename vector_traits<T, N>::int_value_type;
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  decltype(v.data_) result(v.data_);
  int_vector_type iresult{Is...};

  decltype(v.data_) sr;
  int_vector_type isr;

  swallow{
    (
      sr = detail::vector::pow2_shuffler<T, N, Js>(result,
        ::std::make_index_sequence<sizeof(v) / sizeof(T)>()),
      isr = detail::vector::pow2_shuffler<int_value_type, N, Js>(iresult,
        ::std::make_index_sequence<sizeof(v) / sizeof(T)>()),

      result = select(result, sr, result < sr),
      iresult = select(iresult, isr, result < sr),

      Js
    )...
  };

  return iresult;
}

}

}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
constexpr inline vector<T, 3> cortho(vector<T, 3> const& v,
  shift_tag const) noexcept
{
  using int_vector_type = typename vector_traits<T, 3>::int_vector_type;
  using vector_type = typename vector_traits<T, 3>::vector_type;

  constexpr int_vector_type const zero_masks[]{
    int_vector_type{0, ~0, ~0},
    int_vector_type{~0, 0, ~0},
    int_vector_type{~0, ~0, 0}
  };

  constexpr int_vector_type const sign_masks[]{
    int_vector_type(vector_type{0, T(-.0), 0}),
    int_vector_type(vector_type{T(-.0), 0, 0}),
    int_vector_type(vector_type{T(-.0), 0, 0})
  };

#if defined(__clang__)
  vector_type const tmp[]{
    __builtin_shufflevector(v.data_, v.data_, 0, 2, 1, 3),
    __builtin_shufflevector(v.data_, v.data_, 2, 1, 0, 3),
    __builtin_shufflevector(v.data_, v.data_, 1, 0, 2, 3)
  };
#else
  vector_type const tmp[]{
    __builtin_shuffle(v.data_, int_vector_type{0, 2, 1}),
    __builtin_shuffle(v.data_, int_vector_type{2, 1, 0}),
    __builtin_shuffle(v.data_, int_vector_type{1, 0, 2})
  };
#endif

  auto const i(detail::onb::cimin(cabs(v),
    ::std::make_index_sequence<sizeof(v) / sizeof(T)>(),
    ::std::make_index_sequence<detail::vector::log2(3)>())[0]
  );

  return {
    vector_type(zero_masks[i] & int_vector_type(tmp[i]) ^ sign_masks[i])
  };
}

}

#endif // VXL_ONB_HPP

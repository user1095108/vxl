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

#ifndef VXL_DOT_HPP
# define VXL_DOT_HPP
# pragma once

#include "vector.hpp"

namespace vxl
{

#if defined(__ARM_NEON__)

//__attribute__ ((noinline))
inline vector<float, 2> cdot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const prod(float32x2_t(l.data_) * float32x2_t(r.data_));

  return {vector_type(vpadd_f32(prod, prod))};
}

//__attribute__ ((noinline))
inline vector<float, 3> cdot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto prod(float32x4_t(l.data_) * float32x4_t(r.data_));

  prod = vaddq_f32(prod, vrev64q_f32(prod));

  return {
    vector_type(
      vaddq_f32(prod, vcombine_f32(vget_high_f32(prod), vget_low_f32(prod)))
    )
  };
}

//__attribute__ ((noinline))
inline vector<float, 4> cdot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto prod(float32x4_t(l.data_) * float32x4_t(r.data_));

  prod = vaddq_f32(prod, vrev64q_f32(prod));

  return {
    vector_type(
      vaddq_f32(prod, vcombine_f32(vget_high_f32(prod), vget_low_f32(prod)))
    )
  };
}

#endif // __ARM_NEON__

/*
#if defined(__SSE4_1__)

//__attribute__ ((noinline))
inline vector<float, 2> cdot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  return {
    _mm_dp_ps(l.data_, r.data_, 0x3f)
  };
}

//__attribute__ ((noinline))
inline vector<float, 3> cdot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  return {
    _mm_dp_ps(l.data_, r.data_, 0x7f)
  };
}

//__attribute__ ((noinline))
inline vector<float, 4> cdot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  return {
    _mm_dp_ps(l.data_, r.data_, 0xff)
  };
}

#elif defined(__SSE3__) && !defined(__SSE4_1__)

//__attribute__ ((noinline))
inline vector<float, 2> cdot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return {
    _mm_hadd_ps(prod, prod)
  };
}

//__attribute__ ((noinline))
inline vector<float, 3> cdot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return {
    _mm_hadd_ps(prod, prod)
  };
}

//__attribute__ ((noinline))
inline vector<float, 4> cdot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return {
    _mm_hadd_ps(prod, prod)
  };
}

#endif
*/

namespace detail
{

namespace vector
{

/*
** 1   2   3   4
** 12  23  34  14
** 1234 1234 1234 1234
*/

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline typename ::vxl::vector_traits<T, N>::vector_type
cdot(typename vector_traits<T, N>::vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  return swallow {
    // result depends on previous shuffles
    v += pow2_shuffler<T, sizeof(v) / sizeof(T), Is>(v,
      std::make_index_sequence<sizeof(v) / sizeof(T)>()
    )...
  },
  v;
}

}

}

// dot product
//__attribute__ ((noinline))
template <typename T, unsigned N>
constexpr inline vector<T, N> cdot(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return {
    detail::vector::cdot<T, N>(l.data_ * r.data_,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

template <typename T, unsigned N>
inline auto norm2(vector<T, N> const& v) noexcept
{
  return cdot(v, v);
}

}

#endif // VXL_DOT_HPP

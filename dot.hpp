#ifndef VXL_DOT_HPP
# define VXL_DOT_HPP
# pragma once

#include "vector.hpp"

namespace vxl
{

#if defined(__ARM_NEON)

//__attribute__ ((noinline))
inline auto dot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const prod(float32x2_t(l.data_) * float32x2_t(r.data_));
  // prod = l0*r0 l1*r1

  return vector<float, 2>{
    vector_type(prod + vrev64_f32(prod))
  };
  //l0*r0+l1*r1 l1*r1+l0*r0
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto prod(float32x4_t(l.data_) * float32x4_t(r.data_));

  prod += vrev64q_f32(prod);

  return vector<float, 3>{
    vector_type(
      prod + vcombine_f32(vget_high_f32(prod), vget_low_f32(prod))
    )
  };
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto prod(float32x4_t(l.data_) * float32x4_t(r.data_));

  prod += vrev64q_f32(prod);
  // l0 l1 l2 l3
  // r0 r1 r2 r3
  // prod = l0*r0 l1*r1 l2*r2 l3*r3
  // prod = l0*r0+l1*r1 l1*r1+l0*r0 l2*r2+r3*r3 l3*r3+l2*r2

  return vector<float, 4>{
    vector_type(
      prod + vcombine_f32(vget_high_f32(prod), vget_low_f32(prod))
    )
  };
  // l0*r0+l1*r1+l2*r2+r3*r3 ...
}

/*
#elif defined(__SSE4_1__)

//__attribute__ ((noinline))
inline auto dot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  return vector<float, 2>{
    _mm_dp_ps(l.data_, r.data_, 0x3f)
  };
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  return vector<float, 3>{
    _mm_dp_ps(l.data_, r.data_, 0x7f)
  };
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  return vector<float, 4>{
    _mm_dp_ps(l.data_, r.data_, 0xff)
  };
}

#elif defined(__SSE3__)

//__attribute__ ((noinline))
inline auto dot(vector<float, 2> const& l,
  vector<float, 2> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return vector<float, 2>{
    _mm_hadd_ps(prod, prod)
  };
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 3> const& l,
  vector<float, 3> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return vector<float, 3>{
    _mm_hadd_ps(prod, prod)
  };
}

//__attribute__ ((noinline))
inline auto dot(vector<float, 4> const& l,
  vector<float, 4> const& r) noexcept
{
  auto prod(l.data_ * r.data_);

  prod = _mm_hadd_ps(prod, prod);

  return vector<float, 4>{
    _mm_hadd_ps(prod, prod)
  };
}

*/

#endif

namespace detail
{

namespace vector
{

/*
** 1    2    3    4
** 12   23   34   14
** 1234 1234 1234 1234
*/

template <typename T, unsigned N, std::size_t ...Is>
//__attribute__ ((noinline))
inline constexpr auto dot(typename vector_traits<T, N>::vector_type v,
  std::index_sequence<Is...>) noexcept
{
  (
    // result depends on previous shuffles
    (
      v += pow2_shuffler<T, sizeof(v) / sizeof(T), Is>(v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return v;
}

}

}

// dot product
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto dot(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return vector<T, N>{
    detail::vector::dot<T, N>(l.data_ * r.data_,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

template <typename T, unsigned N>
inline constexpr auto norm2(vector<T, N> const& v) noexcept
{
  return dot(v, v);
}

}

#endif // VXL_DOT_HPP

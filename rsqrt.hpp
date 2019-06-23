#ifndef VXL_RSQRT_HPP
# define VXL_RSQRT_HPP
# pragma once

#include "vector.hpp"

#if defined(__SSE__)

# include "xmmintrin.h"

#endif // __SSE__

#if defined(__SSE3__)

#include "pmmintrin.h"

#endif // __SSE3__

#if defined (__SSE4_1__)

#include "smmintrin.h"

#endif // __SSE4_1__

#if defined(__ARM_NEON)

#include "arm_neon.h"

#endif // __ARM_NEON

namespace vxl
{

#if defined(__ARM_NEON)

//__attribute__ ((noinline))
inline auto rsqrt(float const xx) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  vector_type const x{xx};

  r *= vrsqrts_f32(float32x2_t(x), r * r);

  return vector<float, 2>{
    vector_type(
      r * (vrsqrts_f32(float32x2_t(x), r * r))
    )
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 2> const& x) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto r(vrsqrte_f32(float32x2_t(x.data_)));

  r *= vrsqrts_f32(float32x2_t(x.data_), r * r);

  return vector<float, 2>{
    vector_type(
      r * (vrsqrts_f32(float32x2_t(x.data_), r * r))
    )
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 3> const& x) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto r(vrsqrteq_f32(float32x4_t(x.data_)));

  r *= vrsqrtsq_f32(float32x4_t(x.data_), r * r);

  return vector<float, 3>{
    vector_type(
      r * (vrsqrtsq_f32(float32x4_t(x.data_), r * r))
    )
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 4> const& x) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  auto r(vrsqrteq_f32(float32x4_t(x.data_)));

  r *= vrsqrtsq_f32(float32x4_t(x.data_), r * r);

  return vector<float, 4>{
    vector_type(
      r * (vrsqrtsq_f32(float32x4_t(x.data_), r * r))
    )
  };
}

#elif defined(__SSE__)

//__attribute__ ((noinline))
inline auto rsqrt(float const& xx) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  vector_type const x{xx};

  auto r(_mm_rsqrt_ss(x));

  return vector<float, 4>{
    r * (cvector<float, 4>(1.5f) - cvector<float, 4>(.5f) * x * r * r)
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 2> const& x) noexcept
{
  auto r(_mm_rsqrt_ps(x.data_));

  return vector<float, 2>{
    r * (cvector<float, 2>(1.5f) - cvector<float, 2>(.5f) * x.data_ * r * r)
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 3> const& x) noexcept
{
  auto r(_mm_rsqrt_ps(x.data_));

  return vector<float, 3>{
    r * (cvector<float, 3>(1.5f) - cvector<float, 3>(.5f) * x.data_ * r * r)
  };
}

//__attribute__ ((noinline))
inline auto rsqrt(vector<float, 4> const& x) noexcept
{
  auto r(_mm_rsqrt_ps(x.data_));

  return vector<float, 4>{
    r * (cvector<float, 4>(1.5f) - cvector<float, 4>(.5f) * x.data_ * r * r)
  };
}

#endif

namespace detail
{

namespace rsqrt
{

template <typename T>
inline constexpr T magic_constant;

template <>
inline constexpr auto magic_constant<float>{0x5f375a86};

template <>
inline constexpr auto magic_constant<double>{0x5fe6eb50c7b537a9};

}

}

template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto rsqrt(vector<T, N> const& xx) noexcept
{
  using int_value_type = typename vector_traits<T, N>::int_value_type;
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  // magic step
  auto r(cvector<int_value_type, N>(detail::rsqrt::magic_constant<T>) -
    (xi >> cvector<int_value_type, N>(1)));
  auto& rr((vector_type&)r);

  auto const xhalf(cvector<T, N>(T(.5)) * x);

  constexpr auto c(cvector<T, N>(T(1.5)));

  rr *= c - xhalf * rr * rr;
  rr *= c - xhalf * rr * rr;

  return vector<T, N>{rr * (c - xhalf * rr * rr)};
}

template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto sqrt(vector<T, N> const& x) noexcept
{
  return x * rsqrt(x);
}

}

#endif // VXL_RSQRT_HPP

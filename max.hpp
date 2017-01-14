#ifndef VXL_MAX_HPP
# define VXL_MAX_HPP
# pragma once

#if defined(__ARM_NEON__)

#include "arm_neon.h"

#endif // __ARM_NEON__

#include "vector.hpp"

namespace vxl
{

#if defined(__ARM_NEON__)

//__attribute__ ((noinline))
inline vector<float, 2> cmax(vector<float, 2> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const tmp(float32x2_t(v.data_));

  return {
    vector_type(vpmax_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline vector<float, 3> cmax(vector<float, 3> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto tmp(vpmax_f32(vget_low_f32(float32x4_t(v.data_)),
    vdup_lane_f32(vget_high_f32(float32x4_t(v.data_)), 0)));

  tmp = vpmax_f32(tmp, tmp);

  return {
    vector_type(vcombine_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline vector<float, 4> cmax(vector<float, 4> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  auto tmp(vpmax_f32(vget_low_f32(float32x4_t(v.data_)),
    vget_high_f32(float32x4_t(v.data_))));

  tmp = vpmax_f32(tmp, tmp);

  return {
    vector_type(vcombine_f32(tmp, tmp))
  };
}

#endif // __ARM_NEON__

namespace detail
{

namespace max
{

template <typename T, unsigned N, std::size_t ...Is>
inline typename vxl::vector_traits<T, N>::vector_type
cmax(vxl::vector<T, N> const& v, std::index_sequence<Is...> const) noexcept
{
  decltype(v.data_) result(v.data_);

  decltype(v.data_) sr;

  (
    (
      sr = detail::vector::pow2_shuffler<T, N, Is>(result,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()),
      result = select(result, sr, result > sr)
    ),
    ...
  );

  return result;
}

}

}

// max
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline vector<T, N> cmax(vector<T, N> const v) noexcept
{
  return {
    detail::max::cmax(v,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

}

#endif // VXL_MAX_HPP

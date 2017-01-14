#ifndef VXL_MIN_HPP
# define VXL_MIN_HPP
# pragma once

#include "vector.hpp"

#if defined(__ARM_NEON__)

#include "arm_neon.h"

#endif // __ARM_NEON__

namespace vxl
{

#if defined(__ARM_NEON__)

//__attribute__ ((noinline))
inline vector<float, 2> cmin(vector<float, 2> const v) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const tmp(float32x2_t(v.data_));

  return {
    vector_type(vpmin_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline vector<float, 3> cmin(vector<float, 3> const v) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto tmp(vpmin_f32(vget_low_f32(float32x4_t(v.data_)),
    vdup_lane_f32(vget_high_f32(float32x4_t(v.data_)), 0)));

  tmp = vpmin_f32(tmp, tmp);

  return {
    vector_type(vcombine_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline vector<float, 4> cmin(vector<float, 4> const v) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  auto tmp(vpmin_f32(vget_low_f32(float32x4_t(v.data_)),
    vget_high_f32(float32x4_t(v.data_))));

  tmp = vpmin_f32(tmp, tmp);

  return {
    vector_type(vcombine_f32(tmp, tmp))
  };
}

#endif // __ARM_NEON__

namespace detail
{

namespace min
{

template <typename T, unsigned N, std::size_t ...Is>
inline typename vxl::vector_traits<T, N>::vector_type
cmin(vxl::vector<T, N> const& v,
  std::index_sequence<Is...> const) noexcept
{
  decltype(v.data_) result(v.data_);

  decltype(v.data_) sr;

  (
    (
      sr = detail::vector::pow2_shuffler<T, N, Is>(result,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()),
      result = select(result, sr, result < sr)
    ),
    ...
  );

  return result;
}

}

}

// min
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline vector<T, N> cmin(vector<T, N> const v) noexcept
{
  return {
    detail::min::cmin(v,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

}

#endif // VXL_MIN_HPP

#ifndef VXL_MAX_HPP
# define VXL_MAX_HPP
# pragma once

#if defined(__ARM_NEON)

#include "arm_neon.h"

#endif // __ARM_NEON

#include "vector.hpp"

namespace vxl
{

#if defined(__ARM_NEON)

//__attribute__ ((noinline))
inline constexpr auto cmax(vector<float, 2> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const tmp(float32x2_t(v.data_));

  return vector<float, 2>{
    vector_type(vpmax_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline constexpr auto cmax(vector<float, 3> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto tmp(vpmax_f32(vget_low_f32(float32x4_t(v.data_)),
    vdup_lane_f32(vget_high_f32(float32x4_t(v.data_)), 0)));

  tmp = vpmax_f32(tmp, tmp);

  return vector<float, 3>{
    vector_type(vcombine_f32(tmp, tmp))
  };
}

//__attribute__ ((noinline))
inline constexpr auto cmax(vector<float, 4> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  auto tmp(vpmax_f32(vget_low_f32(float32x4_t(v.data_)),
    vget_high_f32(float32x4_t(v.data_))));

  tmp = vpmax_f32(tmp, tmp);

  return vector<float, 4>{
    vector_type(vcombine_f32(tmp, tmp))
  };
}

#endif // __ARM_NEON

namespace detail
{

namespace max
{

// 1 2 3 4
// m(1, 2) m(2, 3) m(3, 4) m(4, 1)
// m(1, 2, 3, 4) m(2, 3, 4, 1) m(3, 4, 1, 2) m(4, 1, 2, 3)
template <typename T, unsigned N, std::size_t ...Is>
inline constexpr auto cmax(vxl::vector<T, N> v,
  std::index_sequence<Is...>) noexcept
{
  decltype(v.data_) sr;

  (
    (
      // sr = pow2-shuffled v
      sr = detail::vector::pow2_shuffler<T, N, Is>(v.data_,
        std::make_index_sequence<sizeof(v.data_) / sizeof(T)>()
      ),
      // compute m(a, b)
      v.data_ = select(v.data_, sr, v.data_ > sr)
    ),
    ...
  );

  return v.data_;
}

}

}

// max
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto cmax(vector<T, N> const& v) noexcept
{
  return vector<T, N>{
    detail::max::cmax(v,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

}

#endif // VXL_MAX_HPP

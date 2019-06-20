#ifndef VXL_MINMAX_HPP
# define VXL_MINMAX_HPP
# pragma once

#if defined(__ARM_NEON)

#include "arm_neon.h"

#endif // __ARM_NEON

#include "vector.hpp"

namespace vxl
{

#if defined(__ARM_NEON)

//__attribute__ ((noinline))
inline constexpr auto cmin(vector<float, 2> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  auto const tmp(float32x2_t(v.data_));

  return vector<float, 2>{
    vector_type(vpmin_f32(tmp, tmp))
  };
}

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
inline constexpr auto cmin(vector<float, 3> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto tmp(vpmin_f32(vget_low_f32(float32x4_t(v.data_)),
    vdup_lane_f32(vget_high_f32(float32x4_t(v.data_)), 0)));

  tmp = vpmin_f32(tmp, tmp);

  return vector<float, 3>{
    vector_type(vcombine_f32(tmp, tmp))
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
inline constexpr auto cmin(vector<float, 4> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  auto tmp(vpmin_f32(vget_low_f32(float32x4_t(v.data_)),
    vget_high_f32(float32x4_t(v.data_))));

  tmp = vpmin_f32(tmp, tmp);

  return vector<float, 4>{
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

#endif

namespace detail
{

namespace minmax
{

// 1 2 3 4
// m(1, 2) m(2, 3) m(3, 4) m(4, 1)
// m(1, 2, 3, 4) m(2, 3, 4, 1) m(3, 4, 1, 2) m(4, 1, 2, 3)
template <typename T, unsigned N, std::size_t ...Is>
inline constexpr auto cmin(typename vector_traits<T, N>::vector_type v,
  std::index_sequence<Is...>) noexcept
{
  decltype(v) sr;

  (
    (
      sr = detail::vector::pow2_shuffler<T, N, Is>(v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ),
      v = select(v, sr, v < sr)
    ),
    ...
  );

  return v;
}

// 1 2 3 4
// m(1, 2) m(2, 3) m(3, 4) m(4, 1)
// m(1, 2, 3, 4) m(2, 3, 4, 1) m(3, 4, 1, 2) m(4, 1, 2, 3)
template <typename T, unsigned N, std::size_t ...Is>
inline constexpr auto cmax(typename vector_traits<T, N>::vector_type v,
  std::index_sequence<Is...>) noexcept
{
  decltype(v) sr;

  (
    (
      // sr = pow2-shuffled v
      sr = detail::vector::pow2_shuffler<T, N, Is>(v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ),
      // compute m(a, b)
      v = select(v, sr, v > sr)
    ),
    ...
  );

  return v;
}

}

}

// min
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto cmin(vector<T, N> const& v) noexcept
{
  return vector<T, N>{
    detail::minmax::cmin<T, N>(v.data_,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

// max
template <typename T, unsigned N>
//__attribute__ ((noinline))
inline constexpr auto cmax(vector<T, N> const& v) noexcept
{
  return vector<T, N>{
    detail::minmax::cmax<T, N>(v.data_,
      std::make_index_sequence<detail::vector::log2(N)>()
    )
  };
}

}

#endif // VXL_MINMAX_HPP

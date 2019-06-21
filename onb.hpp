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
inline constexpr auto ortho(vector<T, 2> const& v, default_tag = {}) noexcept
{
  return vector<T, 2>{-v(1), v(0)};
}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline constexpr auto ortho(vector<T, 3> const& v, default_tag = {}) noexcept
{
  using int_value_type = typename vector_traits<float, 3>::int_value_type;
  using vector_type = typename vector_traits<float, 3>::vector_type;

  auto const tmp(abs(v));

/*
  return tmp(0) > tmp(1) ?
    vector<T, 3>{-v(2), T(0), v(0)} ?
    vector<T, 3>{T(0), -v(2), v(1)};
*/

  // if abs(v(0)) > abs(v(1)), then we keep v(0) and v(2), otherwise
  // v(1) and v(2), this way the maximum vector element by abs value is always
  // retained. We negate the comparison because of select().
  return vector<T, 3>{
    select(
      vector_type{-v(2), T(0), v(0)},
      vector_type{T(0), -v(2), v(1)},
      cvector<int_value_type, 3>(-(tmp(0) > tmp(1)))
    )
  };
}

namespace detail
{

namespace onb
{

template <typename T, unsigned N, std::size_t ...Is, std::size_t ...Js>
inline constexpr auto min_element(typename vector_traits<T, N>::vector_type v,
  std::index_sequence<Is...>, std::index_sequence<Js...>) noexcept
{
  using int_value_type = typename vector_traits<T, N>::int_value_type;
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  int_vector_type j{Js...};
  int_vector_type sj;

  int_vector_type s;

  decltype(v) sv;

  (
    (
      sv = detail::vector::pow2_shuffler<T, N, Is>(v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ),
      s = v < sv,
      v = select(v, sv, s),
      sj = detail::vector::pow2_shuffler<int_value_type, N, Is>(j,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ),
      j = select(j, sj, s)
    ),
    ...
  );

  return j;
}

}

}

//////////////////////////////////////////////////////////////////////////////
template <typename T>
//__attribute__ ((noinline))
inline constexpr auto ortho(vector<T, 3> const& v, canonical_tag) noexcept
{
  // The canonical way is to find the minimum element by absolute value,
  // set it to zero and swap the other 2. This is much more involved.
  auto const i(detail::onb::min_element<T, 3>(v.data_,
      std::make_index_sequence<
        vxl::detail::vector::log2(sizeof(v) / sizeof(T))
      >(),
      std::make_index_sequence<sizeof(v) / sizeof(T)>()
    )[0]
  );

  vector<T, 3> r{};

  switch (i)
  {
    case 0:
      r = swizzled<0, 2, 1>(v);
      break;

    case 1:
      r = swizzled<2, 1, 0>(v);
      break;

    case 2:
      r = swizzled<1, 0, 2>(v);
      break;

    default:
      assert(0);
  }

  r.set_element(i, T(0));

  auto const j((i + 1) % 3);
  r.set_element(j, -r(j));

  return r;
}

}

#endif // VXL_ONB_HPP

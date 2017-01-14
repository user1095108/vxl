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

#ifndef VXL_VECTOR_HPP
# define VXL_VECTOR_HPP
# pragma once

#if __cplusplus < 201402L
# error "You need a c++14 compiler"
#endif // __cplusplus

#if defined(__SSE__)
# include "xmmintrin.h"
#endif

#if defined(__SSE2__)
# include "emmintrin.h"
#endif

#if defined(__SSE4_1__)
# include "smmintrin.h"
#endif // __SSE4_1__

#if defined(__AVX__)
# include "immintrin.h"
#endif // __AVX__

#if defined(__ARM_NEON__)
# include "arm_neon.h"
#endif // __ARM_NEON__

#include <cstring>

#include <cstddef>

#include <cstdint>

#include <ostream>

#include <utility>

namespace vxl
{

using default_tag = struct { };
using no_translation_tag = struct { };
using shift_tag = struct { };

template <class A, class ...B>
struct all_of : std::integral_constant<bool, A{} && all_of<B...>{}>
{
};

template <class A>
struct all_of<A> : std::integral_constant<bool, A{}>
{
};

struct swallow
{
  template <typename ...T>
  constexpr explicit swallow(T&& ...) noexcept
  {
  }
};

#if defined(__clang__)
template <typename U, typename V>
constexpr inline std::enable_if_t<
  !std::is_arithmetic<V>{},
  V
>
select(V const a, V const b, U const c) noexcept
{
  return V((c & U(a)) | (~c & U(b)));
}
#else
template <typename U, typename V>
constexpr inline std::enable_if_t<
  !std::is_arithmetic<V>{},
  V
>
select(V const a, V const b, U const c) noexcept
{
  return c ? a : b;
}
#endif

template <typename U, typename V>
constexpr inline std::enable_if_t<
  std::is_arithmetic<V>{},
  V
>
select(V const a, V const b, U const c) noexcept
{
  static_assert(sizeof(U) == sizeof(V), "sizeof(U) != sizeof(V)");
  auto const r((-c & (U&)(a)) | (~-c & (U&)(b)));

  return (V&)(r);
}

template <typename T, unsigned N> struct vector_traits;

template <>
struct vector_traits<std::int32_t, 1>
{
  static constexpr auto size = 1;

  using value_type = std::int32_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type;
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int32_t, 2>
{
  static constexpr auto size = 2;

  using value_type = std::int32_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint32_t;

#if defined(__SSE__)
  using vector_type = value_type __attribute__((vector_size(16)));
#else
  using vector_type = value_type __attribute__((vector_size(8)));
#endif // __SSE__
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int32_t, 3>
{
  static constexpr auto size = 3;

  using value_type = std::int32_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int32_t, 4>
{
  static constexpr auto size = 4;

  using value_type = std::int32_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::uint32_t, 1>
{
  static constexpr auto size = 1;

  using value_type = std::uint32_t;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type;
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::uint32_t, 2>
{
  static constexpr auto size = 2;

  using value_type = std::uint32_t;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

#if defined(__SSE__)
  using vector_type = value_type __attribute__((vector_size(16)));
#else
  using vector_type = value_type __attribute__((vector_size(8)));
#endif // __SSE__
#if defined(__SSE__)
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
#else
  using int_vector_type = int_value_type __attribute__((vector_size(8)));
#endif // __SSE__
};

template <>
struct vector_traits<std::uint32_t, 3>
{
  static constexpr auto size = 3;

  using value_type = std::uint32_t;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
};

template <>
struct vector_traits<std::uint32_t, 4>
{
  static constexpr auto size = 4;

  using value_type = std::uint32_t;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
};

template <>
struct vector_traits<std::int64_t, 1>
{
  static constexpr auto size = 1;

  using value_type = std::int64_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type;
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int64_t, 2>
{
  static constexpr auto size = 2;

  using value_type = std::int64_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int64_t, 3>
{
  static constexpr auto size = 3;

  using value_type = std::int64_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(32)));
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<std::int64_t, 4>
{
  static constexpr auto size = 4;

  using value_type = std::int64_t;

  using int_value_type = value_type;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(32)));
  using int_vector_type = vector_type;
};

template <>
struct vector_traits<float, 1>
{
  static constexpr auto size = 1;

  using value_type = float;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type;
  using int_vector_type = int_value_type;
};

template <>
struct vector_traits<float, 2>
{
  static constexpr auto size = 2;

  using value_type = float;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

#if defined(__SSE__)
  using vector_type = value_type __attribute__((vector_size(16)));
#else
  using vector_type = value_type __attribute__((vector_size(8)));
#endif // __SSE__
#if defined(__SSE__)
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
#else
  using int_vector_type = int_value_type __attribute__((vector_size(8)));
#endif // __SSE__
};

template <>
struct vector_traits<float, 3>
{
  static constexpr auto size = 3;

  using value_type = float;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
};

template <>
struct vector_traits<float, 4>
{
  static constexpr auto size = 4;

  using value_type = float;

  using int_value_type = std::int32_t;
  using uint_value_type = std::uint32_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
};

template <>
struct vector_traits<double, 1>
{
  static constexpr auto size = 1;

  using value_type = double;

  using int_value_type = std::int64_t;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type;
  using int_vector_type = int_value_type;
};

template <>
struct vector_traits<double, 2>
{
  static constexpr auto size = 2;

  using value_type = double;

  using int_value_type = std::int64_t;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(16)));
  using int_vector_type = int_value_type __attribute__((vector_size(16)));
};

template <>
struct vector_traits<double, 3>
{
  static constexpr auto size = 3;

  using value_type = double;

  using int_value_type = std::int64_t;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(32)));
  using int_vector_type = int_value_type __attribute__((vector_size(32)));
};

template <>
struct vector_traits<double, 4>
{
  static constexpr auto size = 4;

  using value_type = double;

  using int_value_type = std::int64_t;
  using uint_value_type = std::uint64_t;

  using vector_type = value_type __attribute__((vector_size(32)));
  using int_vector_type = int_value_type __attribute__((vector_size(32)));
};

template <typename T, typename = void>
struct deduce
{
};

template <typename T>
struct deduce<T,
  std::enable_if_t<
    !std::is_arithmetic<T>{} &&
    !std::is_array<T>{} &&
    sizeof(std::declval<T>()[0])
  >
>
{
  using value_type =
    typename std::decay<decltype(std::declval<T>()[0])>::type;

  static constexpr auto size = sizeof(T) / sizeof(value_type);

  using vector_type =
    typename vector_traits<value_type, size>::vector_type;
  using int_vector_type =
    typename vector_traits<value_type, size>::int_vector_type;
};

template <typename T>
struct deduce<T,
  std::enable_if_t<std::is_arithmetic<T>{}>
>
{
  using value_type =
    typename std::decay<decltype(std::declval<T>())>::type;

  static constexpr auto size = sizeof(T) / sizeof(value_type);

  using vector_type =
    typename vector_traits<value_type, size>::vector_type;
  using int_vector_type =
    typename vector_traits<value_type, size>::int_vector_type;
};

template <typename, typename = void>
struct is_vector : std::false_type
{
};

template <typename T>
struct is_vector<T,
  std::enable_if_t<
    bool(sizeof(deduce<T>)) &&
    !std::is_arithmetic<T>{}
  >
> : std::true_type
{
};

// convert
namespace detail
{

namespace vector
{

template <typename R, unsigned M, typename T, unsigned N, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<R, M>::vector_type
convert(typename vxl::vector_traits<T, N>::vector_type const& v,
  std::index_sequence<Is...> const) noexcept
{
  return typename vxl::vector_traits<R, M>::vector_type{R(v[Is])...};
}

template <typename R, typename T>
constexpr inline R const& convert(T const& v)
{
  return reinterpret_cast<R const&>(v);
}

}

}

template <typename R, unsigned M, typename V>
constexpr inline std::enable_if_t<
  !is_vector<V>{} && std::is_arithmetic<V>{},
  typename vxl::vector_traits<R, M>::vector_type
>
convert(V const& v) noexcept
{
  static_assert(1 == M, "M must equal 1");
  return v;
}

template <typename R, unsigned M, typename V>
constexpr inline std::enable_if_t<
  is_vector<V>{},
  typename vxl::vector_traits<R, M>::vector_type
>
convert(V const& v) noexcept
{
  return detail::vector::convert<R, M, typename deduce<V>::value_type, M>(
    v, std::make_index_sequence<M>()
  );
}

template <typename T, unsigned N>
struct vector
{
  static constexpr auto dimension = vector_traits<T, N>::size;

  using traits_type = vector_traits<T, N>;

  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  vector_type data_;

  // conversion operators
  operator decltype(data_) const& () const noexcept {return data_;}
  operator decltype((data_)) () noexcept {return data_;}

  // element access
  T operator()(unsigned const i) const noexcept {return data_[i];}
  void operator()(unsigned const i, T const v) noexcept { data_[i] = v; }

  // conversion
  auto ref() const noexcept -> decltype((data_)) {return data_;}
  auto ref() noexcept -> decltype((data_)) {return data_;}
};

template <typename T, unsigned N>
constexpr inline vector<T, N> operator+(vector<T, N> const& a,
  vector<T, N> const& b) noexcept
{
  return { a.data_ + b.data_ };
}

template <typename T, unsigned N>
constexpr inline vector<T, N> operator-(vector<T, N> const& a,
  vector<T, N> const& b) noexcept
{
  return { a.data_ - b.data_ };
}

template <typename T, unsigned N>
constexpr inline vector<T, N> operator*(vector<T, N> const& a,
  vector<T, N> const& b) noexcept
{
  return { a.data_ * b.data_ };
}

template <typename T, unsigned N>
constexpr inline vector<T, N> operator/(vector<T, N> const& a,
  vector<T, N> const& b) noexcept
{
  return { a.data_ / b.data_ };
}

template <typename T, unsigned N>
constexpr inline vector<T, N>& operator+=(vector<T, N>& a,
  vector<T, N> const& b) noexcept
{
  return a.data_ += b.data_, a;
}

template <typename T, unsigned N>
constexpr inline vector<T, N>& operator-=(vector<T, N>& a,
  vector<T, N> const& b) noexcept
{
  return a.data_ -= b.data_, a;
}

template <typename T, unsigned N>
constexpr inline vector<T, N>& operator*=(vector<T, N>& a,
  vector<T, N> const& b) noexcept
{
  return a.data_ *= b.data_, a;
}

template <typename T, unsigned N>
constexpr inline vector<T, N>& operator/=(vector<T, N>& a,
  vector<T, N> const& b) noexcept
{
  return a.data_ /= b.data_, a;
}

// assignment
namespace detail
{

namespace vector
{

template <typename T>
constexpr inline T constant_value(std::size_t const, T const v) noexcept
{
  return v;
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<T, N>::vector_type
cvector(T const c, std::index_sequence<Is...> const) noexcept
{
  return typename vxl::vector_traits<T, N>::vector_type{
    constant_value(Is, c)...
  };
}

}

}

template <typename T, unsigned N>
constexpr inline typename vxl::vector_traits<T, N>::vector_type
cvector(T const c) noexcept
{
  return detail::vector::cvector<T, N>(c,
    std::make_index_sequence<N>()
  );
}

namespace detail
{

namespace vector
{

template <unsigned N, int ...I>
struct swizzle_indices :
  std::conditional<
    sizeof...(I) < N,
    swizzle_indices<N, I..., 0>,
    std::integer_sequence<int, I...>
  >::type
{
  static_assert(sizeof...(I) <= N, "");
};

template <typename T, int ...I>
constexpr inline auto clang_swizzle(T&& a, T&& b,
  std::integer_sequence<int, I...> const) noexcept ->
  decltype(__builtin_shufflevector(a, b, I...))
{
  return __builtin_shufflevector(a, b, I...);
}

}

}

// swizzle
template <int ...I, typename T, unsigned N>
constexpr inline void swizzle(vector<T, N>& v)
{
#if defined(__clang__)
  v.data_ = detail::vector::clang_swizzle(v.data_, v.data_,
    detail::vector::swizzle_indices<sizeof(v.data_) / sizeof(T), I...>()
  );
#else
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  v.data_ = __builtin_shuffle(v.data_, v.data_, int_vector_type{I...});
#endif
}

template <int ...I, typename T, unsigned N>
constexpr inline vector<T, N> swizzled(vector<T, N> const& v) noexcept
{
#if defined(__clang__)
  return {
    detail::vector::clang_swizzle(v.data_, v.data_,
      detail::vector::swizzle_indices<sizeof(v.data_) / sizeof(T),
      (I < N ? I : I + sizeof(v) / sizeof(v(0)) - N)...>()
    )
  };
#else
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  return {
    __builtin_shuffle(v.data_, int_vector_type{I...})
  };
#endif
}

template <int ...I, typename T, unsigned N>
constexpr inline vector<T, N> swizzled(vector<T, N> const& a,
  vector<T, N> const& b) noexcept
{
#if defined(__clang__)
  return {
    detail::vector::clang_swizzle(a.data_, b.data_,
      detail::vector::swizzle_indices<sizeof(a.data_) / sizeof(T),
      (I < N ? I : I + sizeof(a) / sizeof(a(0)) - N)...>()
    )
  };
#else
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  return {
    __builtin_shuffle(a.data_, b.data_,
      int_vector_type{(I < N ? I : I + sizeof(a) / sizeof(a(0)) - N)...})
  };
#endif
}

namespace detail
{

namespace vector
{

template <typename T>
constexpr inline T pow2(T const e) noexcept
{
  return T(1) << e;
}

template <typename T>
constexpr inline T log2(T const n, T const e = 0) noexcept
{
  return pow2(e) < n ? log2(n, e + 1) : e;
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<T, N>::int_vector_type
abs_mask(std::index_sequence<Is...> const) noexcept
{
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  return ~int_vector_type(vector_type{constant_value(Is, T(-.0))...});
}

}

}

// abs
#if defined(__ARM_NEON__)
inline vector<float, 2> cabs(vector<float, 2> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 2>::vector_type;

  return {
    vector_type(vabs_f32(float32x2_t(v.data_)))
  };
}

inline vector<float, 3> cabs(vector<float, 3> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 3>::vector_type;

  return {
    vector_type(vabsq_f32(float32x4_t(v.data_)))
  };
}

inline vector<float, 4> cabs(vector<float, 4> const& v) noexcept
{
  using vector_type = typename vector_traits<float, 4>::vector_type;

  return {
    vector_type(vabsq_f32(float32x4_t(v.data_)))
  };
}

template <typename T, unsigned N>
constexpr inline vector<T, N> cabs(vector<T, N> const& v) noexcept
{
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  return {
    vector_type(int_vector_type(v.data_) &
      detail::vector::abs_mask<T, N>(std::make_index_sequence<N>())
    )
  };
}

template <typename T, unsigned N>
inline void abs(vector<T, N>& v) noexcept
{
  v = cabs(v);
}
#else
template <typename T, unsigned N>
constexpr inline vector<T, N> cabs(vector<T, N> const& v) noexcept
{
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  return {
    vector_type(int_vector_type(v.data_) &
      detail::vector::abs_mask<T, N>(std::make_index_sequence<N>())
    )
  };
}

template <typename T, unsigned N>
inline void abs(vector<T, N>& v) noexcept
{
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;
  using vector_type = typename vector_traits<T, N>::vector_type;

  v.data_ = vector_type(int_vector_type(v.data_) &
    detail::vector::abs_mask<T, N>(std::make_index_sequence<N>())
  );
}
#endif

namespace detail
{

namespace vector
{

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::size_t make_hash(std::size_t seed,
  typename vector_traits<T, N>::vector_type const& v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      seed ^= (
        convert<typename vxl::vector<T, N>::uint_value_type>(
          v[Is + 1]) + 0x9e3779b9 + (seed << 6) + (seed >> 2)
      )
    ),
    ...
  );

  return seed;
}

template <typename T, unsigned N, std::size_t I, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<T, N>::vector_type
lin_shuffler(typename vector_traits<T, N>::vector_type const& v,
  std::index_sequence<Is...> const) noexcept
{
#if defined(__clang__)
  return __builtin_shufflevector(v, v, ((I + Is) % N)...);
#else
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  return __builtin_shuffle(v, int_vector_type{((I + Is) % N)...});
#endif
}

template <typename T, unsigned N, std::size_t I, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<T, N>::vector_type
pow2_shuffler(typename vector_traits<T, N>::vector_type const& v,
  std::index_sequence<Is...> const) noexcept
{
#if defined(__clang__)
  return __builtin_shufflevector(v, v, ((pow2(I) + Is) % N)...);
#else
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

  return __builtin_shuffle(v, int_vector_type{((pow2(I) + Is) % N)...});
#endif
}

template <typename T, unsigned N, std::size_t I, std::size_t ...Is>
constexpr inline typename vxl::vector_traits<T, N>::int_vector_type
than_mask(std::index_sequence<Is...> const) noexcept
{
  // generate mask for each Is
  return typename vxl::vector_traits<T, N>::int_vector_type{
    (
      Is < N - 1 - I ? 0 : ~0
    )...
  };
}

// l = a < b = {a[i] < b[i] ? ~0 : 0}
// e = a == b = {a[i] == b[i] ? ~0 : 0}
// a < b == l[3] v e[3]l[2] v e[3]e[2]l[1] v e[3]e[2]e[1]l[0]
template <typename T, unsigned N, std::size_t ...Is>
constexpr inline bool than(typename vector_traits<T, N>::int_vector_type v,
  typename vector_traits<T, N>::int_vector_type e,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      // calculate terms
      v &= lin_shuffler<
        typename vector_traits<T, N>::int_value_type, N, Is + 1>(
          e,
          std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ) |
      than_mask<
        typename vector_traits<T, N>::int_value_type, N, Is>(
          std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ) // some of the es are masked at each iteration
    ),
    ...
  );

  // ORing all terms together
  (
    (
      v |= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );
  
  return v[0];
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline bool than_equal(
  typename vector_traits<T, N>::int_vector_type v,
  typename vector_traits<T, N>::int_vector_type e,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v &= lin_shuffler<
        typename vector_traits<T, N>::int_value_type, N, Is + 1>(
        e,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ) |
      than_mask<typename vector_traits<T, N>::int_value_type, N, Is>(
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  (
    (
      e &= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        e,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      ),
      v |= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return (e | v)[0];
}

#if defined(__SSE__)
template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  (N == 2) && (4 == sizeof(T)),
  bool
>
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !(_mm_movemask_ps(__m128(v)) & 0x3);
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  (N == 3) && (4 == sizeof(T)),
  bool
>
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !(_mm_movemask_ps(__m128(v)) & 0x7);
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  ((N == 4) && (4 == sizeof(T))) || ((N == 2) && (8 == sizeof(T))),
  bool
>
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !_mm_movemask_ps(__m128(v));
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  (16 < sizeof(typename vector_traits<T, N>::int_vector_type)),
  bool
>
all_zeros(typename vector_traits<T, N>::int_vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v |= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return !v[0];
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  (16 < sizeof(typename vector_traits<T, N>::int_vector_type)),
  bool
>
all_ones(typename vector_traits<T, N>::int_vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v &= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return v[0];
}
#elif defined(__ARM_NEON__)
template <typename T, unsigned N, std::size_t ...Is>
//__attribute__ ((noinline))
constexpr inline std::enable_if_t<((N == 2) &&
  (8 == sizeof(typename vector_traits<T, N>::int_vector_type))), bool>::type
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !int32x2_t(
    vtbl1_s8(
      int8x8_t(int32x2_t(v)),
      int8x8_t{0, 4, 4, 4}
    )
  )[0];
}

template <typename T, unsigned N, std::size_t ...Is>
//__attribute__ ((noinline))
constexpr inline std::enable_if_t<((N == 3) &&
  (16 == sizeof(typename vector_traits<T, N>::int_vector_type))), bool>::type
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !int32x2_t(
    vtbl2_s8(
      int8x8x2_t{
        int8x8_t(vget_low_s32(int32x4_t(v))),
        int8x8_t(vget_high_s32(int32x4_t(v)))
      },
      int8x8_t{0, 4, 8, 8}
    )
  )[0];
}

template <typename T, unsigned N, std::size_t ...Is>
//__attribute__ ((noinline))
constexpr inline std::enable_if_t<((N == 4)) &&
  (16 == sizeof(typename vector_traits<T, N>::int_vector_type)), bool>::type
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !int32x2_t(
    vtbl2_s8(
      int8x8x2_t{
        int8x8_t(vget_low_s32(int32x4_t(v))),
        int8x8_t(vget_high_s32(int32x4_t(v)))
      },
      int8x8_t{0, 4, 8, 12}
    )
  )[0];
}

template <typename T, unsigned N, std::size_t ...Is>
//__attribute__ ((noinline))
constexpr inline std::enable_if_t<((N == 2) &&
  (16 == sizeof(typename vector_traits<T, N>::int_vector_type))), bool>::type
all_zeros(typename vector_traits<T, N>::int_vector_type const v,
  std::index_sequence<Is...> const) noexcept
{
  return !int32x2_t(
    vtbl2_s8(
      int8x8x2_t{
        int8x8_t(vget_low_s64(int64x2_t(v))),
        int8x8_t(vget_high_s64(int64x2_t(v)))
      },
      int8x8_t{0, 4, 8, 12}
    )
  )[0];
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline std::enable_if_t<
  (16 < sizeof(typename vector_traits<T, N>::int_vector_type)),
  bool
>
all_zeros(typename vector_traits<T, N>::int_vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v |= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return !v[0];
}
#else
template <typename T, unsigned N, std::size_t ...Is>
constexpr inline bool all_ones(
  typename vector_traits<T, N>::int_vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v &= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return v[0];
}

template <typename T, unsigned N, std::size_t ...Is>
constexpr inline bool all_zeros(
  typename vector_traits<T, N>::int_vector_type v,
  std::index_sequence<Is...> const) noexcept
{
  (
    (
      v |= pow2_shuffler<typename vector_traits<T, N>::int_value_type, N, Is>(
        v,
        std::make_index_sequence<sizeof(v) / sizeof(T)>()
      )
    ),
    ...
  );

  return !v[0];
}
#endif

}

}

// comparison
// the only reason for the existance of comparison operators are the
// associative containers, for which they are a requirement
template <typename T, unsigned N>
//__attribute__ ((noinline))
constexpr inline bool operator==(
  vector<T, N> const& l, vector<T, N> const& r) noexcept
{
  return detail::vector::all_zeros<T, N>(l.data_ != r.data_,
    std::make_index_sequence<detail::vector::log2(N)>()
  );
}

template <typename T, unsigned N>
constexpr inline bool operator!=(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return !operator==(l, r);
}

template <typename T, unsigned N>
constexpr inline bool operator<(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return detail::vector::than<T, N>(
    l.data_ < r.data_,
    l.data_ == r.data_,
    std::make_index_sequence<N - 1>()
  );
}

template <typename T, unsigned N>
constexpr inline bool operator<=(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return detail::vector::than_equal<T, N>(
    l.data_ < r.data_,
    l.data_ == r.data_,
    std::make_index_sequence<N - 1>()
  );
}

template <typename T, unsigned N>
constexpr inline bool operator>(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return detail::vector::than<T, N>(
    l.data_ > r.data_,
    l.data_ == r.data_,
    std::make_index_sequence<N - 1>()
  );
}

template <typename T, unsigned N>
constexpr inline bool operator>=(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
  return detail::vector::than_equal<T, N>(
    l.data_ > r.data_,
    l.data_ == r.data_,
    std::make_index_sequence<N - 1>()
  );
}

template <typename T, unsigned N>
//__attribute__ ((noinline))
constexpr inline bool all(vector<T, N> const& l)
{
  return detail::vector::all_zeros<T, N>(
    l.data_ == cvector<T, N>(T(0)),
    std::make_index_sequence<detail::vector::log2(N)>()
  );
}

template <typename T, unsigned N>
//__attribute__ ((noinline))
constexpr inline bool any(vector<T, N> const& l) noexcept
{
  return !detail::vector::all_zeros<T, N>(
    l.data_ != cvector<T, N>(T(0)),
    std::make_index_sequence<detail::vector::log2(N)>()
  );
}

// zero
template <typename T, unsigned N>
constexpr inline void zero(vector<T, N>& v) noexcept
{
  v.data_ = decltype(v.data_){};
}

// stream operators
template <typename T, unsigned N>
std::ostream& operator<<(std::ostream& os, vector<T, N> const& v)
{
  os << '(';

  for (unsigned i{}; i != N - 1; ++i)
  {
    os << v.data_[i] << ", ";
  }

  return os << v.data_[N - 1] << ')';
}

}

namespace std
{

template <typename T, unsigned N>
struct hash<vxl::vector<T, N> >
{
  constexpr size_t operator()(vxl::vector<T, N> const& v) const noexcept
  {
    return vxl::detail::vector::make_hash(
      vxl::detail::vector::convert<
        typename vxl::vector<T, N>::uint_value_type
      >(v.data_[0]),
      v.data_,
      std::make_index_sequence<N - 1>()
    );
  }
};

}

#endif // VXL_VECTOR_HPP

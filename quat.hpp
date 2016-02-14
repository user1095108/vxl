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

#ifndef VXL_QUAT_HPP
# define VXL_QUAT_HPP
# pragma once

#include "matrix.hpp"

namespace vxl
{

template <typename T>
struct quat
{
  typename vector_traits<T, 4>::vector_type data_;

  // conversion operators
  operator decltype(data_) const& () const noexcept { return data_; }
  operator decltype((data_)) () noexcept { return data_; }

  // conversion
  auto ref() const noexcept -> decltype((data_)) { return data_; }
  auto ref() noexcept -> decltype((data_)) { return data_; }

  // element access
  T operator()(unsigned const i) const noexcept { return data_[i]; }
  void operator()(unsigned const i, T const v) noexcept { data_[i] = v; }
};

template <typename T>
constexpr inline quat<T> operator+(quat<T> const& a,
  quat<T> const& b) noexcept
{
  return { a.data_ + b.data_ };
}

template <typename T>
constexpr inline quat<T> operator-(quat<T> const& a,
  quat<T> const& b) noexcept
{
  return { a.data_ - b.data_ };
}

template <typename T>
//__attribute__ ((noinline))
inline quat<T> operator*(quat<T> const& l, quat<T> const& r) noexcept
{
//  l(0)r(3) + l(1)r(2) - l(2)r(1) + l(3)r(0)
// -l(0)r(2) + l(1)r(3) + l(2)r(0) + l(3)r(1)
//  l(0)r(1) - l(1)r(0) + l(2)r(3) + l(3)r(2)
// -l(0)r(0) - l(1)r(1) - l(2)r(2) + l(3)r(3)
  using int_value_type = typename vector_traits<T, 4>::int_value_type;
  using int_vector_type = typename vector_traits<T, 4>::int_vector_type;

  constexpr int_vector_type const mask{
    0, 0, 0, 1 << (8 * sizeof(int_value_type) - 1)
  };

#if defined(__clang__)
  auto const t1(
    l.data_ *
    __builtin_shufflevector(r.data_, r.data_, 3, 3, 3, 3)
  );
  auto const t2(
    __builtin_shufflevector(l.data_, l.data_, 1, 2, 0, 2) *
    __builtin_shufflevector(r.data_, r.data_, 2, 0, 1, 2)
  );
  auto const t3(
    __builtin_shufflevector(l.data_, l.data_, 3, 3, 3, 1) *
    __builtin_shufflevector(r.data_, r.data_, 0, 1, 2, 1)
  );
  auto const t4(
    __builtin_shufflevector(l.data_, l.data_, 2, 0, 1, 0) *
    __builtin_shufflevector(r.data_, r.data_, 1, 2, 0, 0)
  );
#else
  auto const t1(
    l.data_ *
    __builtin_shuffle(r.data_, int_vector_type{3, 3, 3, 3})
  );
  auto const t2(
    __builtin_shuffle(l.data_, int_vector_type{1, 2, 0, 2}) *
    __builtin_shuffle(r.data_, int_vector_type{2, 0, 1, 2})
  );
  auto const t3(
    __builtin_shuffle(l.data_, int_vector_type{3, 3, 3, 1}) *
    __builtin_shuffle(r.data_, int_vector_type{0, 1, 2, 1})
  );
  auto const t4(
    __builtin_shuffle(l.data_, int_vector_type{2, 0, 1, 0}) *
    __builtin_shuffle(r.data_, int_vector_type{1, 2, 0, 0})
  );
#endif

  return {
    t1 +
    decltype(t2)(int_vector_type(t2 + t3) ^ mask) -
    t4
  };
}

template <typename T>
constexpr inline quat<T>& operator+=(quat<T>& a, quat<T> const& b) noexcept
{
  return a.data_ += b.data_, a;
}

template <typename T>
constexpr inline quat<T>& operator-=(quat<T>& a, quat<T> const& b) noexcept
{
  return a.data_ -= b.data_, a;
}

// comparison
template <typename T>
inline bool operator==(quat<T> const& l, quat<T> const& r) noexcept
{
  return detail::vector::equal<T, 4>(l.data_ == r.data_,
    ::std::make_index_sequence<detail::vector::log2(4)>()
  );
}

template <typename T>
inline bool operator!=(quat<T> const& l, quat<T> const& r) noexcept
{
  return !operator==(l, r);
}

// scalar part
template <typename T>
constexpr inline T scalar(quat<T> const& x) noexcept
{
  return x.data_[3];
}

namespace detail
{

namespace quat
{

template <typename T, unsigned N, ::std::size_t ...Is>
constexpr inline auto scalar_vector(::vxl::quat<T> const& x,
  ::std::index_sequence<Is...> const) noexcept ->
  typename vector_traits<T, N>::vector_type
{
  using int_vector_type = typename vector_traits<T, N>::int_vector_type;

#if defined(__clang__)
  return __builtin_shufflevector(x.data_, x.data_, (Is, 3)...);
#else
  return __builtin_shuffle(x.data_, int_vector_type{(Is, 3)...});
#endif
}

}

}

template <typename T, unsigned N>
constexpr inline auto scalar_vector(quat<T> const& x) noexcept ->
  typename vector_traits<T, N>::vector_type
{
  return detail::quat::scalar_vector<T, N>(x,
    ::std::make_index_sequence<sizeof(x.data_) / sizeof(x.data_[0])>()
  );
}

// vector part
template <typename T>
constexpr inline vector<T, 3> vec(quat<T> const& x) noexcept
{
  return { x.data_ };
}

// conjugation
template <typename T>
inline void conjugate(quat<T>& x) noexcept
{
  using int_vector_type = typename vector_traits<T, 4>::int_vector_type;
  using vector_type = typename vector_traits<T, 4>::vector_type;
  
  x.data_ = vector_type(
    int_vector_type(x.data_) ^
    int_vector_type(vector_type{T(-.0), T(-.0), T(-.0), T(.0)})
  );
}

template <typename T>
constexpr inline quat<T> conjugated(quat<T> const& x) noexcept
{
  using int_vector_type = typename vector_traits<T, 4>::int_vector_type;
  using vector_type = typename vector_traits<T, 4>::vector_type;

  return {
    vector_type(
      int_vector_type(x.data_) ^
      int_vector_type(vector_type{T(-.0), T(-.0), T(-.0), T(.0)})
    )
  };
}

template <typename T>
inline vector<T, 4> norm2(quat<T> const& x) noexcept
{
  return cdot(vector<T, 4>{x.data_}, vector<T, 4>{x.data_});
}

}

// stream operators
template <typename T>
::std::ostream& operator<<(::std::ostream& os, ::vxl::quat<T> const& v)
{
  os << '(';

  for (unsigned i{}; i != 3; ++i)
  {
    os << v.data_[i] << ", ";
  }

  return os << v.data_[3] << ')';
}

#endif // VXL_QUAT_HPP

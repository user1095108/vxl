#ifndef VXL_MATRIX_HPP
# define VXL_MATRIX_HPP
# pragma once

#include <cassert>

#include "dot.hpp"

namespace vxl
{

namespace detail
{

namespace matrix
{

template <unsigned I, typename D, typename S, std::size_t ...Is>
inline constexpr void copy(D& dst, S const& src, std::index_sequence<Is...>)
{
  ((dst[Is][I] = src[Is]), ...);
}

template <typename D, unsigned I, typename S, std::size_t ...Is>
inline constexpr auto sample(S const& src, std::index_sequence<Is...>)
{
  return D{(src[Is][I])...};
}

}

}

template <typename T, unsigned M, unsigned N>
struct matrix
{
  static constexpr auto rows = M;
  static constexpr auto cols = N;

#ifndef VXL_ROW_MAJOR
  typename vector_traits<T, N>::vector_type data_[M];
#else
  typename vector_traits<T, M>::vector_type data_[N];
#endif // VXL_ROW_MAJOR

  constexpr auto operator()(unsigned const i, unsigned const j) const noexcept
  {
    return get_element(i, j);
  }

  // assignment
  auto& operator=(std::initializer_list<T> const l) noexcept
  {
    assert(M * N == l.size());
    auto k(l.begin());

    for (unsigned i{}; i != M; ++i)
    {
      for (unsigned j{}; j != N; ++j)
      {
#ifndef VXL_ROW_MAJOR
        data_[j][i] = *k++;
#else
        data_[i][j] = *k++;
#endif // VXL_ROW_MAJOR
      }
    }

    return *this;
  }

  // element access
  constexpr auto get_element(unsigned const i,
    unsigned const j) const noexcept
  {
#ifndef VXL_ROW_MAJOR
    return data_[j][i];
#else
    return data_[i][j];
#endif // VXL_ROW_MAJOR
  }

  constexpr void set_element(unsigned const i, unsigned const j,
    T const v) noexcept
  {
#ifndef VXL_ROW_MAJOR
    data_[j][i] = v;
#else
    data_[i][j] = v;
#endif // VXL_ROW_MAJOR
  }

  template <unsigned I>
  constexpr auto row() const noexcept
  {
#ifndef VXL_ROW_MAJOR
    return detail::matrix::sample<
      typename vector_traits<T, N>::vector_type, I
    >(data_, std::make_index_sequence<N>());
#else
    return data_[i];
#endif // VXL_ROW_MAJOR
  }

  template <unsigned I>
  constexpr void set_row(
    typename vector_traits<T, N>::vector_type const& v) noexcept
  {
#ifndef VXL_ROW_MAJOR
    detail::matrix::copy<I>(data_,
      v,
      std::make_index_sequence<N>()
    );
#else
    data_[i] = v;
#endif // VXL_ROW_MAJOR
  }

  template <unsigned I>
  constexpr void set_row(vector<T, N> const& v) noexcept
  {
#ifndef VXL_ROW_MAJOR
    detail::matrix::copy<I>(data_,
      v.data_,
      std::make_index_sequence<N>()
    );
#else
    data_[i] = v.data_;
#endif // VXL_ROW_MAJOR
  }

  template <unsigned I, typename ...A,
    typename = typename std::enable_if_t<
      all_of<
        std::is_same<T, std::decay_t<A>>...
      >{}
    >
  >
  constexpr void set_row(A const... a) noexcept
  {
    set_row<I>(typename vector_traits<T, N>::vector_type{a...});
  }

  template <unsigned J>
  constexpr auto col() const noexcept
  {
#ifndef VXL_ROW_MAJOR
    return data_[J];
#else
    return detail::matrix::sample<
      typename vector_traits<T, M>::vector_type, J
    >(data_, std::make_index_sequence<M>());
#endif // VXL_ROW_MAJOR
  }

  template <unsigned J>
  constexpr void set_col(
    typename vector_traits<T, M>::vector_type const& v) noexcept
  {
#ifndef VXL_ROW_MAJOR
    data_[J] = v;
#else
    detail::matrix::copy<J>(data_, v,
      std::make_index_sequence<M>()
    );
#endif // VXL_ROW_MAJOR
  }

  template <unsigned J>
  constexpr void set_col(vector<T, M> const& v) noexcept
  {
#ifndef VXL_ROW_MAJOR
    data_[J] = v.data_;
#else
    detail::matrix::copy<J>(data_, v.data_,
      std::make_index_sequence<M>()
    );
#endif // VXL_ROW_MAJOR
  }

  template <unsigned I, typename ...A,
    typename = typename std::enable_if_t<
      all_of<
        std::is_same<T, std::decay_t<A>>...
      >{}
    >
  >
  constexpr void set_col(A const... a) noexcept
  {
    set_col<I>(typename vector_traits<T, N>::vector_type{a...});
  }

  // ref
  constexpr auto& ref() noexcept {return data_;}
  constexpr auto& ref() const noexcept {return data_;}
};

template <unsigned M, unsigned N, typename ...A,
  typename = std::enable_if_t<
    all_of<
      std::is_same<std::decay_t<front_t<A...>>, std::decay_t<A>>...
    >{} &&
    std::is_arithmetic<std::decay_t<front_t<A...>>>{}
  >
>
inline constexpr auto make_matrix(A const ...a) noexcept
{
  static_assert(M * N == sizeof...(A));
  matrix<std::decay_t<front_t<A...>>, M, N> r{};

  unsigned i{};

  (
    (
      r.set_element(i / M, i % N, a),
      ++i
    ),
    ...
  );

  return r;
}

namespace detail
{

namespace matrix
{

template <unsigned I, typename T, unsigned M, unsigned N, std::size_t ...Is>
constexpr inline auto sample(vxl::matrix<T, M, N> const& m,
  std::index_sequence<Is...> const) noexcept ->
  typename vector_traits<T, M>::vector_type
{
  return typename vector_traits<T, M>::vector_type{m.data_[Is][I]...};
}

template <typename T, unsigned M, unsigned N, std::size_t ...Is>
constexpr inline auto sample(vxl::matrix<T, M, N> const& m,
  unsigned const i, std::index_sequence<Is...> const) noexcept ->
  typename vector_traits<T, M>::vector_type
{
  return typename vector_traits<T, M>::vector_type{m.data_[Is][i]...};
}

}

}

template <unsigned I, typename T, unsigned M, unsigned N>
constexpr inline vector<T, M> row(matrix<T, M, N> const& m) noexcept
{
#ifndef VXL_ROW_MAJOR
  return {
    detail::matrix::sample<I>(m, I, std::make_index_sequence<N>())
  };
#else
  return {
    l.data_[I]
  };
#endif // VXL_ROW_MAJOR
}

template <typename T, unsigned M, unsigned N>
constexpr inline vector<T, M> row(matrix<T, M, N> const& m,
  unsigned const i) noexcept
{
#ifndef VXL_ROW_MAJOR
  return {
    detail::matrix::sample(m, i, std::make_index_sequence<N>())
  };
#else
  return {
    l.data_[i]
  };
#endif // VXL_ROW_MAJOR
}

template <unsigned J, typename T, unsigned M, unsigned N>
constexpr inline vector<T, M> col(matrix<T, M, N> const& m) noexcept
{
#ifndef VXL_ROW_MAJOR
  return {
    m.data_[J]
  };
#else
  return {
    detail::matrix::sample<J>(m, std::make_index_sequence<M>())
  };
#endif // VXL_ROW_MAJOR
}

template <typename T, unsigned M, unsigned N>
constexpr inline vector<T, M> col(matrix<T, M, N> const& m,
  unsigned const j) noexcept
{
#ifndef VXL_ROW_MAJOR
  return {
    m.data_[j]
  };
#else
  return {
    detail::matrix::sample(m, j, std::make_index_sequence<M>())
  };
#endif // VXL_ROW_MAJOR
}

// arithmetic operations
template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline auto operator+(matrix<T, M, N> const& l,
  matrix<T, M, N> const& r) noexcept
{
  decltype(l + r) result;

#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    result.data_[j] = l.data_[j] + r.data_[j];
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    result.data_[i] = l.data_[i] + r.data_[i];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N> operator-(matrix<T, M, N> const& l,
  matrix<T, M, N> const& r) noexcept
{
  decltype(l - r) result;

#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    result.data_[j] = l.data_[j] - r.data_[j];
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    result.data_[i] = l.data_[i] - r.data_[i];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M1, unsigned N1, unsigned M2, unsigned N2>
//__attribute__ ((noinline))
inline matrix<T, M1, N2> operator*(matrix<T, M1, N1> const& l,
  matrix<T, M2, N2> const& r) noexcept
{
  static_assert(N1 == M2);
  decltype(l * r) result;

#ifndef VXL_ROW_MAJOR
  for (unsigned i{}; i != N2; ++i)
  {
    result.data_[i] = cvector<T, M1>(r.data_[i][0]) * l.data_[0];

    for (unsigned j{1}; j != N1; ++j)
    {
      result.data_[i] += cvector<T, M1>(r.data_[i][j]) * l.data_[j];
    }
  }
#else
  for (unsigned i{}; i != M1; ++i)
  {
    result.data_[i] = cvector<T, N2>(l.data_[i][0]) * r.data_[0];

    for (unsigned j{1}; j != M2; ++j)
    {
      result.data_[i] += cvector<T, N2>(l.data_[i][j]) * r.data_[j];
    }
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M1, unsigned N1, unsigned N>
//__attribute__ ((noinline))
inline vector<T, M1> operator*(matrix<T, M1, N1> const& l,
  vector<T, N> const& r) noexcept
{
  static_assert(N1 == N, "");

#ifndef VXL_ROW_MAJOR
  decltype(l * r) result{cvector<T, M1>(r.data_[0]) * l.data_[0]};

  for (unsigned j{1}; j != N; ++j)
  {
    result.data_ += cvector<T, M1>(r.data_[j]) * l.data_[j];
  }
#else
  decltype(l * r) result;

  for (unsigned i{}; i != M1; ++i)
  {
    result.data_[i] = dot({l.data_[i]}, r).data_[0];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned N, unsigned M2, unsigned N2>
//__attribute__ ((noinline))
inline vector<T, N2> operator*(vector<T, N> const& l,
  matrix<T, M2, N2> const& r) noexcept
{
  static_assert(N == M2);

#ifndef VXL_ROW_MAJOR
  decltype(l * r) result;

  for (unsigned j{}; j != N2; ++j)
  {
    result.data_[j] = dot(l, {r.data_[j]}).data_[0];
  }
#else
  decltype(l * r) result{cvector<T, N2>(l.data_[0]) * r.data_[0]};

  for (unsigned j{1}; j != M2; ++j)
  {
    result.data_ += cvector<T, N2>(l.data_[j]) * r.data_[j];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N> operator*(T const l,
  matrix<T, M, N> const& r) noexcept
{
  decltype(l * r) result;

#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    result.data_[j] = vxl::cvector<T, M>(l) * r.data_[j];
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    result.data_[i] = vxl::cvector<T, N>(l) * r.data_[i];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N> operator*(matrix<T, M, N> const& l,
  T const r) noexcept
{
  return operator*(r, l);
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N> operator/(matrix<T, M, N> const& l,
  T const r) noexcept
{
  decltype(l * r) result;

  auto const invr(T(1) / r);

#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    result.data_[j] = vxl::cvector<T, M>(invr) * l.data_[j];
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    result.data_[i] = vxl::cvector<T, N>(invr) * l.data_[i];
  }
#endif // VXL_ROW_MAJOR

  return result;
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N>& operator*=(matrix<T, M, N>& l,
  matrix<T, M, N> const& r) noexcept
{
#ifndef VXL_ROW_MAJOR
  // iterate over all columns of r
  for (unsigned i{}; i != N; ++i)
  {
    // capture i-th column
    auto const tmp(r.data_[i]);

    l.data_[i] = cvector<T, M>(tmp[0]) * l.data_[0];

    for (unsigned j{1}; j != N; ++j)
    {
      l.data_[i] += cvector<T, M>(tmp[j]) * l.data_[j];
    }
  }
#else
  // iterate over all rows of l
  for (unsigned i{}; i != M; ++i)
  {
    // capture i-th row
    auto const tmp(r.data_[i]);

    l.data_[i] = cvector<T, N>(tmp[0]) * r.data_[0];

    for (unsigned j{1}; j != M; ++j)
    {
      l.data_[i] += cvector<T, N>(tmp[j]) * r.data_[j];
    }
  }
#endif // VXL_ROW_MAJOR

  return l;
}

template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline matrix<T, M, N>& operator*=(matrix<T, M, N>& l, T const r) noexcept
{
#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    l.data_[j] *= vxl::cvector<T, M>(r);
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    l.data_[i] *= vxl::cvector<T, N>(r);
  }
#endif // VXL_ROW_MAJOR

  return l;
}

// comparison
template <typename T, unsigned M, unsigned N>
//__attribute__ ((noinline))
inline bool operator==(matrix<T, M, N> const& l,
  matrix<T, M, N> const& r) noexcept
{
#ifndef VXL_ROW_MAJOR
  typename vector_traits<T, M>::int_vector_type result(
    l.data_[0] != r.data_[0]
  );

  for (unsigned j{1}; j != N; ++j)
  {
    result |= l.data_[j] != r.data_[j];
  }

  return detail::vector::all_zeros<T, M>(result,
    std::make_index_sequence<detail::vector::log2(M)>()
  );
#else
  typename vector_traits<T, N>::int_vector_type result(
    l.data_[0] != r.data_[0]
  );

  for (unsigned i{1}; i != M; ++i)
  {
    result |= l.data_[i] != r.data_[i];
  }

  return detail::vector::all_zeros<T, N>(result,
    std::make_index_sequence<detail::vector::log2(N)>()
  );
#endif // VXL_ROW_MAJOR
}

namespace detail
{

namespace matrix
{

template <typename T, unsigned M, std::size_t ...Is>
inline constexpr auto identity(std::index_sequence<Is...>) noexcept
{
  vxl::matrix<T, M, M> r{};

  (
    r.set_element(Is, Is, T(1)),
    ...
  );

  return r;
}

template <typename T, unsigned M, std::size_t ...Is>
inline constexpr auto diag(vxl::vector<T, M> const& v,
  std::index_sequence<Is...>) noexcept
{
  vxl::matrix<T, M, M> r{};

  (
    r.set_element(Is, Is, v(Is)),
    ...
  );

  return r;
}

}

}

// diagonal
template <typename T, unsigned M>
inline constexpr auto diag(vxl::vector<T, M> const& v) noexcept
{
  return detail::matrix::diag(v, std::make_index_sequence<M>());
}

// identity
template <typename T, unsigned M, unsigned N>
inline constexpr void identity(matrix<T, M, N>& m) noexcept
{
  static_assert(M == N);
  m = detail::matrix::identity<T, M>(std::make_index_sequence<M>());
}

template <typename T, unsigned M, unsigned N>
inline constexpr auto identity() noexcept
{
  static_assert(M == N);
  return detail::matrix::identity<T, M>(std::make_index_sequence<M>());
}

// transposition
template <typename T, unsigned M, unsigned N>
inline constexpr auto trans(matrix<T, M, N> const& m) noexcept
{
  matrix<T, N, M> result{};

#ifndef VXL_ROW_MAJOR
  for (unsigned j{}; j != N; ++j)
  {
    for (unsigned i{}; i != M; ++i)
    {
      result.data_[i][j] = m.data_[j][i];
    }
  }
#else
  for (unsigned i{}; i != M; ++i)
  {
    for (unsigned j{}; j != N; ++j)
    {
      result.data_[j][i] = m.data_[i][j];
    }
  }
#endif // VXL_ROW_MAJOR

  return result;
}

// zero
template <typename T, unsigned M, unsigned N>
inline constexpr auto zero() noexcept
{
  static_assert(M == N);
  return matrix<T, M, N>{};
}

template <typename T, unsigned M, unsigned N>
inline constexpr void zero(matrix<T, M, N>& m) noexcept
{
  static_assert(M == N);
  m = zero<T, M, N>();
}

template <typename T, unsigned M, unsigned N>
std::ostream& operator<<(std::ostream& os, matrix<T, M, N> const& m)
{
  for (unsigned i{}; i != M - 1; ++i)
  {
    os << (i ? ' ' : '[');

    for (unsigned j{}; j != N - 1; ++j)
    {
      os << m(i, j) << ", ";
    }

    os << m(i, N - 1) << '\n';
  }

  os << ' ';

  for (unsigned j{}; j != N - 1; ++j)
  {
    os << m(M - 1, j) << ", ";
  }

  return os << m(M - 1, N - 1) << ']';
}

}

#endif // VXL_MATRIX_HPP

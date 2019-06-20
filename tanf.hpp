#ifndef VXL_TANF_HPP
# define VXL_TANF_HPP
# pragma once

#include "sinf.hpp"

namespace vxl
{

template <unsigned N>
inline vector<float, N> tan(vector<float, N> xx) noexcept
{
  using int_value_type = typename vector_traits<float, N>::int_value_type;
  using int_vector_type = typename vector_traits<float, N>::int_vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  auto sign(int_vector_type(x) & cvector<int_value_type, N>(1 << 31));

  xi &= cvector<int_value_type, N>(~(1 << 31));

  auto j(convert<int_value_type, N>(
    cvector<float, N>(detail::constantsf::FOPI) * x));

  j += j & cvector<int_value_type, N>(1);

  auto y(convert<float, N>(j));

  auto const z(((x - y * cvector<float, N>(detail::constantsf::DP1)) -
    y * cvector<float, N>(detail::constantsf::DP2)) -
    y * cvector<float, N>(detail::constantsf::DP3));

  auto const zz(z * z);

  y = select(
    (((((cvector<float, N>(9.38540185543e-3f) * zz +
      cvector<float, N>(3.11992232697e-3f)) * zz +
      cvector<float, N>(2.44301354525e-2f)) * zz +
      cvector<float, N>(5.34112807005e-2f)) * zz +
      cvector<float, N>(1.33387994085e-1f)) * zz +
      cvector<float, N>(3.33331568548e-1f)) * zz * z + z,
    z,
    int_vector_type(x > cvector<float, N>(1e-4f))
  );

  auto r(select(
    cvector<float, N>(-1.f) / y,
    y,
    int_vector_type(cvector<int_value_type, N>(0) <
      (j & cvector<int_value_type, N>(2))))
  );
  (int_vector_type&)(r) ^= sign;

  return {r};
}

template <unsigned N>
inline vector<float, N> cot(vector<float, N> xx) noexcept
{
  using int_value_type = typename vector_traits<float, N>::int_value_type;
  using int_vector_type = typename vector_traits<float, N>::int_vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  auto sign(int_vector_type(x) & cvector<int_value_type, N>(1 << 31));

  xi &= cvector<int_value_type, N>(~(1 << 31));

  auto j(convert<int_value_type, N>(
    cvector<float, N>(detail::constantsf::FOPI) * x));

  j += j & cvector<int_value_type, N>(1);

  auto y(convert<float, N>(j));

  auto const z(((x - y * cvector<float, N>(detail::constantsf::DP1)) -
    y * cvector<float, N>(detail::constantsf::DP2)) -
    y * cvector<float, N>(detail::constantsf::DP3));

  auto const zz(z * z);

  y = select(
    (((((cvector<float, N>(9.38540185543e-3f) * zz +
      cvector<float, N>(3.11992232697e-3f)) * zz +
      cvector<float, N>(2.44301354525e-2f)) * zz +
      cvector<float, N>(5.34112807005e-2f)) * zz +
      cvector<float, N>(1.33387994085e-1f)) * zz +
      cvector<float, N>(3.33331568548e-1f)) * zz * z + z,
    z,
    int_vector_type(x > cvector<float, N>(1e-4f))
  );

  auto r(select(
    -y,
    cvector<float, N>(1.f) / y,
    int_vector_type(cvector<int_value_type, N>(0) <
      (j & cvector<int_value_type, N>(2))))
  );
  (int_vector_type&)(r) ^= sign;

  return {r};
}

}

#endif // VXL_TANF_HPP

#include <cstdlib>

#include <iostream>

#include "cross.hpp"

#include "dot.hpp"

#include "onb.hpp"

#include "minmax.hpp"

#include "rsqrt.hpp"

#include "sinf.hpp"

#include "tanf.hpp"

int main()
{
  auto a(vxl::make_vector(1.f, 2.f, 3.f));
  vxl::vector<float, 3> b{1.f, 1.f, 1.f};

  std::cout << (a == a) << std::endl;
  std::cout << (a == b) << std::endl;
  std::cout << (a >= b) << std::endl;
  std::cout << (a <= b) << std::endl;

  std::cout << vxl::all(a) << std::endl;
  std::cout << vxl::any(b) << std::endl;

  std::cout << vxl::dot(a, b).data_[0] << std::endl;
  std::cout << vxl::cross(a, b) << std::endl;
  std::cout << vxl::sqrt(a) << std::endl;

  vxl::vector<float, 4> c{1.f, 2.f, -3.f, 4.f};

  {
    auto const sc(vxl::sincos(c));
    std::cout << sc.first << std::endl;
    std::cout << sc.second << std::endl;
  }

  std::cout << vxl::sin(c) << std::endl;
  std::cout << vxl::cos(c) << std::endl;
  std::cout << vxl::tan(c) << std::endl;
  std::cout << vxl::cot(c) << std::endl;

  {
    vxl::vector<float, 1> d{1.f};

    auto const sc(vxl::sincos(d));
    std::cout << sc.first(0) << std::endl;
    std::cout << sc.second(0) << std::endl;
  }

  {
    vxl::vector<double, 1> d{3.};
    std::cout << vxl::sqrt(d)(0) << std::endl;
  }

  std::cout << vxl::max(a) << std::endl;
  std::cout << vxl::min(a) << std::endl;

  vxl::vector<float, 3> const u{3.0f, 2.0f, 1.0f};
  std::cout << vxl::ortho(u) << std::endl;
  std::cout << vxl::ortho(u, vxl::canonical_tag{}) << std::endl;

  vxl::vector<double, 4> e{1., 2., 3., 4.};
  std::cout << (e == e) << std::endl;
  std::cout << vxl::norm2(e) << std::endl;

  std::cout << vxl::dot(
      vxl::vector<float, 2>{1, 2},
      vxl::vector<float, 2>{3, 4}
    ).data_[0] <<
    std::endl;

  return 0;
}

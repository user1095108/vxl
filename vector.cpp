#include <cstdlib>

#include <iostream>

#include "cross.hpp"

#include "dot.hpp"

#include "onb.hpp"

#include "max.hpp"

#include "min.hpp"

#include "sqrt.hpp"

#include "sinf.hpp"

#include "tanf.hpp"

int main()
{
  ::vxl::vector<float, 3> a{1.f, 2.f, 3.f};
  ::vxl::vector<float, 3> b{1.f, 1.f, 1.f};

  ::std::cout << (a == a) << ::std::endl;
  ::std::cout << (a == b) << ::std::endl;
  ::std::cout << (a >= b) << ::std::endl;
  ::std::cout << (a <= b) << ::std::endl;

  ::std::cout << ::vxl::all(a) << ::std::endl;
  ::std::cout << ::vxl::any(b) << ::std::endl;

  ::std::cout << ::vxl::cdot(a, b).data_[0] << ::std::endl;
  ::std::cout << ::vxl::ccross(a, b) << ::std::endl;
  ::std::cout << ::vxl::csqrt(a) << ::std::endl;

  ::vxl::vector<float, 4> c{1.f, 2.f, -3.f, 4.f};
  auto const sincos(::vxl::csincos(c));
  ::std::cout << sincos.first << ::std::endl;
  ::std::cout << sincos.second << ::std::endl;

  ::std::cout << ::vxl::csin(c) << ::std::endl;
  ::std::cout << ::vxl::ccos(c) << ::std::endl;
  ::std::cout << ::vxl::ctan(c) << ::std::endl;
  ::std::cout << ::vxl::ccot(c) << ::std::endl;

  ::vxl::vector<float, 1> d{1.f};
  auto const sc(::vxl::csincos(d));
  ::std::cout << sc.first.data_ << ::std::endl;
  ::std::cout << sc.second.data_ << ::std::endl;

  ::std::cout << ::vxl::cmax(a) << ::std::endl;
  ::std::cout << ::vxl::cmin(a) << ::std::endl;

  ::vxl::vector<float, 3> const u{3.0f, 2.0f, 1.0f};
  ::std::cout << ::vxl::cortho(u) << ::std::endl;

  return 0;
}

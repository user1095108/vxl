#include <cstdlib>

#include <iostream>

#include "rotation.hpp"

int main()
{
  vxl::quat<float> a{0.f, 1.f, 0.f, 1.f};
  vxl::quat<float> b{.5f, .5f, .75f, 1.f};

  std::cout << a * b << std::endl;
//(.87 - .5k)(i + 2j)(.87 + .5k) = (2.25, .14, 0)
  std::cout << vxl::rotated(vxl::quat<float>{.0f, .0f, -.5f, .87f},
      vxl::vector<float, 3u>{1.f, 2.f, 0.f}) <<
    std::endl;
  std::cout << vxl::quat<float>{.0f, .0f, -.5f, .87f} *
    vxl::quat<float>{1.f, 2.f, 0.f} *
    vxl::quat<float>{.0f, .0f, .5f, .87f} <<
    std::endl;

  std::cout <<
    vxl::aa2q(vxl::vector<float, 3>{1.f, .0f, .0f}, 2.f) <<
    std::endl;

  std::cout <<
    vxl::norm2(vxl::quat<float>{1, 1, 1, 1}) <<
    std::endl;

  return 0;
}

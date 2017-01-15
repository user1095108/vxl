#include <iostream>

#include "matrix.hpp"

#include "matrix_determinant.hpp"

int main()
{
  auto m2(vxl::make_matrix<float, 2, 2>(1.0f, 2.0f, 3.0f, 6.0f));
  auto n2(vxl::make_matrix<float, 2, 2>(-2.0f, -2.0f, 1.0f, 1.0f));

  std::cout << m2 << std::endl;
  std::cout << n2 << std::endl;
  std::cout << (m2 == m2) << std::endl;
  std::cout << (m2 == n2) << std::endl;
  std::cout << vxl::trans(m2) << std::endl;
  std::cout << (m2 * n2) << std::endl;
  std::cout << (m2 * vxl::vector<float, 2>{-1.f, 1.f}) << std::endl;
  std::cout << (vxl::vector<float, 2>{-1.f, 1.f} * m2) << std::endl;

  std::cout << vxl::det(
    vxl::make_matrix<float, 2, 2>(
      1.f, 2.f, 3.f, 4.f)
    ) <<
    std::endl;

  std::cout << vxl::det(
    vxl::make_matrix<float, 3, 3>(
      6.f, 4.f, 7.f, 3.f, 8.f, 6.f, 4.f, 7.f, 3.f)
    ) <<
    std::endl;

  std::cout << vxl::det(
    vxl::make_matrix<float, 4, 4>(
      1.f, 2.f, 3.f, 4.f,
      2.f, 3.f, 4.f, 1.f,
      1.f, 4.f, 2.f, 3.f,
      3.f, 2.f, 4.f, 5.f
    )
  ) <<
  std::endl;

  vxl::zero(m2);

  return 0;
}

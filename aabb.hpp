#ifndef VXL_AABB_HPP
# define VXL_AABB_HPP
# pragma once

#include <array>

#include "vector.hpp"

namespace vxl
{

template <typename T = float, unsigned N = 3>
using aabb_t = std::array<vector<T, N>, 2>;

}

#endif // VXL_AABB_HPP

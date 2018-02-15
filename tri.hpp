#ifndef VXL_TRI_HPP
# define VXL_TRI_HPP
# pragma once

namespace vxl
{

// triangle type, 3 vertices of arbitrary dimension
template <typename T = float, unsigned N = 3>
using triangle_t = std::array<vector<T, N>, 3>;

}

#endif // VXL_TRI_HPP

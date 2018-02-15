#ifndef VXL_TRI_HPP
# define VXL_TRI_HPP
# pragma once

namespace vxl
{

template <typename T = float, unsigned N = 3>
using tri_type = ::std::array<vector<T, N>, 3>;

}

#endif // VXL_TRI_HPP

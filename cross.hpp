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

#ifndef VXL_CROSS_HPP
# define VXL_CROSS_HPP
# pragma once

#include "vector.hpp"

namespace vxl
{

// cross product
template <typename T, unsigned N>
//__attribute__ ((noinline))
constexpr inline vector<T, N> ccross(vector<T, N> const& l,
  vector<T, N> const& r) noexcept
{
/*
  return vector<T, N>{
    l.data_[1] * r.data_[2] - l.data_[2] * r.data_[1],
    l.data_[2] * r.data_[0] - l.data_[0] * r.data_[2],
    l.data_[0] * r.data_[1] - l.data_[1] * r.data_[0]
  };
*/
  return swizzled<1, 2, 0>(
    l * swizzled<1, 2, 0>(r) -
    r * swizzled<1, 2, 0>(l)
  );
}

}

#endif // VXL_CROSS_HPP

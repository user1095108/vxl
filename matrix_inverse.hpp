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

#ifndef VXL_MATRIX_INVERSE_HPP
# define VXL_MATRIX_INVERSE_HPP
# pragma once

#include "matrix_determinant.hpp"

namespace vxl
{

//////////////////////////////////////////////////////////////////////////////
template <class T>
//__attribute__ ((noinline))
inline matrix<T, 2, 2> inv(matrix<T, 2, 2> const& ma) noexcept
{
  decltype(inv(ma)) mb;

#ifndef VXL_ROW_MAJOR
  mb.template set_col<0>(vector<T, 2>{ma(1, 1), -ma(1, 0)});
  mb.template set_col<1>(vector<T, 2>{-ma(0, 1), ma(0, 0)});
#else
  mb.template set_row<0>(vector<T, 2>{ma(1, 1), -ma(0, 1)});
  mb.template set_row<1>(vector<T, 2>{-ma(1, 0), ma(0, 0)});
#endif // VXL_ROW_MAJOR

  return mb / det(ma);
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
inline matrix<T, 3, 3> inv(matrix<T, 3, 3> const& ma) noexcept
{
  decltype(inv(ma)) mb;

  mb(0, 0, ma(1, 1) * ma(2, 2) - ma(1, 2) * ma(2, 1));
  mb(0, 1, ma(0, 2) * ma(2, 1) - ma(0, 1) * ma(2, 2));
  mb(0, 2, ma(0, 1) * ma(1, 2) - ma(0, 2) * ma(1, 1));

  mb(1, 0, ma(1, 2) * ma(2, 0) - ma(1, 0) * ma(2, 2));
  mb(1, 1, ma(0, 0) * ma(2, 2) - ma(0, 2) * ma(2, 0));
  mb(1, 2, ma(0, 2) * ma(1, 0) - ma(0, 0) * ma(1, 2));

  mb(2, 0, ma(1, 0) * ma(2, 1) - ma(1, 1) * ma(2, 0));
  mb(2, 1, ma(0, 1) * ma(2, 0) - ma(0, 0) * ma(2, 1));
  mb(2, 2, ma(0, 0) * ma(1, 1) - ma(0, 1) * ma(1, 0));

  return mb / det(ma);
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
inline matrix<T, 4, 4> inv(matrix<T, 4, 4> const& ma) noexcept
{
  decltype(inv(ma)) mb;

  mb(0, 0,
    ma(1, 2) * ma(2, 3) * ma(3, 1) - ma(1, 3) * ma(2, 2) * ma(3, 1) +
    ma(1, 3) * ma(2, 1) * ma(3, 2) - ma(1, 1) * ma(2, 3) * ma(3, 2) -
    ma(1, 2) * ma(2, 1) * ma(3, 3) + ma(1, 1) * ma(2, 2) * ma(3, 3)
  );
  mb(0, 1,
    ma(0, 3) * ma(2, 2) * ma(3, 1) - ma(0, 2) * ma(2, 3) * ma(3, 1) -
    ma(0, 3) * ma(2, 1) * ma(3, 2) + ma(0, 1) * ma(2, 3) * ma(3, 2) +
    ma(0, 2) * ma(2, 1) * ma(3, 3) - ma(0, 1) * ma(2, 2) * ma(3, 3)
  );
  mb(0, 2,
    ma(0, 2) * ma(1, 3) * ma(3, 1) - ma(0, 3) * ma(1, 2) * ma(3, 1) +
    ma(0, 3) * ma(1, 1) * ma(3, 2) - ma(0, 1) * ma(1, 3) * ma(3, 2) -
    ma(0, 2) * ma(1, 1) * ma(3, 3) + ma(0, 1) * ma(1, 2) * ma(3, 3)
  );
  mb(0, 3,
    ma(0, 3) * ma(1, 2) * ma(2, 1) - ma(0, 2) * ma(1, 3) * ma(2, 1) -
    ma(0, 3) * ma(1, 1) * ma(2, 2) + ma(0, 1) * ma(1, 3) * ma(2, 2) +
    ma(0, 2) * ma(1, 1) * ma(2, 3) - ma(0, 1) * ma(1, 2) * ma(2, 3)
  );
  mb(1, 0,
    ma(1, 3) * ma(2, 2) * ma(3, 0) - ma(1, 2) * ma(2, 3) * ma(3, 0) -
    ma(1, 3) * ma(2, 0) * ma(3, 2) + ma(1, 0) * ma(2, 3) * ma(3, 2) +
    ma(1, 2) * ma(2, 0) * ma(3, 3) - ma(1, 0) * ma(2, 2) * ma(3, 3)
  );
  mb(1, 1,
    ma(0, 2) * ma(2, 3) * ma(3, 0) - ma(0, 3) * ma(2, 2) * ma(3, 0) +
    ma(0, 3) * ma(2, 0) * ma(3, 2) - ma(0, 0) * ma(2, 3) * ma(3, 2) -
    ma(0, 2) * ma(2, 0) * ma(3, 3) + ma(0, 0) * ma(2, 2) * ma(3, 3)
  );
  mb(1, 2,
    ma(0, 3) * ma(1, 2) * ma(3, 0) - ma(0, 2) * ma(1, 3) * ma(3, 0) -
    ma(0, 3) * ma(1, 0) * ma(3, 2) + ma(0, 0) * ma(1, 3) * ma(3, 2) +
    ma(0, 2) * ma(1, 0) * ma(3, 3) - ma(0, 0) * ma(1, 2) * ma(3, 3)
  );
  mb(1, 3,
    ma(0, 2) * ma(1, 3) * ma(2, 0) - ma(0, 3) * ma(1, 2) * ma(2, 0) +
    ma(0, 3) * ma(1, 0) * ma(2, 2) - ma(0, 0) * ma(1, 3) * ma(2, 2) -
    ma(0, 2) * ma(1, 0) * ma(2, 3) + ma(0, 0) * ma(1, 2) * ma(2, 3)
  );
  mb(2, 0,
    ma(1, 1) * ma(2, 3) * ma(3, 0) - ma(1, 3) * ma(2, 1) * ma(3, 0) +
    ma(1, 3) * ma(2, 0) * ma(3, 1) - ma(1, 0) * ma(2, 3) * ma(3, 1) -
    ma(1, 1) * ma(2, 0) * ma(3, 3) + ma(1, 0) * ma(2, 1) * ma(3, 3)
  );
  mb(2, 1,
    ma(0, 3) * ma(2, 1) * ma(3, 0) - ma(0, 1) * ma(2, 3) * ma(3, 0) -
    ma(0, 3) * ma(2, 0) * ma(3, 1) + ma(0, 0) * ma(2, 3) * ma(3, 1) +
    ma(0, 1) * ma(2, 0) * ma(3, 3) - ma(0, 0) * ma(2, 1) * ma(3, 3)
  );
  mb(2, 2,
    ma(0, 1) * ma(1, 3) * ma(3, 0) - ma(0, 3) * ma(1, 1) * ma(3, 0) +
    ma(0, 3) * ma(1, 0) * ma(3, 1) - ma(0, 0) * ma(1, 3) * ma(3, 1) -
    ma(0, 1) * ma(1, 0) * ma(3, 3) + ma(0, 0) * ma(1, 1) * ma(3, 3)
  );
  mb(2, 3,
    ma(0, 3) * ma(1, 1) * ma(2, 0) - ma(0, 1) * ma(1, 3) * ma(2, 0) -
    ma(0, 3) * ma(1, 0) * ma(2, 1) + ma(0, 0) * ma(1, 3) * ma(2, 1) +
    ma(0, 1) * ma(1, 0) * ma(2, 3) - ma(0, 0) * ma(1, 1) * ma(2, 3)
  );
  mb(3, 0,
    ma(1, 2) * ma(2, 1) * ma(3, 0) - ma(1, 1) * ma(2, 2) * ma(3, 0) -
    ma(1, 2) * ma(2, 0) * ma(3, 1) + ma(1, 0) * ma(2, 2) * ma(3, 1) +
    ma(1, 1) * ma(2, 0) * ma(3, 2) - ma(1, 0) * ma(2, 1) * ma(3, 2)
  );
  mb(3, 1,
    ma(0, 1) * ma(2, 2) * ma(3, 0) - ma(0, 2) * ma(2, 1) * ma(3, 0) +
    ma(0, 2) * ma(2, 0) * ma(3, 1) - ma(0, 0) * ma(2, 2) * ma(3, 1) -
    ma(0, 1) * ma(2, 0) * ma(3, 2) + ma(0, 0) * ma(2, 1) * ma(3, 2)
  );
  mb(3, 2,
    ma(0, 2) * ma(1, 1) * ma(3, 0) - ma(0, 1) * ma(1, 2) * ma(3, 0) -
    ma(0, 2) * ma(1, 0) * ma(3, 1) + ma(0, 0) * ma(1, 2) * ma(3, 1) +
    ma(0, 1) * ma(1, 0) * ma(3, 2) - ma(0, 0) * ma(1, 1) * ma(3, 2)
  );
  mb(3, 3,
    ma(0, 1) * ma(1, 2) * ma(2, 0) - ma(0, 2) * ma(1, 1) * ma(2, 0) +
    ma(0, 2) * ma(1, 0) * ma(2, 1) - ma(0, 0) * ma(1, 2) * ma(2, 1) -
    ma(0, 1) * ma(1, 0) * ma(2, 2) + ma(0, 0) * ma(1, 1) * ma(2, 2)
  );

  return mb / det(ma);
}

}

#endif // VXL_MATRIX_INVERSE_HPP

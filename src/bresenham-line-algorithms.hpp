#ifndef BRESENHAM_LINE_ALGORITHMS_HPP
#define BRESENHAM_LINE_ALGORITHMS_HPP


#include <boost/gil/gil_all.hpp>
#include "line-struct.hpp"




template <typename T> void drawLine(T &img, LineData line, int opacity);
template <typename T> unsigned long lineScore(T &img, int opacity);







#endif // BRESENHAM_LINE_ALGORITHMS_HPP

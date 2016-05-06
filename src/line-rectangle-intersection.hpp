#ifndef LINE_RECTANGE_INTERSECTION_HPP
#define LINE_RECTANGE_INTERSECTION_HPP

#include "line-struct.hpp"


// calculates the intersection with a rectangle for a line through two points, where 1 point is inside the rectangle and 1 point is at the border of the rectangle.
// the point pointToBeReplaced is the one that should be replaced (and inside), while usablePoint is the one only already on the border.
PixelCoords getLineBorderPoint(PixelCoords usablePoint, PixelCoords pointToBeReplaced, int maxX, int maxY);


#endif // LINE_RECTANGE_INTERSECTION_HPP

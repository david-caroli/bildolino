#ifndef BRESENHAM_LINE_ALGORITHMS_HPP
#define BRESENHAM_LINE_ALGORITHMS_HPP


#include <boost/gil/gil_all.hpp>
#include "line-struct.hpp"



// uses the bresenham algorithm to draw a 1 pixel grayscale line on the image view img
void drawGrayscaleLine(boost::gil::gray8_view_t &img, LineData line, int opacity, bool additive);
void drawGrayscaleLineNormalAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity);
void drawGrayscaleLineSteepAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity);
void drawGrayscaleLineNormalAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity);
void drawGrayscaleLineSteepAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity);

// uses the bresenham algorithm to calculate a score of how much a drawn 1 pixel grayscale line would change the image
unsigned long grayscaleLineScore(boost::gil::gray8_view_t &img, LineData line, int lineOpacity);
unsigned long grayscaleLineScoreNormalAngle(boost::gil::gray8_view_t &img, LineData line, int opacity);
unsigned long grayscaleLineScoreSteepAngle(boost::gil::gray8_view_t &img, LineData line, int opacity);




#endif // BRESENHAM_LINE_ALGORITHMS_HPP

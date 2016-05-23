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
unsigned long grayscaleLineScore(boost::gil::gray8_view_t &img, LineData line, int opacity, bool additive);
unsigned long grayscaleLineScoreNormalAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity);
unsigned long grayscaleLineScoreSteepAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity);
unsigned long grayscaleLineScoreNormalAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity);
unsigned long grayscaleLineScoreSteepAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity);

// uses the bresenham algorithm to draw a 1 pixel rgb line on the image view img
void drawRGBLine(boost::gil::rgb8_view_t &img, LineData line, int opacity, bool additive, int channel);
void drawRGBLineNormalAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
void drawRGBLineSteepAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
void drawRGBLineNormalAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
void drawRGBLineSteepAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);

// uses the bresenham algorithm to calculate a score of how much a drawn 1 pixel rgb line would change the image
unsigned long rgbLineScore(boost::gil::rgb8_view_t &img, LineData line, int opacity, bool additive, int channel);
unsigned long rgbLineScoreNormalAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
unsigned long rgbLineScoreSteepAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
unsigned long rgbLineScoreNormalAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);
unsigned long rgbLineScoreSteepAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c);












#endif // BRESENHAM_LINE_ALGORITHMS_HPP

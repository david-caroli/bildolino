#ifndef PIXEL_SORTING_HPP
#define PIXEL_SORTING_HPP

#include <boost/gil/gil_all.hpp>
#include "line-struct.hpp"


void sortPixels(int pixelCount, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img);
void sortPixels(int pixelCount, PixelCoords *result, bool additive, boost::gil::rgb8_view_t &img, int byChannel);
int getPixelPos(int curPixelValue, int &resultSize, PixelCoords *result, bool &additive, boost::gil::gray8_view_t &img);
int getPixelPos(int curPixelValue, int &resultSize, PixelCoords *result, bool &additive, boost::gil::rgb8_view_t &img, int &byChannel);



void sortPixelsRandomized(int pixelCount, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img);
void sortPixelsRandomized(int pixelCount, PixelCoords *result, bool additive, boost::gil::rgb8_view_t &img, int byChannel);
int getPixelPosRandomized(int curPixelValue, int &curRand, int &resultSize, PixelCoords *result, int *randoms, bool &additive, boost::gil::gray8_view_t &img);
int getPixelPosRandomized(int curPixelValue, int &curRand, int &resultSize, PixelCoords *result, int *randoms, bool &additive, boost::gil::rgb8_view_t &img, int &byChannel);


#endif // PIXEL_SORTING_HPP


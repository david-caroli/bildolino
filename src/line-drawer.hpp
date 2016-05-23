#ifndef LINE_DRAWER_HPP
#define LINE_DRAWER_HPP

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "bresenham-line-algorithms.hpp"
#include "svg-files.hpp"
#include "pixel-sorting.hpp"
#include "lock-circle.hpp"
#include "line-rectangle-intersection.hpp"


#define LOCK_COUNT 3


enum OutputFileType { OUT_F_SVG, OUT_F_PNG, OUT_F_JPEG, OUT_F_UNDEFINED };
typedef struct {
	std::string filePath;
	OutputFileType type;
} OutputFile;




void loadAndRedrawImage(int lineCount,
						int testLineCount,
						int lineOpacity,
						int lineWidth,
						bool additive, // false -> subtractive
						bool grayscale, // false -> color mode
						bool resize, // not implented
						int resizeWidth, // not implented
						int resizeHeight, // not implented
						int threadCount,
						std::string inputFile, // jpeg or png
						std::vector<OutputFile> outputFiles,
						bool showProgress,
						bool showWarnings,
						bool showErrors);

bool loadGrayImage(std::string inputFile, boost::gil::gray8_image_t &img);
bool loadRGBImage(std::string inputFile, boost::gil::rgb8_image_t &img);

void redrawImage(int lineCount,
				 int testLineCount,
				 int lineOpacity,
				 int lineWidth,
				 bool additive, // false -> subtractive
				 bool grayscale, // false -> color mode
				 int threadCount,
				 bool showProgress,
				 bool showWarnings,
				 bool showErrors,
				 boost::gil::gray8_view_t &imgGray,
				 boost::gil::rgb8_view_t &imgRGB,
				 std::vector<std::fstream*> &svgFiles);

void testLineThread(int lineCount,
					int testLineCount,
					int lineOpacity,
					bool additive, // false -> subtractive
					bool grayscale, // false -> color mode
					int threadCount,
					int threadNumber,
					LockCircle *lc,
					PixelCoords **bestPixels,
					boost::gil::gray8_view_t *imgGray,
					boost::gil::rgb8_view_t *imgRGB,
					unsigned long *bestLineScore,
					LineData *bestLine);




#endif // LINE_DRAWER_HPP


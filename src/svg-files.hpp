#ifndef SVG_FILES_HPP
#define SVG_FILES_HPP

#include <fstream>
#include "line-struct.hpp"

void initSvg(std::fstream &outFile, int height, int width, bool blackBackground, int lineOpacity, int lineThickness);
void addLine(std::fstream &outFile, LineData lineCoords);
void finishSvg(std::fstream &outFile);

#endif // SVG_FILES_HPP

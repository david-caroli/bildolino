#ifndef SVG_FILES_HPP
#define SVG_FILES_HPP

#include <fstream>
#include "line-struct.hpp"

void initSvg(std::fstream &outFile, int height, int width, bool grayscale, bool blackBackground, int lineOpacity, int lineThickness);
void addLineGrey(std::fstream &outFile, LineData lineCoords);
void addLineCol(std::fstream &outFile, LineData lineCoords, std::string colour);
void finishSvg(std::fstream &outFile);

#endif // SVG_FILES_HPP

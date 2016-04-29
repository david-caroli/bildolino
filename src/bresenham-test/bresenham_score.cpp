#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

using namespace boost::gil;

unsigned long scoreLine(gray8_view_t &img, float x1, float y1, float x2, float y2);
unsigned long bresenhamNormal(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);
unsigned long bresenhamSteep(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);


void score(gray8_view_t &img, int x1, int y1, int x2, int y2) {
	std::cout << "line from (" << x1 << "|" << y1 << ") to (" << x2 << "|" << y2 << ") score: " << scoreLine(img, x1, y1, x2, y2) << std::endl;
}

int main(int argc, char **argv) {
	std::cout << "start" << std::endl;
	gray8_image_t img_data;
	png_read_and_convert_image("bresenham_draw.png", img_data);
	gray8_view_t img = view(img_data);
	std::cout << "loaded" << std::endl;
	std::cout << "dimensons: " << img.width() << "*" << img.height() << std::endl;
	
	for(int i=0; i<=20; ++i) {
		score(img, 0, (10*i), 255, (10*i));
	}
	
	std::cout << "end" << std::endl;
	return 0;
}




unsigned long scoreLine(gray8_view_t &img, float x1, float y1, float x2, float y2) {
	if(fabs(y2 - y1) > fabs(x2 - x1)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
		return bresenhamSteep(img, x1, y1, x2, y2);
	} else {
		return bresenhamNormal(img, x1, y1, x2, y2);
	}
}

unsigned long bresenhamNormal(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
	// Bresenham's line algorithm
	unsigned long score = 0;
	if(x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	
	float dx = x2 - x1;
	float dy = fabs(y2 - y1);
	float error = dx / 2.0f;
	int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	int maxX = 1+(int)x2;
	
	for(int x=(int)x1; x<maxX; x++) {
		score += img(x, y);
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
	return score;
}

unsigned long bresenhamSteep(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
	// Bresenham's line algorithm
	unsigned long score = 0;
	if(x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	
	float dx = x2 - x1;
	float dy = fabs(y2 - y1);
	float error = dx / 2.0f;
	int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	int maxX = 1+(int)x2;
	
	for(int x=(int)x1; x<maxX; x++) {
		score += img(y, x);
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
	return score;
}


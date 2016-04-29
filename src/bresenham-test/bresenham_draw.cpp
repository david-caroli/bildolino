#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

using namespace boost::gil;

void drawLine(gray8_view_t &img, float x1, float y1, float x2, float y2);
void bresenhamNormal(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);
void bresenhamSteep(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);



int main(int argc, char **argv) {
	std::cout << "start" << std::endl;
	gray8_image_t img_data(256, 256);
	gray8_view_t img = view(img_data);
	std::cout << "initiated" << std::endl;
	std::cout << "dimensons: " << img.width() << "*" << img.height() << std::endl;
	
	for(int i=0; i<256; ++i) {
		img(i, 50) = 128;
		img(i, 100) = 128;
		img(i, 150) = 128;
		img(i, 200) = 128;
		img(200, i) = 128;
		img(250, i) = 128;
	}
	drawLine(img, 0, 100, 100, 200);
	drawLine(img, 0, 100, 100, 0);
	drawLine(img, 100, 0, 200, 100);
	drawLine(img, 100, 200, 200, 100);
	for(int i=0; i<20; ++i) {
		drawLine(img, 10+(5*i), 50, (10*i), 200);
	}
	for(int i=0; i<20; ++i) {
		drawLine(img, 200, 10+(5*i), 250, (10*i));
	}
	img(50, 228) = 128;
	img(50, 232) = 128;
	drawLine(img, 50, 230, 205,230);
	img(205, 228) = 128;
	img(205, 232) = 128;
	
	png_write_view("bresenham_draw.png", img);
	
	std::cout << "end" << std::endl;
	return 0;
}




void drawLine(gray8_view_t &img, float x1, float y1, float x2, float y2) {
	if(fabs(y2 - y1) > fabs(x2 - x1)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
		bresenhamSteep(img, x1, y1, x2, y2);
	} else {
		bresenhamNormal(img, x1, y1, x2, y2);
	}
}

void bresenhamNormal(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
	// Bresenham's line algorithm
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
		img(x, y) = 255;
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void bresenhamSteep(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
	// Bresenham's line algorithm
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
		img(y, x) = 255;
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}


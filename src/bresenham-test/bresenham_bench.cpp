#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <limits>
#include <chrono>

using namespace boost::gil;



unsigned long scoreLine(gray8_view_t &img, float x1, float y1, float x2, float y2);
unsigned long bresenhamNormalScore(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);
unsigned long bresenhamSteepScore(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);
void drawLine(gray8_view_t &img, float x1, float y1, float x2, float y2);
void bresenhamNormal(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);
void bresenhamSteep(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2);


void score(gray8_view_t &img, int x1, int y1, int x2, int y2) {
	std::cout << "line from (" << x1 << "|" << y1 << ") to (" << x2 << "|" << y2 << ") score: " << scoreLine(img, x1, y1, x2, y2) << std::endl;
}



typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
} LineCoords;


int main(int argc, char **argv) {
	srand(time(NULL));
	std::chrono::steady_clock::time_point timeA, timeB;
	std::chrono::milliseconds duration;
	std::cout << "start" << std::endl;
	timeA = std::chrono::steady_clock::now();
	gray8_image_t img_data;
	png_read_and_convert_image("bresenham_bench.png", img_data);
	gray8_view_t img = view(img_data);
	timeB = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeB - timeA);
	std::cout << "loaded in " << duration.count() << " ms" << std::endl;
	std::cout << "dimensons: " << img.width() << "*" << img.height() << std::endl;
	
	timeA = std::chrono::steady_clock::now();
	LineCoords bestLine, line;
	unsigned long bestScore = std::numeric_limits<unsigned long>::max(), currScore;
	std::vector<LineCoords> lines;
	
	for(int i=0; i<=1000; ++i) {
		for(int j=0; j<400; ++j) {
			if(rand()%2 == 0) {
				line.x1 = 0;
				line.y1 = rand() % img.height();
			} else {
				line.x1 = rand() % img.width();
				line.y1 = 0;
			}
			if(rand()%2 == 0) {
				line.x2 = img.width()-1;
				line.y2 = rand() % img.height();
			} else {
				line.x2 = rand() % img.width();
				line.y2 = img.height()-1;
			}
			//std::cout << "a" << std::endl;
			currScore = scoreLine(img, line.x1, line.y1, line.x2, line.y2);
			//std::cout << "b" << std::endl;
			if(currScore < bestScore) {
				bestScore = currScore;
				bestLine = line;
			}
		}
		//std::cout << "c" << std::endl;
		drawLine(img, bestLine.x1, bestLine.y1, bestLine.x2, bestLine.y2);
		//std::cout << "d" << std::endl;
		lines.push_back(bestLine);
	}
	timeB = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeB - timeA);
	std::cout << duration.count()/1000 << " seconds" << std::endl;
	
	std::cout << "end" << std::endl;
	return 0;
}





unsigned long scoreLine(gray8_view_t &img, float x1, float y1, float x2, float y2) {
	if(fabs(y2 - y1) > fabs(x2 - x1)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
		return bresenhamSteepScore(img, x1, y1, x2, y2);
	} else {
		return bresenhamNormalScore(img, x1, y1, x2, y2);
	}
}

unsigned long bresenhamNormalScore(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
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

unsigned long bresenhamSteepScore(gray8_view_t &img, float &x1, float &y1, float &x2, float &y2) {
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


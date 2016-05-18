#include "bresenham-line-algorithms.hpp"




void drawGrayscaleLine(boost::gil::gray8_view_t &img, LineData line, int opacity, bool additive) {
	if(abs(line.b.y - line.a.y) > abs(line.b.x - line.a.x)) {
		std::swap(line.a.x, line.a.y);
		std::swap(line.b.x, line.b.y);
		if(additive) {
			drawGrayscaleLineSteepAngleAdditive(img, line, opacity);
		} else {
			drawGrayscaleLineSteepAngleSubtractive(img, line, opacity);
		}
	} else {
		if(additive) {
			drawGrayscaleLineNormalAngleAdditive(img, line, opacity);
		} else {
			drawGrayscaleLineNormalAngleSubtractive(img, line, opacity);
		}
	}
}

void drawGrayscaleLineNormalAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y) < (255-opacity)) {
			img(x,y) = img(x,y)+opacity;
		} else {
			img(x,y) = 255;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawGrayscaleLineSteepAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x) < (255-opacity)) {
			img(y,x) = img(y,x)+opacity;
		} else {
			img(y,x) = 255;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawGrayscaleLineNormalAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y) >= opacity) {
			img(x,y) = img(x,y) - opacity;
		} else {
			img(x,y) = 0;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawGrayscaleLineSteepAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x) >= opacity) {
			img(y,x) = img(y,x) - opacity;
		} else {
			img(y,x) = 0;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}





unsigned long grayscaleLineScore(boost::gil::gray8_view_t &img, LineData line) {
	if(abs(line.b.y - line.a.y) > abs(line.b.x - line.a.x)) {
		std::swap(line.a.x, line.a.y);
		std::swap(line.b.x, line.b.y);
		return grayscaleLineScoreSteepAngle(img, line);
	} else {
		return grayscaleLineScoreNormalAngle(img, line);
	}
}

unsigned long grayscaleLineScoreNormalAngle(boost::gil::gray8_view_t &img, LineData line) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score = 0;

	for(int x=line.a.x; x<maxX; x++) {
		score += img(x,y);
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
	return score;
}

unsigned long grayscaleLineScoreSteepAngle(boost::gil::gray8_view_t &img, LineData line) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score = 0;

	for(int x=line.a.x; x<maxX; x++) {
		score += img(y,x);
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
	return score;
}


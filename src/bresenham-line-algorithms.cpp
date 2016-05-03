#include "bresenham-line-algorithms.hpp"




void drawGrayscaleLine(boost::gil::gray8_view_t &img, LineData line, int opacity, bool additive) {
	if(abs(line.y1 - line.y0) > abs(line.x1 - line.x0)) {
		std::swap(line.x0, line.y0);
		std::swap(line.x1, line.y1);
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
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;

	for(int x=line.x0; x<maxX; x++) {
		img(x,y) = (img(x,y)+opacity) % 256;
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawGrayscaleLineSteepAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;

	for(int x=line.x0; x<maxX; x++) {
		img(y,x) = (img(y,x)+opacity) % 256;
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawGrayscaleLineNormalAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;

	for(int x=line.x0; x<maxX; x++) {
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
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;

	for(int x=line.x0; x<maxX; x++) {
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
	if(abs(line.y1 - line.y0) > abs(line.x1 - line.x0)) {
		std::swap(line.x0, line.y0);
		std::swap(line.x1, line.y1);
		return grayscaleLineScoreSteepAngle(img, line);
	} else {
		return grayscaleLineScoreNormalAngle(img, line);
	}
}

unsigned long grayscaleLineScoreNormalAngle(boost::gil::gray8_view_t &img, LineData line) {
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;
	unsigned long score = 0;

	for(int x=line.x0; x<maxX; x++) {
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
	if(line.x0 > line.x1) {
		std::swap(line.x0, line.x1);
		std::swap(line.y0, line.y1);
	}

	float dx = line.x1 - line.x0;
	float dy = abs(line.y1 - line.y0);
	float error = dx / 2.0f;
	int ystep = (line.y0 < line.y1) ? 1 : -1;
	int y = line.y0;
	const int maxX = 1+line.x1;
	unsigned long score = 0;

	for(int x=line.x0; x<maxX; x++) {
		score += img(y,x);
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
	return score;
}


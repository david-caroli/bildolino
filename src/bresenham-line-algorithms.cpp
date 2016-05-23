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






unsigned long grayscaleLineScore(boost::gil::gray8_view_t &img, LineData line, int opacity, bool additive) {
	if(abs(line.b.y - line.a.y) > abs(line.b.x - line.a.x)) {
		std::swap(line.a.x, line.a.y);
		std::swap(line.b.x, line.b.y);
		if(additive) {
			return grayscaleLineScoreSteepAngleAdditive(img, line, opacity);
		} else {
			return grayscaleLineScoreSteepAngleSubtractive(img, line, opacity);
		}
	} else {
		if(additive) {
			return grayscaleLineScoreNormalAngleAdditive(img, line, opacity);
		} else {
			return grayscaleLineScoreNormalAngleSubtractive(img, line, opacity);
		}
	}
}
unsigned long grayscaleLineScoreNormalAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y) < opacity) {
			score += img(x,y);
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long grayscaleLineScoreNormalAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if((255-img(x,y)) < opacity) { // potentially optimize to if(img(x,y) > invertedOpacity)   where invertedOpacity=255-opacity
			score += (255-img(x,y));
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long grayscaleLineScoreSteepAngleAdditive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x) < opacity) {
			score += img(y,x);
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long grayscaleLineScoreSteepAngleSubtractive(boost::gil::gray8_view_t &img, LineData line, int opacity) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if((255-img(y,x)) < opacity) { // potentially optimize to if(img(y,x) > invertedOpacity)   where invertedOpacity=255-opacity
			score += (255-img(y,x));
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}










void drawRGBLine(boost::gil::rgb8_view_t &img, LineData line, int opacity, bool additive, int channel) {
	if(abs(line.b.y - line.a.y) > abs(line.b.x - line.a.x)) {
		std::swap(line.a.x, line.a.y);
		std::swap(line.b.x, line.b.y);
		if(additive) {
			drawRGBLineSteepAngleAdditive(img, line, opacity, channel);
		} else {
			drawRGBLineSteepAngleSubtractive(img, line, opacity, channel);
		}
	} else {
		if(additive) {
			drawRGBLineNormalAngleAdditive(img, line, opacity, channel);
		} else {
			drawRGBLineNormalAngleSubtractive(img, line, opacity, channel);
		}
	}
}
void drawRGBLineNormalAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y)[c] < (255-opacity)) {
			img(x,y)[c] = img(x,y)[c]+opacity;
		} else {
			img(x,y)[c] = 255;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawRGBLineSteepAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x)[c] < (255-opacity)) {
			img(y,x)[c] = img(y,x)[c]+opacity;
		} else {
			img(y,x)[c] = 255;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawRGBLineNormalAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y)[c] >= opacity) {
			img(x,y)[c] = img(x,y)[c] - opacity;
		} else {
			img(x,y)[c] = 0;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void drawRGBLineSteepAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x)[c] >= opacity) {
			img(y,x)[c] = img(y,x)[c] - opacity;
		} else {
			img(y,x)[c] = 0;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}





unsigned long rgbLineScore(boost::gil::rgb8_view_t &img, LineData line, int opacity, bool additive, int channel) {
	if(abs(line.b.y - line.a.y) > abs(line.b.x - line.a.x)) {
		std::swap(line.a.x, line.a.y);
		std::swap(line.b.x, line.b.y);
		if(additive) {
			return rgbLineScoreSteepAngleAdditive(img, line, opacity, channel);
		} else {
			return rgbLineScoreSteepAngleSubtractive(img, line, opacity, channel);
		}
	} else {
		if(additive) {
			return rgbLineScoreNormalAngleAdditive(img, line, opacity, channel);
		} else {
			return rgbLineScoreNormalAngleSubtractive(img, line, opacity, channel);
		}
	}
}
unsigned long rgbLineScoreNormalAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(x,y)[c] < opacity) {
			score += img(x,y)[c];
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long rgbLineScoreNormalAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if((255-img(x,y)[c]) < opacity) { // potentially optimize to if(img(x,y) > invertedOpacity)   where invertedOpacity=255-opacity
			score += (255-img(x,y)[c]);
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long rgbLineScoreSteepAngleAdditive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if(img(y,x)[c] < opacity) {
			score += img(y,x)[c];
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}

unsigned long rgbLineScoreSteepAngleSubtractive(boost::gil::rgb8_view_t &img, LineData line, int opacity, int c) {
	if(line.a.x > line.b.x)
		std::swap(line.a, line.b);

	float dx = line.b.x - line.a.x;
	float dy = abs(line.b.y - line.a.y);
	float error = dx / 2.0f;
	int ystep = (line.a.y < line.b.y) ? 1 : -1;
	int y = line.a.y;
	const int maxX = 1+line.b.x;
	unsigned long score=0, pixelCount=0;

	for(int x=line.a.x; x<maxX; x++) {
		if((255-img(y,x)[c]) < opacity) { // potentially optimize to if(img(y,x) > invertedOpacity)   where invertedOpacity=255-opacity
			score += (255-img(y,x)[c]);
		} else {
			score += opacity;
		}
		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
		++pixelCount;
	}
	return score/pixelCount;
}


#include "pixel-sorting.hpp"



void sortPixels(int pixelCount, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img) {
	// does not use any border pixels as they lead to annoying edge cases, and will very likely get lines through them as each line will pass through two borders
	// side effect: this will make lines along an edge impossible
	// selection sort (good when number of pixels in the image is much bigger than the number of lines drawns/the numbers of pixels selected)
	int sorted = 0;
	int pos;
	for(int y=1; y<img.height()-1; ++y) {
		for(int x=1; x<img.width()-1; ++x) {
			pos = getPixelPos(img(x,y), sorted, result, additive, img);
			if(pos < pixelCount) {
				if(sorted < pixelCount) // grow until array is full
					++sorted;
				for(int i=sorted-1; i>pos; --i)
					result[i] = result[i-1];
				result[pos].x = x;
				result[pos].y = y;
			}
		}
	}
}

void sortPixels(int pixelCount, PixelCoords *result, bool additive, boost::gil::rgb8_view_t &img, int byChannel) {
	// does not use any border pixels as they lead to annoying edge cases, and will very likely get lines through them as each line will pass through two borders
	// side effect: this will make lines along an edge impossible
	// selection sort (good when number of pixels in the image is much bigger than the number of lines drawns/the numbers of pixels selected)
	int sorted = 0;
	int pos;
	for(int y=1; y<img.height()-1; ++y) {
		for(int x=1; x<img.width()-1; ++x) {
			pos = getPixelPos(img(x,y)[byChannel], sorted, result, additive, img, byChannel);
			if(pos < pixelCount) {
				if(sorted < pixelCount) // grow until array is full
					++sorted;
				for(int i=sorted-1; i>pos; --i)
					result[i] = result[i-1];
				result[pos].x = x;
				result[pos].y = y;
			}
		}
	}
}

int getPixelPos(int curPixelValue, int &resultSize, PixelCoords *result, bool &additive, boost::gil::gray8_view_t &img) {
	// binary search
	int left = 0;
	int right = resultSize;
	int center;
	while(left != right) {
		center = (left+right)/2;
		if(additive) {
			if(img(result[center].x, result[center].y) < curPixelValue) {
				right = center;
			} else {
				left = center+1;
			}
		} else {
			if(img(result[center].x, result[center].y) > curPixelValue) {
				right = center;
			} else {
				left = center+1;
			}
		}
	}
	return left;
}

int getPixelPos(int curPixelValue, int &resultSize, PixelCoords *result, bool &additive, boost::gil::rgb8_view_t &img, int &byChannel) {
	// binary search
	int left = 0;
	int right = resultSize;
	int center;
	while(left != right) {
		center = (left+right)/2;
		if(additive) {
			if(img(result[center].x, result[center].y)[byChannel] < curPixelValue) {
				right = center;
			} else {
				left = center+1;
			}
		} else {
			if(img(result[center].x, result[center].y)[byChannel] > curPixelValue) {
				right = center;
			} else {
				left = center+1;
			}
		}
	}
	return left;
}









void sortPixelsRandomized(int pixelCount, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img) {
	// add random values to make sure that for many pixel of exact the same color not only the topleftmost pixels are taken, but rather a random roughly equal distribution.
	std::random_device rd;
	std::minstd_rand randGen(rd());
	std::uniform_int_distribution<int> distr(0, 65535);
	int randoms[pixelCount];
	int newRand;
	// does not use any border pixels as they lead to annoying edge cases, and will very likely get lines through them as each line will pass through two borders
	// side effect: this will make lines along an edge impossible
	// selection sort (good when number of pixels in the image is much bigger than the number of lines drawns/the numbers of pixels selected)
	int sorted = 0;
	int pos;
	for(int y=1; y<img.height()-1; ++y) {
		for(int x=1; x<img.width()-1; ++x) {
			newRand = distr(randGen);
			pos = getPixelPosRandomized(img(x,y), newRand, sorted, result, randoms, additive, img);
			if(pos < pixelCount) {
				if(sorted < pixelCount) // grow until array is full
					++sorted;
				for(int i=sorted-1; i>pos; --i) {
					result[i] = result[i-1];
					randoms[i] = randoms[i-1];
				}
				result[pos].x = x;
				result[pos].y = y;
				randoms[pos] = newRand;
			}
		}
	}
}

void sortPixelsRandomized(int pixelCount, PixelCoords *result, bool additive, boost::gil::rgb8_view_t &img, int byChannel) {
	// add random values to make sure that for many pixel of exact the same color not only the topleftmost pixels are taken, but rather a random roughly equal distribution.
	std::random_device rd;
	std::minstd_rand randGen(rd());
	std::uniform_int_distribution<int> distr(0, 65535);
	int randoms[pixelCount];
	int newRand;
	// does not use any border pixels as they lead to annoying edge cases, and will very likely get lines through them as each line will pass through two borders
	// side effect: this will make lines along an edge impossible
	// selection sort (good when number of pixels in the image is much bigger than the number of lines drawns/the numbers of pixels selected)
	int sorted = 0;
	int pos;
	for(int y=1; y<img.height()-1; ++y) {
		for(int x=1; x<img.width()-1; ++x) {
			newRand = distr(randGen);
			pos = getPixelPosRandomized(img(x,y)[byChannel], newRand, sorted, result, randoms, additive, img, byChannel);
			if(pos < pixelCount) {
				if(sorted < pixelCount) // grow until array is full
					++sorted;
				for(int i=sorted-1; i>pos; --i) {
					result[i] = result[i-1];
					randoms[i] = randoms[i-1];
				}
				result[pos].x = x;
				result[pos].y = y;
				randoms[pos] = newRand;
			}
		}
	}
}

int getPixelPosRandomized(int curPixelValue, int &curRand, int &resultSize, PixelCoords *result, int *randoms, bool &additive, boost::gil::gray8_view_t &img) {
	// binary search
	int left = 0;
	int right = resultSize;
	int center;
	while(left != right) {
		center = (left+right)/2;
		if(additive) {
			if(img(result[center].x, result[center].y) < curPixelValue) {
				right = center;
			} else if(img(result[center].x, result[center].y) > curPixelValue) {
				left = center+1;
			} else if(randoms[center] < curRand) {
				right = center;
			} else {
				left = center+1;
			}
		} else {
			if(img(result[center].x, result[center].y) > curPixelValue) {
				right = center;
			} else if(img(result[center].x, result[center].y) < curPixelValue) {
				left = center+1;
			} else if(randoms[center] > curRand) {
				right = center;
			} else {
				left = center+1;
			}
		}
	}
	return left;
}

int getPixelPosRandomized(int curPixelValue, int &curRand, int &resultSize, PixelCoords *result, int *randoms, bool &additive, boost::gil::rgb8_view_t &img, int &byChannel) {
	// binary search
	int left = 0;
	int right = resultSize;
	int center;
	while(left != right) {
		center = (left+right)/2;
		if(additive) {
			if(img(result[center].x, result[center].y)[byChannel] < curPixelValue) {
				right = center;
			} else if(img(result[center].x, result[center].y)[byChannel] > curPixelValue) {
				left = center+1;
			} else if(randoms[center] < curRand) {
				right = center;
			} else {
				left = center+1;
			}
		} else {
			if(img(result[center].x, result[center].y)[byChannel] > curPixelValue) {
				right = center;
			} else if(img(result[center].x, result[center].y)[byChannel] < curPixelValue) {
				left = center+1;
			} else if(randoms[center] > curRand) {
				right = center;
			} else {
				left = center+1;
			}
		}
	}
	return left;
}


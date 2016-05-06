#include "line-drawer.hpp"

using namespace boost::gil;



void loadAndRedrawImage(int lineCount,
						int testLineCount,
						int lineOpacity,
						int lineWidth,
						bool additive, // false -> subtractive
						bool grayscale, // false -> color mode (not implemented)
						bool resize, // not implented
						int resizeWidth, // not implented
						int resizeHeight, // not implented
						int threadCount,
						std::string inputFile, // jpeg or png
						std::vector<OutputFile> outputFiles,
						bool showProgress,
						bool showWarnings,
						bool showErrors) {
	if(grayscale) {
		gray8_image_t img;
		if(!loadGrayImage(inputFile, img)) {
			if(showErrors)
				std::cout << "Error: unable to open file" << std::endl;
			return;
		}
		if(showProgress)
			std::cout << "image loaded" << std::endl;
		// TODO implement resizing
		// TODO open line svg files and pass them
		gray8_view_t imgView = view(img);
		redrawInGrayscale(lineCount, testLineCount, lineOpacity, lineWidth, additive, threadCount, showProgress, showWarnings, showErrors, imgView);
		// TODO save result and close svg file
	} else {
		// TODO implement
		if(showErrors)
			std::cout << "Error: color mode is not yet implemented" << std::endl;
	}
}

bool loadRGBImage(std::string inputFile, rgb8_image_t &img) {
	return false; // TODO implement loading images
}
bool loadGrayImage(std::string inputFile, gray8_image_t &img) {
	return false; // TODO implement loading images
}


void redrawInGrayscale(int lineCount,
					   int testLineCount,
					   int lineOpacity,
					   int lineWidth,
					   bool additive, // false -> subtractive
					   int threadCount,
					   bool showProgress,
					   bool showWarnings,
					   bool showErrors,
					   gray8_view_t &img) {
	// sort pixels
	if(showProgress)
		std::cout << "sorting pixels" << std::endl;
	PixelCoords bestPixels[lineCount];
	sortPixels(lineCount, bestPixels, additive, img);
	if(showProgress)
		std::cout << "pixels sorted" << std::endl;
	// initiate lock circles for each thread
	LockCircle lc[threadCount];
	unsigned int currLock[threadCount];
	for(int t=0; t<threadCount; ++t) {
		lcInitLockCircle(lc[t], LOCK_COUNT);
		lcThreadInit(lc[t], true, 0);
		currLock[t] = 0;
	}
	// start threads
	std::thread *tlThread[threadCount];
	LineData bestLine[threadCount];
	unsigned long bestLineScore[threadCount];
	//TestViewWrapper testWrapper[threadCount];
	for(int t=0; t<threadCount; ++t) {
		tlThread[t] = new std::thread(grayscaleTestLineThread, lineCount, (testLineCount/threadCount) + (t<(testLineCount%threadCount) ? 1:0), additive, threadCount, t, &lc[t], &bestPixels[0], &img, &bestLineScore[t], &bestLine[t]);
	}
	// wait for threads to initiate lock circles
	for(int t=0; t<threadCount; ++t) {
		lcWaitForInit(lc[t]);
	}
	// draw lines
	if(showProgress)
		std::cout << "starting to draw" << std::endl;
	LineData selectedLines[lineCount];
	unsigned long currentScore;
	for(int l=0; l<lineCount; ++l) {
		// got through all locks until threads done, then go active again
		for(int i=0; i<LOCK_COUNT; ++i)
			for(int t=0; t<threadCount; ++t)
				lcGetNextLock(lc[t], currLock[t]);
		// find best line
		currentScore = bestLineScore[0];
		selectedLines[l] = bestLine[0];
		for(int t=1; t<threadCount; ++t) {
			if((currentScore < bestLineScore[t]) == additive) {
				currentScore = bestLineScore[t];
				selectedLines[l] = bestLine[t];
			}
		}
		// substract best line from image (negate additive)
		drawGrayscaleLine(img, selectedLines[l], lineOpacity, !additive);
		// TODO safe line coords to buff (and optionally push buffer to svg file)
	}
	for(int t=0; t<threadCount; ++t) {
		lcUnlock(lc[t], currLock[t]);
		tlThread[t]->join();
		delete tlThread[t];
		lcCleanup(lc[t]);
	}
}


void sortPixels(int pixelCount, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img) {
	// does not use any border pixels as they lead to annoying edge cases, and will very likely get lines through them as each line will pass through two borders
	// side effect: this will make lines allong an edge impossible
	int sorted = 0;
	int pos;
	for(int y=1; y<img.height()-1; ++y) {
		for(int x=1; x<img.width()-1; ++x) {
			pos = getPixelPos(img(x,y), sorted, result, additive, img);
			if(pos < pixelCount) {
				if(sorted < pixelCount) // grow until array is full
					++sorted;
				for(int i=sorted; i>pos; --i)
					result[i] = result[i-1];
				result[pos].x = x;
				result[pos].y = y;
			}
		}
	}
}

int getPixelPos(int curPixelValue, int resultSize, PixelCoords *result, bool additive, boost::gil::gray8_view_t &img) {
	// binary search
	int left = 0;
	int right = resultSize;
	int center;
	while(left != right) {
		center = (left+right)/2;
		if(additive) {
			if(img(result[center].x, result[center].y) <= curPixelValue) {
				left = center+1;
			} else {
				right = center;
			}
		} else {
			if(img(result[center].x, result[center].y) >= curPixelValue) {
				left = center+1;
			} else {
				right = center;
			}
		}
	}
	return left;
}



void grayscaleTestLineThread(int lineCount,
							 int testLineCount,
							 bool additive, // false -> subtractive
							 int threadCount,
							 int threadNumber,
							 LockCircle *lc,
							 PixelCoords *bestPixels,
							 boost::gil::gray8_view_t *img,
							 unsigned long *bestLineScore,
							 LineData *bestLine) {
	// lock circle init
	unsigned int currLock = lc->n-1;
	lcThreadInit(*lc, false, currLock);
	lcWaitForInit(*lc);
	
	// random generator
	int xRangeStart = 0;
	for(int t=0; t<threadNumber; ++t)
		xRangeStart += (img->width()/threadCount) + (t<(img->width()%threadCount) ? 1:0);
	int xRange = (img->width()/threadCount) + (threadNumber<(img->width()%threadCount) ? 1:0);
	int yRangeStart = 0;
	for(int t=0; t<threadNumber; ++t)
		yRangeStart += (img->height()/threadCount) + (t<(img->height()%threadCount) ? 1:0);
	int yRange = (img->height()/threadCount) + (threadNumber<(img->height()%threadCount) ? 1:0);
	std::minstd_rand randGen;
	std::uniform_int_distribution<int> xDistribution(0, xRange-1);
	std::uniform_int_distribution<int> yDistribution(0, yRange-1);
	
	// vars
	int xLeft=0, xRight=img->width()-1;
	int yTop=0, yBot=img->height()-1;
	int testL=0, side, rand;
	unsigned long currScore;
	LineData currLine;

	// iterate selected best pixels to draw test lines
	for(int l=0; l<lineCount; ++l) {
		// get active lock
		lcGetNextLock(*lc, currLock);
		// score init test line starting from the bottom
		bestLine->a.x = xRangeStart + xDistribution(randGen);
		bestLine->a.y = yBot;
		bestLine->b = getLineBorderPoint(currLine.a, bestPixels[l], xRight, yBot);
		*bestLineScore = grayscaleLineScore(*img, *bestLine);
		// score test lines
		for(testL=1; testL<testLineCount;) {
			// iterate the for rectangle sides
			for(side=0; side<4 && testL<testLineCount; ++side) {
				switch(side) {
					case 0: // left
						currLine.a.x = xLeft;
						currLine.a.y = yRangeStart + yDistribution(randGen);
						break;
					case 1: // top
						currLine.a.x = xRangeStart + xDistribution(randGen);
						currLine.a.y = yTop;
						break;
					case 2: // right
						currLine.a.x = xRight;
						currLine.a.y = yRangeStart + yDistribution(randGen);
						break;
					case 3: // bottom
						currLine.a.x = xRangeStart + xDistribution(randGen);
						currLine.a.y = yBot;
						break;
				}
				currLine.b = getLineBorderPoint(currLine.a, bestPixels[l], xRight, yBot);
				// score test line and compare to best test line
				currScore = grayscaleLineScore(*img, currLine);
				if((currScore > *bestLineScore) == additive) {
					*bestLine = currLine;
					*bestLineScore = currScore;
				}
				++testL;
			}
		}
		// get inactive lock and pass active lock on
		lcGetNextLock(*lc, currLock);
		lcGetNextLock(*lc, currLock);
	}
	// lock circle cleanup
	lcUnlock(*lc, currLock);
}

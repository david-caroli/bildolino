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
	gray8_image_t imgGray;
	rgb8_image_t imgRGB;
	if(grayscale) {
		if(!loadGrayImage(inputFile, imgGray)) {
			if(showErrors)
				std::cout << "Error: unable to open file" << std::endl;
			return;
		}
	} else {
		if(!loadRGBImage(inputFile, imgRGB)) {
			if(showErrors)
				std::cout << "Error: unable to open file" << std::endl;
			return;
		}
	}
	gray8_view_t imgGrayView = view(imgGray);
	rgb8_view_t imgRGBView = view(imgRGB);
	if(showProgress)
		std::cout << "image loaded" << std::endl;
	// TODO implement resizing
	// open line svg files and pass them to drawing function
	std::vector<std::fstream*> svgFiles;
	std::fstream *svgF;
	for(unsigned int i=0; i<outputFiles.size(); ++i) {
		if(outputFiles[i].type == OUT_F_SVG) {
			// here could be an option to only append to an allready started image, in order to prevent losing all progress on crashing
			svgF = new std::fstream(outputFiles[i].filePath.c_str(), std::ios::out | std::ios::trunc);
			if(!svgF->is_open()) {
				delete svgF;
				for(unsigned int j=0; j<svgFiles.size(); ++j) {
					svgFiles[j]->close();
					delete svgFiles[j];
				}
				svgFiles.clear();
				if(showErrors)
					std::cout << "Error: unable to open file: \"" << outputFiles[i].filePath << "\"" << std::endl;
				return;
			}
			if(grayscale) {
				initSvg(*svgF, imgGrayView.height(), imgGrayView.width(), grayscale, additive, lineOpacity, lineWidth);
			} else {
				initSvg(*svgF, imgRGBView.height(), imgRGBView.width(), grayscale, additive, lineOpacity, lineWidth);
			}
			svgFiles.push_back(svgF);
		}
	}
	// draw
	redrawImage(lineCount, testLineCount, lineOpacity, lineWidth, additive, grayscale, threadCount, showProgress, showWarnings, showErrors, imgGrayView, imgRGBView, svgFiles);
	// close svg files
	for(unsigned int i=0; i<svgFiles.size(); ++i) {
		finishSvg(*svgFiles[i]);
		svgFiles[i]->close();
		delete svgFiles[i];
	}
	if(showProgress)
		std::cout << "svg file(s) saved" << std::endl;
	// TODO draw and save result to png and jpeg files
}




bool loadGrayImage(std::string inputFile, gray8_image_t &img) {
	// try jpeg format
	try {
		jpeg_read_and_convert_image(inputFile.c_str(), img);
		return true;
	} catch(std::ios_base::failure e) {}
	// try png format
	try {
		png_read_and_convert_image(inputFile.c_str(), img);
		return true;
	} catch(std::ios_base::failure e) {}
	// unable to read file or neither jpeg nor png
	return false;
}
bool loadRGBImage(std::string inputFile, rgb8_image_t &img) {
	// try jpeg format
	try {
		jpeg_read_and_convert_image(inputFile.c_str(), img);
		return true;
	} catch(std::ios_base::failure e) {}
	// try png format
	try {
		png_read_and_convert_image(inputFile.c_str(), img);
		return true;
	} catch(std::ios_base::failure e) {}
	// unable to read file or neither jpeg nor png
	return false;
}


void redrawImage(int lineCount,
				 int testLineCount,
				 int lineOpacity,
				 int lineWidth,
				 bool additive, // false -> subtractive
				 bool grayscale, // false -> color mode
				 int threadCount,
				 bool showProgress,
				 bool showWarnings,
				 bool showErrors,
				 gray8_view_t &imgGray,
				 rgb8_view_t &imgRGB,
				 std::vector<std::fstream*> &svgFiles) {
	int channelCount = (grayscale ? 1:3);
	// sort pixels
	if(showProgress)
		std::cout << "sorting pixels" << std::endl;
	PixelCoords **bestPixels = new PixelCoords*[channelCount];
	for(int channel=0; channel<channelCount; ++channel) {
		bestPixels[channel] = new PixelCoords[lineCount];
		if(grayscale) {
			sortPixels(lineCount, bestPixels[channel], additive, imgGray);
		} else {
			sortPixels(lineCount, bestPixels[channel], additive, imgRGB, channel);
		}
	}
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
	LineData bestThreadLine[threadCount];
	unsigned long bestThreadLineScore[threadCount];
	//TestViewWrapper testWrapper[threadCount];
	for(int t=0; t<threadCount; ++t) {
		tlThread[t] = new std::thread(testLineThread, lineCount, (testLineCount/threadCount) + (t<(testLineCount%threadCount) ? 1:0), lineOpacity, additive, grayscale, threadCount, t, &lc[t], bestPixels, &imgGray, &imgRGB, &bestThreadLineScore[t], &bestThreadLine[t]);
	}
	// wait for threads to initiate lock circles
	for(int t=0; t<threadCount; ++t) {
		lcWaitForInit(lc[t]);
	}
	// draw lines
	if(showProgress)
		std::cout << "starting to calculate lines" << std::endl;
	LineData selectedLines[channelCount][lineCount];
	unsigned long currentScore;
	int showProgressInterval = (channelCount*lineCount)/10; // 10%
	for(int l=0; l<lineCount; ++l) {
		for(int channel=0; channel<channelCount; ++channel) {
			// got through all locks for inactivity, let threads work
			for(int t=0; t<threadCount; ++t) {
				lcGetNextLock(lc[t], currLock[t]);
				lcGetNextLock(lc[t], currLock[t]);
			}
			// push last finished line to svg files while threads work on next line
			if(l > 0) {
				for(unsigned int i=0; i<svgFiles.size(); ++i) {
					if(grayscale) {
						addLineGrey(*svgFiles[i], selectedLines[channel][l-1]);
					} else {
						if(additive) {
							addLineCol(*svgFiles[i], selectedLines[channel][l-1], (channel==0 ? "red":(channel==1 ? "green":"blue")));
						} else {
							addLineCol(*svgFiles[i], selectedLines[channel][l-1], (channel==0 ? "cyan":(channel==1 ? "magenta":"yellow")));
						}
					}
				}
			}
			if(showProgress && ((channelCount*l)+channel)%showProgressInterval==0)
				std::cout << "lines: " << ((channelCount*l)+channel) << "/" << (channelCount*lineCount) << "   progress: ~" << ((((channelCount*l)+channel)*100)/(channelCount*lineCount)) << "%" << std::endl;
			// go active again after threads finish
			for(int t=0; t<threadCount; ++t)
				lcGetNextLock(lc[t], currLock[t]);
			// find best line
			currentScore = bestThreadLineScore[0];
			selectedLines[channel][l] = bestThreadLine[0];
			for(int t=1; t<threadCount; ++t) {
				if(currentScore < bestThreadLineScore[t]) {
					currentScore = bestThreadLineScore[t];
					selectedLines[channel][l] = bestThreadLine[t];
				}
			}
			// substract best line from image (negate additive)
			if(grayscale) {
				drawGrayscaleLine(imgGray, selectedLines[channel][l], lineOpacity, !additive);
			} else {
				drawRGBLine(imgRGB, selectedLines[channel][l], lineOpacity, !additive, channel);
			}
		}
	}
	// push remaining line to svg files
	if(lineCount > 0) {
		for(unsigned int i=0; i<svgFiles.size(); ++i) {
			if(grayscale) {
				addLineGrey(*svgFiles[i], selectedLines[channelCount-1][lineCount-1]);
			} else {
				addLineCol(*svgFiles[i], selectedLines[channelCount-1][lineCount-1], (channelCount-1==0 ? "red":(channelCount-1==1 ? "green":"blue")));
			}
		}
	}
	if(showProgress)
		std::cout << "lines: " << (channelCount*lineCount) << "/" << (channelCount*lineCount) << "   progress: ~100%" << std::endl;
	// cleanup and waiting for threads to finish
	for(int t=0; t<threadCount; ++t) {
		lcUnlock(lc[t], currLock[t]);
		tlThread[t]->join();
		delete tlThread[t];
		lcCleanup(lc[t]);
	}
	for(int channel=0; channel<channelCount; ++channel)
		delete[] bestPixels[channel];
	delete[] bestPixels;
}





void testLineThread(int lineCount,
					int testLineCount,
					int lineOpacity,
					bool additive, // false -> subtractive
					bool grayscale, // false -> color mode
					int threadCount,
					int threadNumber,
					LockCircle *lc,
					PixelCoords **bestPixels,
					gray8_view_t *imgGray,
					rgb8_view_t *imgRGB,
					unsigned long *bestLineScore,
					LineData *bestLine) {
	int channelCount = (grayscale ? 1:3);
	// lock circle init
	unsigned int currLock = lc->n-1;
	lcThreadInit(*lc, false, currLock);
	lcWaitForInit(*lc);

	// random generator
	int width, height;
	if(grayscale) {
		width = imgGray->width();
		height = imgGray->height();
	} else {
		width = imgRGB->width();
		height = imgRGB->height();
	}
	int xRangeStart = 0;
	for(int t=0; t<threadNumber; ++t)
		xRangeStart += (width/threadCount) + (t<(width%threadCount) ? 1:0);
	int xRange = (width/threadCount) + (threadNumber<(width%threadCount) ? 1:0);
	int yRangeStart = 0;
	for(int t=0; t<threadNumber; ++t)
		yRangeStart += (height/threadCount) + (t<(height%threadCount) ? 1:0);
	int yRange = (height/threadCount) + (threadNumber<(height%threadCount) ? 1:0);
	std::random_device rd;
	std::minstd_rand randGen(rd());
	std::uniform_int_distribution<int> xDistribution(0, xRange-1);
	std::uniform_int_distribution<int> yDistribution(0, yRange-1);

	// vars
	int xLeft=0, xRight=width-1;
	int yTop=0, yBot=height-1;
	int testL=0, side;
	unsigned long currScore;
	LineData currLine;

	// iterate selected best pixels to draw test lines
	for(int l=0; l<lineCount; ++l) {
		for(int channel=0; channel<channelCount; ++channel) {
			// get active lock
			lcGetNextLock(*lc, currLock);
			// score init test line starting from the bottom
			bestLine->a.x = xRangeStart + xDistribution(randGen);
			bestLine->a.y = yBot;
			bestLine->b = getLineBorderPoint(bestLine->a, bestPixels[channel][l], xRight, yBot);
			if(grayscale) {
				*bestLineScore = grayscaleLineScore(*imgGray, *bestLine, lineOpacity, additive);
			} else {
				*bestLineScore = rgbLineScore(*imgRGB, *bestLine, lineOpacity, additive, channel);
			}
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
					currLine.b = getLineBorderPoint(currLine.a, bestPixels[channel][l], xRight, yBot);
					// score test line and compare to best test line
					if(grayscale) {
						currScore = grayscaleLineScore(*imgGray, currLine, lineOpacity, additive);
					} else {
						currScore = rgbLineScore(*imgRGB, currLine, lineOpacity, additive, channel);
					}
					if(currScore > *bestLineScore) {
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
	}
	// lock circle cleanup
	lcUnlock(*lc, currLock);
}


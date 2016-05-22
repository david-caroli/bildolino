#include <iostream>
#include <chrono>
#include "line-drawer.hpp"
#include <string.h>

//helper function to determine whether a parameter is a number
bool isNumber(const std::string& s);
//helper function to determine whether a file already exists
inline bool fileExists(const std::string& name);
//helper function to determine whether a given argv[i] is a valid parameter
bool isPrm(char *arg, const char *variant0, const char *variant1);

// handle program parameters and call matching functions
int main(int argc, char **argv) {
	//convert gets set true if a convert parameter is given
	//render gets set true if a render parameter is given
	//if both are set the program terminates with return code -1 (it can't perform a conversion and a rendering at once)
	bool convert = false;
	bool render = false;
	//setting default values
	int lineCount = 10000;
	int testLineCount = 200;
	int lineOpacity = 128;
	int lineWidth = 1;
	bool additive = false;
	bool grayscale = true;
	bool resize = false;
	int resizeWidth = 0;
	int resizeHeight = 0;
	int threadCount = 1;
	std::vector<OutputFile> outFiles;
	std::string inputFile;
	bool showProgress = true;
	bool showWarnings = true;
	bool showErrors = true;

	if(argc == 2 && (isPrm(argv[1], "-v", "--version"))){
		std::cout << "Current Version: 1.0.0";
		return 0;
	} else if(argc == 2 && (isPrm(argv[1], "-i", "--interactive"))){
		std::cout << "Interactive mode is not implemented yet." << std::endl;
		return 0;
	} else {
		for (int i = 1; i < argc; ++i) {
			if(isPrm(argv[i], "-l", "--lines")) {																		// -l, --lines
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The given argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					lineCount = std::stoi(argv[i + 1]);
					render = true;
					++i;
				}
			} else if(isPrm(argv[i], "-tl", "--test-lines")) {															// -tl, --test-lines
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The given argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					testLineCount = std::stoi(argv[i + 1]);
					render = true;
					++i;
				}
			} else if(isPrm(argv[i], "-lt", "--line-transparency")) {													// -lt, --line-transparency
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The given argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					lineOpacity = std::stoi(argv[i+1]);
					render = true;
					++i;
				}
			} else if(isPrm(argv[i], "-lw", "--line-width")) {															// -lt, --line-transparency
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The given argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					lineWidth = std::stoi(argv[i+1]);
					render = true;
					++i;
				}
			} else if(isPrm(argv[i], "-add", "--additive")) {															// -add, --additive
				additive = true;
				render = true;
			} else if(isPrm(argv[i], "-sub", "--subtractive")) {														// -sub, --subtractive
				additive = false;
				render = true;
			} else if(isPrm(argv[i], "-gray", "--grayscale")){															// -gray, --grayscale
				grayscale = true;
				render = true;
			} else if(isPrm(argv[i], "-col", "--color")){																// -col, --color
				grayscale = false;
				render = true;
			} else if(isPrm(argv[i], "-res", "--resize")) {																// -res, --resize
				if(i+2>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The first argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else if(!isNumber(argv[i+2])){
					std::cout << "The second argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					resize = true;
					resizeWidth = std::stoi(argv[i+1]);
					resizeHeight = std::stoi(argv[i+2]);
					render = true;
					i += 2;
				}
			} else if(isPrm(argv[i], "-tc", "--thread-count")) {														// -tc, --thread-count
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The given argument for \"" << argv[i] << "\" is not a number" << std::endl;
					return -1;
				} else {
					threadCount = std::stoi(argv[i+1]);
					render = true;
					++i;
				}
			} else if(isPrm(argv[i], "-inp", "--input-png")) {															// -inp, --input-png
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(!fileExists(argv[i+1])){
						std::cout << "The given input file does not exist. Please specify a path to an existing file." << std::endl;
						return -1;
					} else {
						inputFile = argv[i + 1];
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-inj", "--input-jpeg")) {															// -inj, --input-jpeg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(!fileExists(argv[i+1])){
						std::cout << "The given input file does not exist. Please specify a path to an existing file." << std::endl;
						return -1;
					} else {
						inputFile = argv[i + 1];
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outs", "--output-svg")) {														// -outs, --output-svg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(argv[i+1])){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else {
						OutputFile file;
						file.filePath = argv[i + 1];
						file.type = OUT_F_SVG;
						outFiles.push_back(file);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outp", "--output-png")) {														// -outp, --output-png
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(argv[i+1])){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else {
						OutputFile file;
						file.filePath = argv[i + 1];
						file.type = OUT_F_PNG;
						outFiles.push_back(file);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outj", "--output-jpeg")) {														// -outj, --output-jpeg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(argv[i+1])){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else {
						OutputFile file;
						file.filePath = argv[i + 1];
						file.type = OUT_F_JPEG;
						outFiles.push_back(file);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outsp", "--output-svg-png")) {													// -outsp, --output-svg-png
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(std::string(argv[i+1])+".svg")){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else if(fileExists(std::string(argv[i+1])+".png")){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					}  else {
						//SVG-File
						OutputFile sfile;
						sfile.filePath = argv[i+1];
						sfile.type = OUT_F_SVG;
						outFiles.push_back(sfile);
						//PNG-File
						OutputFile pfile;
						pfile.filePath = argv[i+1];
						pfile.type = OUT_F_PNG;
						outFiles.push_back(pfile);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outsj", "--output-svg-jpeg")) {													// -outsj --output-svg-jpeg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(!(fileExists(std::string(argv[i+1])+".svg"))){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else if(!(fileExists(std::string(argv[i+1])+".jpeg"))){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else {
						//SVG-File
						OutputFile sfile;
						sfile.filePath = argv[i+1];
						sfile.type = OUT_F_SVG;
						outFiles.push_back(sfile);
						//JPEG-File
						OutputFile jfile;
						jfile.filePath = argv[i+1];
						jfile.type = OUT_F_JPEG;
						outFiles.push_back(jfile);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outpj", "--output-png-jpeg")) {													// -outsj --output-png-jpeg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(std::string(argv[i+1])+".png")){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					} else if(fileExists(std::string(argv[i+1])+".jpeg")){
						std::cout << "The file " << argv[i+1] << " already exists. Please specify a different path." << std::endl;
						return -1;
					}  else {
						//PNG-File
						OutputFile pfile;
						pfile.filePath = argv[i+1];
						pfile.type = OUT_F_PNG;
						outFiles.push_back(pfile);
						//JPEG-File
						OutputFile jfile;
						jfile.filePath = argv[i+1];
						jfile.type = OUT_F_JPEG;
						outFiles.push_back(jfile);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-outspj", "--output-svg-png-jpeg")) {												// -outspj --output-svg-png-jpeg
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					if(fileExists(std::string(argv[i+1])+".png")){
						std::cout << "The file " << argv[i+1] << ".png already exists. Please specify a different path." << std::endl;
						return -1;
					} else if(fileExists(std::string(argv[i+1])+".jpeg")){
						std::cout << "The file " << argv[i+1] << ".jpeg already exists. Please specify a different path." << std::endl;
						return -1;
					} else if(fileExists(std::string(argv[i+1])+".svg")){
						std::cout << "The file " << argv[i+1] << ".svg already exists. Please specify a different path." << std::endl;
						return -1;
					} else {
						//PNG-File
						OutputFile pfile;
						pfile.filePath = argv[i+1];
						pfile.type = OUT_F_PNG;
						outFiles.push_back(pfile);
						//JPEG-File
						OutputFile jfile;
						jfile.filePath = argv[i+1];
						jfile.type = OUT_F_JPEG;
						outFiles.push_back(jfile);
						//SVG-File
						OutputFile sfile;
						sfile.filePath = argv[i+1];
						sfile.type = OUT_F_SVG;
						outFiles.push_back(sfile);
						render = true;
						++i;
					}
				}
			} else if(isPrm(argv[i], "-svgrmlines", "--modify-custom-svg-remove-lines")) {								// -svgrmlines --modify-custom-svg-remove-lines TODO: implement: check if file exists in all convert methods
				if(i+2>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The first argument for \"" << argv[i] << "\" is not a number." << std::endl;
					return -1;
				} else {
					convert = true;
					i += 2;
				}
			} else if(isPrm(argv[i], "-svgmodlinet", "--modify-custom-svg-mod-line-transparency")) {					// -svgmodlinet --modify-custom-svg-mod-line-transparency
				if(i+2>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The first argument for \"" << argv[i] << "\" is not a number." << std::endl;
					return -1;
				} else {
					convert = true;
					i += 2;
				}
			} else if(isPrm(argv[i], "-svgmodlinew", "--modify-custom-svg-mod-line-width")) {							// -svgmodlinew --modify-custom-svg-mod-line-width
				if(i+2>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else if(!isNumber(argv[i+1])){
					std::cout << "The first argument for \"" << argv[i] << "\" is not a number." << std::endl;
					return -1;
				} else {
					convert = true;
					i += 2;
				}
			} else if(isPrm(argv[i], "-svginvertcol", "--modify-custom-svg-invert-colors")) {							// -svginvertcol --modify-custom-svg-invert-colors
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					convert = true;
					++i;
				}
			} else if(isPrm(argv[i], "-svgconvert", "--convert-svg-to-pixel-format")) {									// -svgconvert --convert-svg-to-pixel-format
				if(i+1>argc){
					std::cout << "Not enough arguments for \"" << argv[i] << "\"." << std::endl;
					return -1;
				} else {
					convert = true;
					++i;
				}
			} else if(isPrm(argv[i], "-nop", "--no-progress")) {														// -nop --no-progress
				showProgress = false;
				render = true;
			} else if(isPrm(argv[i], "-now", "--no-warnings")) {														// -now --no-warnings
				showWarnings = false;
				render = true;
			} else if(isPrm(argv[i], "-noe", "--no-errors")) {															// -noe --no-errors
				showErrors = false;
				render = true;
			} else if(isPrm(argv[i], "-q", "--quiet")) {																// -q --quiet
				showProgress = false;
				showWarnings = false;
				showErrors = false;
				render = true;
			}
		}
		if(convert && !render){
			std::cout << "Not yet implemented." << std::endl;
			return 0;
		} else if(!convert && render){
			if(!inputFile.empty()) {
				std::chrono::steady_clock::time_point a, b;
				a = std::chrono::steady_clock::now();
				std::cout << "Starting render sequence." << std::endl;
				loadAndRedrawImage(lineCount, // lineCount
								   testLineCount, // testLineCount
								   lineOpacity, // lineOpacity,
								   lineWidth, // lineWidth,
								   additive, // additive, // false -> subtractive
								   grayscale, // grayscale, // false -> color mode (not implemented)
								   resize, // resize, // not implented
								   resizeWidth, // resizeWidth, // not implented
								   resizeHeight, // resizeHeight, // not implented
								   threadCount, // threadCount,
								   inputFile, // inputFile // jpeg or png
								   outFiles,
								   showProgress, // showProgress,
								   showWarnings, // showWarnings,
								   showErrors); // showErrors);
				std::cout << "Ending render sequence." << std::endl;
				b = std::chrono::steady_clock::now();
				std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(b - a);
				std::cout << "duration: " << ms.count() << " milliseconds" << std::endl;
				return 0;
			} else {
				std::cout << "No input file was found." << std::endl;
				return -1;
			}
		} else {
			std::cout << "Too many options given. This tool cannot convert and render at the same time" << std::endl;
			return -1;
		}

	}
}

bool isNumber(const std::string& s) {
	if(!s.empty()) {
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	} else return false;
}

inline bool fileExists (const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

bool isPrm(char *arg, const char *variant0, const char *variant1) {
	if(strcmp(arg, variant0) == 0)
		return true;
	if(strcmp(arg, variant1) == 0)
		return true;
	return false;
}


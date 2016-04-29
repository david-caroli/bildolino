#include <iostream>
#include <fstream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <string>

using namespace boost::gil;
using namespace std;

int main(int argc, char **argv) {
	std::cout << "start" << std::endl;
	gray8_image_t input_img;
	jpeg_read_and_convert_image("fullhdtestimage.jpg", input_img);
	gray8_view_t input = view(input_img);
	std::cout << "loaded" << std::endl;
	int linecount = 1000;
	while (linecount<20000){
		ofstream outfile;
		outfile.open("randrom-"+to_string(linecount)+".svg");
		outfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << input.width() << "\" height=\"" << input.height() << "\" style = \"background: white\">\n";
		for(int y=0; y<linecount; y++){
			outfile << "	<line x1=\"0\" y1=\"" << rand() % input.height() << "\" x2=\"" << input.width() << "\" y2=\"" << rand() % input.height() << "\" stroke=\"black\" stroke-width=\"0.5\" />\n";
		}
		outfile << "</svg>";
		outfile.close();
		linecount *= 2;
	};
	std::cout << "dimensons: " << input.width() << "*" << input.height() << std::endl;
	std::cout << "end" << std::endl;
	return 0;
}

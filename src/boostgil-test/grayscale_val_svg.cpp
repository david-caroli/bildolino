#include <iostream>
#include <fstream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <string>

using namespace boost::gil;
using namespace std;

int main(int argc, char **argv) {
	std::cout << "start" << std::endl;
	gray8_image_t input_img;
	png_read_and_convert_image("turn.png", input_img);
	gray8_view_t input = view(input_img);
	std::cout << "loaded" << std::endl;
	ofstream outfile;
	outfile.open("littlesvg.svg");
	outfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << input.width() << "\" height=\"" << input.height() << "\" style = \"background: white\">\n";
	for(int y=0; y<input.height(); y++){
		outfile << "	<line x1=\"0\" y1=\"0\" x2=\"" << input.width() << "\" y2=\"" << y << "\" stroke=\"black\" stroke-width=\"0.5\" />\n";
	}
	outfile << "</svg>";
	outfile.close();
	std::cout << "dimensons: " << input.width() << "*" << input.height() << std::endl;
	std::cout << "end" << std::endl;
	return 0;
}

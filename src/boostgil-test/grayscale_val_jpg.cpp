#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>

using namespace boost::gil;

int main(int argc, char **argv) {
	std::cout << "start" << std::endl;
	gray8_image_t input_img;
	jpeg_read_and_convert_image("grayscale_val.jpg", input_img);
	gray8_view_t input = view(input_img);
	std::cout << "loaded" << std::endl;
	std::cout << "dimensons: " << input.width() << "*" << input.height() << std::endl;
	gray8_image_t output_img(input.height(), input.width());
	gray8_view_t output = view(output_img);

	for(int y=0; y<input.height(); ++y) {
		for(int x=0; x<input.width(); ++x) {
			std::cout << "(" << x << "|" << y << "): " << ((int)input(x,y)) << std::endl;
		}
	}

	std::cout << "end" << std::endl;
	return 0;
}

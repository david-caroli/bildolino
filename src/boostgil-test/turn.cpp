#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

using namespace boost::gil;



void turn90(rgb8_view_t &in, rgb8_view_t &out, unsigned int count) {
	count %= 4;
	for(int y=0; y<in.height(); ++y) {
		for(int x=0; x<in.width(); ++x) {
			switch(count) {
			case 1:
				out(in.height()-(y+1), x) = in(x,y);
				break;
			case 2:
				out(in.width()-(x+1), in.height()-(y+1)) = in(x,y);
				break;
			case 3:
				out(y, in.width()-(x+1)) = in(x,y);
				break;
			}
		}
	}
}



int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "needs exactly 1 parameter. example:" << std::endl << std::endl;
		std::cout << "./turn 90" << std::endl;
		std::cout << "./turn 180" << std::endl;
		std::cout << "./turn 270" << std::endl;
		return -1;
	}
	unsigned int turncount;
	if(strcmp(argv[1], "90") == 0) {
		turncount = 1;
	} else if(strcmp(argv[1], "180") == 0) {
		turncount = 2;
	} else if(strcmp(argv[1], "270") == 0) {
		turncount = 3;
	} else {
		std::cout << "invalid parameter. try:" << std::endl << std::endl;
		std::cout << "./turn 90" << std::endl;
		std::cout << "./turn 180" << std::endl;
		std::cout << "./turn 270" << std::endl;
		return -2;
	}
	
	std::cout << "start" << std::endl;
	rgb8_image_t input_img;
	png_read_and_convert_image("turn.png", input_img);
	rgb8_view_t input = view(input_img);
	std::cout << "loaded" << std::endl;
	std::cout << "dimensons: " << input.width() << "*" << input.height() << std::endl;
	rgb8_image_t output_img(input.height(), input.width());
	rgb8_view_t output = view(output_img);
	
	turn90(input, output, turncount);
	
	png_write_view("turn.png", output);
	std::cout << "end" << std::endl;
	return 0;
}

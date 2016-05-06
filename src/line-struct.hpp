#ifndef LINE_STRUCT_HPP
#define LINE_STRUCT_HPP


typedef struct {
	int x;
	int y;
} PixelCoords;


typedef struct {
	PixelCoords a;
	PixelCoords b;
	// keep opacity and width separate as they are the same for all lines
} LineData;




#endif // LINE_STRUCT_HPP

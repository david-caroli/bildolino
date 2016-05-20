#include "svg-files.hpp"

void initSvg(std::fstream &outFile, int height, int width, bool blackBackground, int lineOpacity, int lineThickness){
	outFile << "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width = \"" << width << "\" height = \"" << height << "\">";
	outFile << "\n\t<defs>";
	outFile << "\n\t\t<style type=\"text/css\"><![CDATA[";
	outFile << "\n\t\t\tline{";
	outFile << "\n\t\t\t\tstroke-width:"<< lineThickness <<" ;";
	outFile << "\n\t\t\t\tstroke-opacity:"<< ((float)lineOpacity/255) <<" ;";
	outFile << "\n\t\t\t\tstroke:"<< (blackBackground ? "white":"black") <<" ;";
	outFile << "\n\t\t\t}";
	outFile << "\n\t\t]]></style>";
	outFile << "\n\t</defs>";
	outFile << "\n<rect width = \""<< width <<"\" height = \""<< height <<"\" fill=\""<< (blackBackground ? "black":"white") <<"\"/>\n";
}

void addLineGrey(std::fstream &outFile, LineData lineCoords){
	outFile << "	<line x1=\"" << lineCoords.a.x << "\" y1=\"" << lineCoords.a.y << "\" x2=\"" << lineCoords.b.x << "\" y2=\"" << lineCoords.b.y << "\"/>\n";
}

void addLineCol(std::fstream &outFile, LineData lineCoords, std::string colour){
	outFile << "	<line x1=\"" << lineCoords.a.x << "\" y1=\"" << lineCoords.a.y << "\" x2=\"" << lineCoords.b.x << "\" y2=\"" << lineCoords.b.y << "\" stroke=\"" << colour << "\"/>\n";
}

void finishSvg(std::fstream &outFile){
	outFile << "</svg>";
}

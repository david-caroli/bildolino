#include "svg-files.hpp"

void initSvg(std::fstream &outFile, int height, int width, bool blackBackground, int lineOpacity, int lineThickness){
    outFile << "<xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width = \"" << width << "\" height = \"" << height << "\" style = \"background: \"" << (blackBackground ? "black":"white") << ">\n\t<defs>\n\t\t<style type=\"text/css\"><![CDATA[\n\t\t\tline{\n\t\t\t\tstroke-width:"<< lineThickness <<" ;\n\t\t\t\tstroke-opacity:"<< lineOpacity <<" ;\n\t\t\t}\n\t\t]]></style>\n\t</defs>\n";;
    outFile << ""
}

void addLineGrey(std::fstream &outFile, LineData lineCoords){
  outFile = "	<line x1=\"" << lineCoords.a.x << "\" y1=\"" << lineCoords.a.y << "\" x2=\"" << lineCoords.b.x << "\" y2=\"" << lineCoords.b.y << "\" stroke=\"black\"/>\n";
}

void addLineCol(std::fstream &outFile, LineData lineCoords, std::string colour){

}

void finishSvg(std::fstream &outFile){
  outFile << "</svg>";
}

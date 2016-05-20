### About  

Bildolinio will be a command line tool to redraw images by only using lines.  
The lines wil be drawn from one end of the image to the other (they don't end inside the image).  

The general idea and the basic greedy algorithm were inspired by the website [linify.me](http://linify.me).  
The tool is being written in c++ and we'll try to optimize it for speed as much as possible.  

### Example  

Original image:  
![original image](https://raw.githubusercontent.com/david-caroli/bildolino/master/example/example1-original.jpg)  

Bildolino will generate a svg image like this: [svg file](https://github.com/david-caroli/bildolino/blob/master/example/example1-result-generated.svg)  
(*Needs javascript enabled to render the svg image on github, but you can download the image if you want to check it out locally.*)  

The svg image can be converted to png, jpeg, or other image formats.  
The result might look like this (conversion done using gimp for the time being):  
![png result (gimp)](https://raw.githubusercontent.com/david-caroli/bildolino/master/example/example1-result-svg-to-png-with-gimp.png)  

### Dependencies  

We'll release statically compiled binaries, but here are Bildolino's dependencies anyway:  

* Boost GIL  
* libjpeg  
* libpng  
* libz  


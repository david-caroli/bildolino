#include "line-rectangle-intersection.hpp"



PixelCoords getLineBorderPoint(PixelCoords usablePoint, PixelCoords pointToBeReplaced, int maxX, int maxY) {
	PixelCoords result;
	PixelCoords a = usablePoint;
	PixelCoords b = pointToBeReplaced;
	float dX = b.x - a.x;
	float dY = b.y - a.y;
	float slope = dY / dX;
	float tiltedSlope = dX / dY;
	float x, y;
	// try left and right border, or a perfectly vertical line
	if(a.x == b.x) { // vertical line
		if(a.y == 0) {
			result.x = b.x;
			result.y = maxY;
		} else {
			result.x = b.x;
			result.y = 0;
		}
		return result;
	} else if(a.x < b.x) { // try right side
		y = (float)(a.y) + (slope * (float)(maxX-a.x));
		if(0<=y && y<=maxY) {
			result.x = maxX;
			result.y = y;
			return result;
		}
	} else { // try left side
		y = (float)(a.y) + (-slope * (float)(a.x/*-0*/));
		if(0<=y && y<=maxY) {
			result.x = 0;
			result.y = y;
			return result;
		}
	}
	// try top and bottom border, or a a perfectly horizontal line
	if(a.y == b.y) { // horizontal line
		if(a.x == 0) {
			result.x = maxX;
			result.y = b.y;
		} else {
			result.x = 0;
			result.y = b.y;
		}
		return result;
	} else if(a.y < b.y) { // try bottom
		x = (float)(a.x) + (tiltedSlope * (float)(maxY-a.y));
		if(0<=x && x<=maxX) {
			result.y = maxY;
			result.x = x;
			return result;
		}
	} else { // try top
		x = (float)(a.x) + (-tiltedSlope * (float)(a.y/*-0*/));
		if(0<=x && x<=maxX) {
			result.y = 0;
			result.x = x;
			return result;
		}
	}

	// this point should be impossible to reach, if the requirements for both points are fullfilled
	return usablePoint; // not good
}

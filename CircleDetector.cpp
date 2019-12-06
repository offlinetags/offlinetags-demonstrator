/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CircleDetector.h"


CircleDetector::CircleDetector(void)
{

	minDist = 15;
	param1 = 75;
	param2 = 275;
	minRadius = 0; 
	maxRadius = 500;

}

/**
* Finds circles in a given cv::Mat.
* Returns a vector of cv::Vec3f containing x- and y-coordinate and radius of each circle
*/
std::vector<cv::Vec3f> CircleDetector::detectCircles( cv::Mat &image ){

	std::vector<cv::Vec3f> result;

	//TODO modify these params for better results
	cv::HoughCircles(image, result, CV_HOUGH_GRADIENT, 2, minDist, param1, param2, minRadius, maxRadius);
	/*

	    image – grayscale input image.
		circles – Output vector of found circles. Each vector is encoded as a 3-element floating-point vector (x, y, radius) .
		method – The only implemented method is CV_HOUGH_GRADIENT
		dp – Inverse ratio of the accumulator resolution to the image resolution.
		minDist – Minimum distance between the centers of the detected circles. If the parameter is too small, multiple neighbor circles may be falsely detected in addition to a true one. 
					If it is too large, some circles may be missed.
		param1 – It is the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller).
			Set lower, to find more circles.
		param2 – It is the accumulator threshold for the circle centers at the detection stage.  
			The smaller it is, the more false circles may be detected.
		minRadius – Minimum circle radius.
		maxRadius – Maximum circle radius.

	*/

	return result;

}

CircleDetector::~CircleDetector(void)
{
}

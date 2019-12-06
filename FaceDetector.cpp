/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FaceDetector.h"

FaceDetector::FaceDetector(char* filename)
{
	cascadeClassifier.load(filename);
}

/**
* Returns a vector of cv::Rect, where faces in a cv::Mat can be found
*/
std::vector<cv::Rect> FaceDetector::detectFaces(cv::Mat &image){

	std::vector<cv::Rect> faces;

	cascadeClassifier.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(0,0), cv::Size(image.rows, image.cols));

	return faces;

}

FaceDetector::~FaceDetector(void)
{
}

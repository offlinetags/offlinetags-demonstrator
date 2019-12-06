/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include "FaceTagPair.h"
#include <vector>
#include <limits>
#include <math.h>

class FaceManipulator
{
public:
	FaceManipulator(void);
	~FaceManipulator(void);

	void manipulateFaces(cv::Mat *image,
						std::vector<cv::Rect> faces,
						std::vector<cv::Vec3f> blue,
						std::vector<cv::Vec3f> green,
						std::vector<cv::Vec3f> red,
						std::vector<cv::Vec3f> yellow);

private:
	std::vector<FaceTagPair> pairs;

	void matchPairs(std::vector<cv::Vec3f> tags, FaceTagPair::Color color);

	void blurMe(cv::Rect *face, cv::Mat *image);
	void tagMe(cv::Rect *face, cv::Mat *image);
	void uploadMe(cv::Rect *face, cv::Mat *image);
	void noPhoto(cv::Rect *face, cv::Mat *image);
	void unknown(cv::Rect *face, cv::Mat *image);

	cv::Mat getRoi(cv::Mat *image, cv::Rect *roi);
	
};


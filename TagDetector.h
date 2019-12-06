/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <math.h>

#include"CircleDetector.h"

class TagDetector
{
public:
	TagDetector(void);
	~TagDetector(void);

	void detectTags(std::vector<cv::Vec3f> *roi,
									  std::vector<cv::Vec3f> *blue,
									  std::vector<cv::Vec3f> *green,
									  std::vector<cv::Vec3f> *red,
									  std::vector<cv::Vec3f> *yellow,
									  CircleDetector *circleDetector,
									  cv::Mat *image);

	int threshold1, threshold2;
	bool dillate;

private:
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

};


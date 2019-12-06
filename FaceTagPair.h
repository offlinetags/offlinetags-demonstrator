/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include <limits>

class FaceTagPair
{
public:

	
	enum Color {UNKNOWN, BLUE, GREEN, RED, YELLOW};

	FaceTagPair(void);
	~FaceTagPair(void);

	void setFace(cv::Rect *facePtr);
	cv::Rect *getFace();

	void setTag(cv::Vec3f *tagPtr);
	cv::Vec3f *getTag();

	void setDistance(double distance);
	double getDistance();

	void setColor(Color color);
	Color getColor();



private:
	cv::Rect *face;
	cv::Vec3f *tag;
	Color color;

	double distance;

};


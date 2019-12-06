/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QImage>

#include "ImageLoader.h"
#include "FaceDetector.h"
#include "CircleDetector.h"
#include "TagDetector.h"
#include "FaceManipulator.h"

class ImageAnalyzer
{

public:
	ImageAnalyzer(void);

	QImage getImage();
	~ImageAnalyzer(void);

	ImageLoader *loader;
	FaceDetector *faceDetector;
	CircleDetector *circleDetector;
	TagDetector *tagDetector;
	FaceManipulator *manipulator;

	bool blur;
	int blurValue;

	bool configureContrast1, configureContrast2, configureContrast3, configureContrast4;
	double contrast1, contrast2, contrast3, contrast4;
	int brightness1, brightness2, brightness3, brightness4;

	bool showFaces, showCircles, showTags, showManipulation;

private:
	QImage getQImage(cv::Mat &mat);	
	void changeSaturationAndBrightness(cv::Mat &image, double saturation, double brightness);

	std::vector<cv::Rect> faces;
	std::vector<cv::Vec3f> circles;
	std::vector<cv::Vec3f> blue;
	std::vector<cv::Vec3f> green;
	std::vector<cv::Vec3f> red;
	std::vector<cv::Vec3f> yellow;
};


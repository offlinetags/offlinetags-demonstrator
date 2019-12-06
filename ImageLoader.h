/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include <QImage>
#include <QMessagebox>

class ImageLoader
{
public:
    ImageLoader();
    cv::Mat getMat();
	~ImageLoader();

	void setDevice(int device);

private:
	cv::VideoCapture *capture;
};


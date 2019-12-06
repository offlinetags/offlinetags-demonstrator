/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "imageloader.h"
#include <iostream>
#include <QImage>

ImageLoader::ImageLoader()
{
	capture = new cv::VideoCapture(0);
}

/**
* Returns a snapshot from the camera device.
* If no device is opened, it will try to open device 0.
* WARNING: If device 0 can not be opened this will hang up in a loop
*/
cv::Mat ImageLoader::getMat(){

	// if we can't open the device, we put out a error message and open device 0
	while(!capture->isOpened()){
		QMessageBox msgBox;
		msgBox.setStyleSheet("QLabel{background-color: gray;}");
		msgBox.setText("Please connect a camera device.");
		msgBox.exec();
		capture->open(0);
    }

	cv::Mat frame;
    *capture >> frame; // get a new frame from camera
	*capture >> frame; // has to be called twice so we don't get outdated images

	return frame;
}

// open a device different to 0
void ImageLoader::setDevice(int device){
	delete capture;
	capture = new cv::VideoCapture(device);
}

ImageLoader::~ImageLoader()
{
	delete capture;
	capture = 0;
}
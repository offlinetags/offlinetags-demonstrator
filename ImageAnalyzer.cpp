/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ImageAnalyzer.h"


ImageAnalyzer::ImageAnalyzer(void)
{
	loader = new ImageLoader();
	faceDetector = new FaceDetector("lbpcascade_frontalface.xml");
	circleDetector = new CircleDetector();
	tagDetector = new TagDetector();
	manipulator = new FaceManipulator();

	blur = false;
	blurValue = 3;

	configureContrast1 = false;
	configureContrast2 = false;
	configureContrast3 = false;
	configureContrast4 = false;

	contrast1 = 1.0;
	contrast2 = 1.0;
	contrast3 = 1.0;
	contrast4 = 1.0;
	brightness1 = 0;
	brightness2 = 0; 
	brightness3 = 0; 
	brightness4 = 0;

	showFaces = false;
	showCircles = false;
	showTags = false; 
	showManipulation = true;
}

/**
* This is the function which calls all neccessary methods and return an output image
* following the class variables.
*
* TODO This method needs some improvements as it will take way too long when 
* all found circles have to be analysed.
*/
QImage ImageAnalyzer::getImage(){

	cv::Mat image;

	image = loader->getMat();

	// Face detection
	std::vector<cv::Rect> faces = faceDetector->detectFaces(image);

	// tag detection
	cv::Mat grayImage;
	cv::cvtColor(image, grayImage, CV_BGR2GRAY); // we need a grayscale image 

	std::vector<cv::Vec3f> circles = circleDetector->detectCircles(grayImage);

	// We can blur the image, if we find to many circles
	if(blur){
		cv::GaussianBlur(grayImage, grayImage, cv::Size(blurValue, blurValue), 2, 2);
	}

	tagDetector->detectTags(&circles, &blue, &green, &red, &yellow, circleDetector, &grayImage);

	// Contrast and brightness can be configured 4 times (for each tag once)
	if(configureContrast1){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast1, brightness1);
		std::vector<cv::Vec3f> tmpCircles = circleDetector->detectCircles(tmp);
		for (int i = 0; i < tmpCircles.size(); i++){
			circles.push_back(tmpCircles[i]);
		}
	}
	if(configureContrast2){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast2, brightness2);
		std::vector<cv::Vec3f> tmpCircles = circleDetector->detectCircles(tmp);
		for (int i = 0; i < tmpCircles.size(); i++){
			circles.push_back(tmpCircles[i]);
		}
	}
	if(configureContrast3){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast3, brightness3);
		std::vector<cv::Vec3f> tmpCircles = circleDetector->detectCircles(tmp);
		for (int i = 0; i < tmpCircles.size(); i++){
			circles.push_back(tmpCircles[i]);
		}
	}
	if(configureContrast4){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast4, brightness4);
		std::vector<cv::Vec3f> tmpCircles = circleDetector->detectCircles(tmp);
		for (int i = 0; i < tmpCircles.size(); i++){
			circles.push_back(tmpCircles[i]);
		}
	}
	
	// we have to repeat the transformation steps from  before so we can handle all found circles
	if(configureContrast1){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast1, brightness1);
		tagDetector->detectTags(&circles, &blue, &green, &red, &yellow, circleDetector, &tmp);	
	}
	if(configureContrast2){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast2, brightness2);
		tagDetector->detectTags(&circles, &blue, &green, &red, &yellow, circleDetector, &tmp);
	}
	if(configureContrast3){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast3, brightness3);
		tagDetector->detectTags(&circles, &blue, &green, &red, &yellow, circleDetector, &tmp);
	}
	if(configureContrast4){
		cv::Mat tmp;
		grayImage.copyTo(tmp);
		changeSaturationAndBrightness(tmp, contrast4, brightness4);
		tagDetector->detectTags(&circles, &blue, &green, &red, &yellow, circleDetector, &tmp);
	}

	// draw rectangles around faces
	if(showFaces){
		for each (cv::Rect face in faces){
			cv::rectangle(image, face, cv::Scalar(123, 123, 123, 123));
		}
	}

	// draw circles around possible markers
	if(showCircles){
		for each (cv::Vec3f circle in circles){
			cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			int radius = cvRound(circle[2]);
			// draw the circle center
			cv::circle( image, center, 3, cv::Scalar(255,0,255), -1, 8, 0 );
			// draw the circle outline
			cv::circle( image, center, radius, cv::Scalar(255,0,255), 3, 8, 0 );
		}
	}

	if(showTags){
		// draw blue circles around blue markers
		for each (cv::Vec3f circle in blue){
			cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			int radius = cvRound(circle[2]);
			// draw the circle center
			cv::circle( image, center, 3, cv::Scalar(255,0,0), -1, 8, 0 );
			// draw the circle outline
			cv::circle( image, center, radius, cv::Scalar(255,0,0), 3, 8, 0 );
		}

		// draw yellow circles around yellow markers
		for each (cv::Vec3f circle in yellow){
			cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			int radius = cvRound(circle[2]) - 8;
			// draw the circle center
			cv::circle( image, center, 3, cv::Scalar(0,255,255), -1, 8, 0 );
			// draw the circle outline
			cv::circle( image, center, radius, cv::Scalar(0,255,255), 3, 8, 0 );
		}

		// draw red circles around red markers
		for each (cv::Vec3f circle in red){
			cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			int radius = cvRound(circle[2]) - 6;
			// draw the circle center
			cv::circle( image, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
			// draw the circle outline
			cv::circle( image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
		}

		// draw green circles around green markers
		for each (cv::Vec3f circle in green){
			cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			int radius = cvRound(circle[2]) - 4;
			// draw the circle center
			cv::circle( image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
			// draw the circle outline
			cv::circle( image, center, radius, cv::Scalar(0,255,0), 3, 8, 0 );
		}
	}

	if(showManipulation){
		manipulator->manipulateFaces(&image, faces, blue, green, red, yellow);
	}

	blue.clear();
	yellow.clear();
	red.clear();
	green.clear();

	return getQImage(image);

}

/**
* Converts a cv::Mat to a QImage
*/
QImage ImageAnalyzer::getQImage(cv::Mat &mat){
		
    // convert the Mat to QImage
    QImage qImage(
            mat.data,
            mat.size().width,
            mat.size().height,
            mat.step,
            QImage::Format_RGB888
        );

    return qImage.rgbSwapped();
}

ImageAnalyzer::~ImageAnalyzer(void)
{
	delete loader;
	loader = 0;
	delete faceDetector;
	faceDetector = 0;
	delete circleDetector;
	circleDetector = 0;
	delete tagDetector;
	tagDetector = 0;
	delete manipulator;
	manipulator = 0;
}

/**
* Wrapper class to change saturation and brightness of a given cv::Mat
*/
void ImageAnalyzer::changeSaturationAndBrightness(
			cv::Mat &image, 
			double saturation, 
			double brightness){

	image.convertTo(image, -1, saturation, brightness);
}


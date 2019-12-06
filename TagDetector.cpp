/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TagDetector.h"

TagDetector::TagDetector(void)
{
	threshold1 = 50;
	threshold2 = 150;
	dillate = false;
}


TagDetector::~TagDetector(void)
{
}

/**
* Detects the defined tags in the region of interest of a given cv::Mat.
* The found tags will be written into the vectors directly.
*/
void TagDetector::detectTags(std::vector<cv::Vec3f> *roi,
								  std::vector<cv::Vec3f> *blue,
								  std::vector<cv::Vec3f> *green,
								  std::vector<cv::Vec3f> *red,
								  std::vector<cv::Vec3f> *yellow,
								  CircleDetector *circleDetector,
								  cv::Mat *image){

						
	for each(cv::Vec3f region in *roi){

		// cut off x and y on (0,0) and image->size()
		int x0 = std::max(0, std::min(cvRound(region[0] - region[2]), image->cols - 1));
		int x1 = std::max(1, std::min(cvRound(region[0] + region[2]), image->cols));
		int y0 = std::max(0, std::min(cvRound(region[1] - region[2]), image->rows - 1));
		int y1 = std::max(1, std::min(cvRound(region[1] + region[2]), image->rows));

		// get the region of interest
		cv::Mat roiPtr = (*image)(cv::Range(y0, y1),
								  cv::Range(x0, x1));
		cv::Mat temp;
		// we have to make a copy of the region or else the original image
		// is changed which will manipulate other regions
		roiPtr.copyTo(temp);

		// check for the green button first, because the 
		// region will be manipulated
		if(circleDetector->detectCircles(temp).size() >= 1){
				green->push_back(region);
		}

		// apply canny to filter out the edges
		Canny( temp, temp, threshold1, threshold2, 3, true );

		// dillate can help if the contours are uncontinous
		if(dillate){
			cv::dilate(temp, temp, cv::Mat(), cv::Point(-1,-1));
		}

		// store all contours
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(temp, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		std::vector<cv::Point> approx;
		
		for(size_t i = 0; i < contours.size(); i++){

			// filter out the important points from the contour
			cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);
			
			// 4 = square, 6 = check mark, 12 = X
			// filter out noisy contours < 20
			if((approx.size() == 4  || approx.size() == 6 || approx.size() == 12) && 
				std::fabs(cv::contourArea(cv::Mat(approx))) > 20){

					double maxCosine = 0;

					// the max cosine has to be lower than 0.3 so we can assume, 
					// that all angles are ~90 degrees
					for( int j = 2; j < approx.size() + 1; j++ ){
						double cosine =  std::fabs(angle(approx[j%approx.size()], approx[j-2], approx[j-1]));
						maxCosine = std::max(maxCosine, cosine);
					}

					// save the result
					if(maxCosine < 0.3){
						if(approx.size() == 4){
							blue->push_back(region);
							break;
						} 
						else if(approx.size() == 6){
							yellow->push_back(region);
							break;
						}
						else if (approx.size() == 12){
							red->push_back(region);
							break;
						}
					}

			}
		
		}
	}
}

// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double TagDetector::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0){
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
			
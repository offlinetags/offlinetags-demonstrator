/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FaceManipulator.h"


FaceManipulator::FaceManipulator(void)
{
}


FaceManipulator::~FaceManipulator(void)
{
}

/**
* This method connects the tags to the faces and marks the faces as defined
*/
void FaceManipulator::manipulateFaces(cv::Mat *image,
						std::vector<cv::Rect> faces,
						std::vector<cv::Vec3f> blue,
						std::vector<cv::Vec3f> green,
						std::vector<cv::Vec3f> red,
						std::vector<cv::Vec3f> yellow){


	if(faces.size() == 0){
		return;
	}

	pairs.clear();

	for (int i = 0; i < faces.size(); i++){
		FaceTagPair tmp = FaceTagPair();
		tmp.setFace(&(faces[i]));
		pairs.push_back(tmp);
	}

	matchPairs(red, FaceTagPair::RED);
	matchPairs(blue, FaceTagPair::BLUE);
	matchPairs(yellow, FaceTagPair::YELLOW);
	matchPairs(green, FaceTagPair::GREEN);

	for (int i = 0; i < pairs.size(); i++){
		switch (pairs[i].getColor()){
			case FaceTagPair::RED: 
				noPhoto(pairs[i].getFace(), image);
				break;
			case FaceTagPair::BLUE: 
				blurMe(pairs[i].getFace(), image);
				break;
			case FaceTagPair::YELLOW: 
				uploadMe(pairs[i].getFace(), image);
				break;
			case FaceTagPair::GREEN: 
				tagMe(pairs[i].getFace(), image);
				break;
			case FaceTagPair::UNKNOWN: 
				unknown(pairs[i].getFace(), image);
		}
	}

}

/**
* This method contains the algorithm to connect each face to a group of tags
*/
void FaceManipulator::matchPairs(std::vector<cv::Vec3f> tags, FaceTagPair::Color color){
	
	// search a matching face for every tag, that exists
	for(int i = 0; i < tags.size(); i++){

		double minDist = std::numeric_limits<double>::max(); // set minimum to a high value

		// compare each tag to each face
		for(std::vector<FaceTagPair>::iterator pair = pairs.begin(); pair != pairs.end(); pair++){
			
			// distance between face and tag
			double distance = sqrt(
				pow( tags[i][0] - pair->getFace()->x -( pair->getFace()->width / 2) ,2) + 
				pow(tags[i][1] - pair->getFace()->y - (pair->getFace()->height / 2), 2));

			// we only have to handle this, if the distance is lower than the actual minimum
			if(distance < minDist && distance < pair->getDistance()){

				// we found a possible match. Set the values to  the pair
				pair->setTag(&tags[i]);
				pair->setColor(color);
				pair->setDistance(distance);

				// check for every face if there already is a better result
				for(std::vector<FaceTagPair>::iterator existingPair = pairs.begin(); existingPair != pairs.end(); existingPair++){

					// if any existing pair already owns this tag, we  have to look, which face is closer
					if(*existingPair->getTag() == tags[i] && existingPair->getFace() != pair->getFace()){
						if(existingPair->getDistance() > distance){
							// reset the existing pair
							existingPair->setColor(FaceTagPair::UNKNOWN);
							existingPair->setDistance(std::numeric_limits<double>::max());
							existingPair->setTag(&cv::Vec3f(-1, -1, -1));
						}
						else{
							// reset the actual handled pair (we don't need to check any further pairs
							// since every tag can only be set to one other pair)
							pair->setColor(FaceTagPair::UNKNOWN);
							pair->setDistance(std::numeric_limits<double>::max());
							pair->setTag(&cv::Vec3f(-1, -1, -1));
							break;
						}
					}
				}
				minDist = distance;
			}

		}
	}
}

/**
* Blurs a face and draws a blue border
*/ 
void FaceManipulator::blurMe(cv::Rect *face, cv::Mat *image){
	cv::Mat roiPtr = getRoi(image, face);
	cv::blur(roiPtr, roiPtr, cv::Size(100, 100));
	cv::Mat copy;
	image->copyTo(copy);
	cv::rectangle(copy, *face, cv::Scalar(255, 0, 0), face->width * 3 / 100);
	cv::addWeighted(*image, 0.5, copy, 0.5, 0.0, *image);
	
}

/**
* Draws a green border around a face
*/
void FaceManipulator::tagMe(cv::Rect *face, cv::Mat *image){
	cv::Mat copy;
	image->copyTo(copy);
	cv::rectangle(copy, *face, cv::Scalar(0, 255, 0), face->width * 3 / 100);
	cv::addWeighted(*image, 0.5, copy, 0.5, 0.0, *image);
}

/**
* Draws a yellow border around a face and writes 'Upload Me!' to it
*/
void FaceManipulator::uploadMe(cv::Rect *face, cv::Mat *image){
	cv::Mat copy;
	image->copyTo(copy);
	cv::putText(copy, "Upload Me!", cv::Point(face->x + face->width * 5 / 100, face->y + face->height * 90 / 100), cv::FONT_HERSHEY_COMPLEX, face->height * 5 / 1000.0, cv::Scalar(0, 255, 255));
	cv::rectangle(copy, *face, cv::Scalar(0, 255, 255), face->width * 1 / 100);
	cv::addWeighted(*image, 0.5, copy, 0.5, 0.0, *image);
}

/**
* Draws a red cross over a face
*/
void FaceManipulator::noPhoto(cv::Rect *face, cv::Mat *image){
	cv::Mat copy;
	image->copyTo(copy);
	//cv::Mat roiPtr = getRoi(image, face);
	cv::line(copy, cv::Point(face->x, face->y), cv::Point(face->x + face->width, face->y + face->height), cv::Scalar(0, 0, 255), face->width / 10);
	cv::line(copy, cv::Point(face->x + face->width, face->y), cv::Point(face->x , face->y + face->height), cv::Scalar(0, 0, 255), face->width / 10);
	cv::addWeighted(*image, 0.5, copy, 0.5, 0.0, *image);
}

/**
* Draws a question mark over a face
*/
void FaceManipulator::unknown(cv::Rect *face, cv::Mat *image){	
	cv::Mat copy;
	image->copyTo(copy);
	cv::putText(copy, "?", cv::Point(face->x, face->y + face->height), cv::FONT_HERSHEY_PLAIN, face->height / 10, cv::Scalar(255, 255, 0), face->height / 30);
	cv::addWeighted(*image, 0.91, copy, 0.09, 0.0, *image);
}

/**
* Returns the region of interest of an image as Mat (that stills points to the part of the original Mat)
*/
cv::Mat FaceManipulator::getRoi(cv::Mat *image, cv::Rect *roi){
	cv::Mat roiPtr = (*image)(cv::Range(roi->y, roi->y + roi->height),
							  cv::Range(roi->x, roi->x + roi->width));
	return roiPtr;
}
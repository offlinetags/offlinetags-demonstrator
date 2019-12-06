/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FaceTagPair.h"


/**
* This class defines object in which a face is connected to a tag
*/

FaceTagPair::FaceTagPair(void)
{
	color = Color::UNKNOWN;
	face = 0;
	tag = 0;
	distance = std::numeric_limits<double>::max( );
	tag = &cv::Vec3f(-1, -1, -1);
}


FaceTagPair::~FaceTagPair(void)
{
}


void FaceTagPair::setFace(cv::Rect *facePtr){
	face = facePtr;
}

cv::Rect* FaceTagPair::getFace(){
	return face;
}

void FaceTagPair::setTag(cv::Vec3f *tagPtr){
	tag = tagPtr;
}

cv::Vec3f* FaceTagPair::getTag(){
	return tag;
}

void FaceTagPair::setDistance(double distance){
	this->distance = distance;
}
	
double FaceTagPair::getDistance(){
	return distance;
}

void FaceTagPair::setColor(Color color){
	this->color = color;
}

FaceTagPair::Color FaceTagPair::getColor(){
	return color;
}
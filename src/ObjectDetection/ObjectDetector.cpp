/*
	Copyright 2011-2012 Xavier Baró

	This file is part of LibSUNAI.

    LibSUNAI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LibSUNAI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ObjectDetection/ObjectDetector.h"

LibSUNAI::CObjectDetector::CObjectDetector(void) {
	
}


LibSUNAI::CObjectDetector::~CObjectDetector(void) {
}


bool LibSUNAI::CObjectDetector::findObjects(CArtDatabase *db,int firstIdx=-1,int lastIdx=-1) {
	Mat image;
	bool ret=false;

	// Check the database pointer
	if(db==NULL) {
		throw CObjectDetectorException("findObjects: NULL Database object.");		
	}

	// Assign default index values
	if(firstIdx==-1) {
		firstIdx=0;
	}
	if(lastIdx==-1) {
		lastIdx=db->getNumImages()-1;
	}

	// Check index values
	if(firstIdx<0 || firstIdx>lastIdx || lastIdx>=db->getNumImages()) {
		throw CObjectDetectorException("findObjects: Wrong image indexes.");
	}

	// Call the detection method for each image
	for(unsigned int i=firstIdx;i<lastIdx;i++) {
		// Gets the i-th image
		if(!db->getImage(i,image)) {
			throw CObjectDetectorException("findObjects: Cannot load the image.");
		}

		// Perform the object detection process
		ret= ret || findObjects(image);
	}
		
	return ret;
}

void LibSUNAI::CObjectDetector::clear(void) {

	// TODO: Create the objects storage and implement related functions
	throw CObjectDetectorException("Not implemented yet");
}

int LibSUNAI::CObjectDetector::getNumObjects(void) {

	// TODO: Create the objects storage and implement related functions
	throw CObjectDetectorException("Not implemented yet");
}

LibSUNAI::CDetectedObject* LibSUNAI::CObjectDetector::getObject(int pos) {

	// Check object position
	if(pos<0 || pos>=getNumObjects()) {
		throw CObjectDetectorException("getObject: Wrong object index.");
	}

	// TODO: Create the objects storage and implement related functions
	throw CObjectDetectorException("Not implemented yet");
}
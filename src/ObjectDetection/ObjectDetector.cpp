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
	// Clear the list of objects
	clearObjects();
}

void LibSUNAI::CObjectDetector::setDataPath(string path) {
	// Copy the new path
	m_dataPath=path;
	if(!m_dataPath.empty()) {        
        if(!m_dataPath.at(m_dataPath.length()-1)=='/' && !m_dataPath.at(m_dataPath.length()-1)=='\\') {            
            m_dataPath+="/";
        }
    }
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

void LibSUNAI::CObjectDetector::clearObjects(void) {
	vector<CDetectedObject*>::iterator objIterator;

	// Remove all objects
	for(objIterator = m_detectedObjects.begin(); objIterator != m_detectedObjects.end(); objIterator++) {
		delete *objIterator;
	}

	// Clear the objects vector
	m_detectedObjects.clear();
}

int LibSUNAI::CObjectDetector::getNumObjects(void) {

	// Returns the size of the object's vector
	return m_detectedObjects.size();
}

LibSUNAI::CDetectedObject* LibSUNAI::CObjectDetector::getObject(int pos) {

	// Check object position
	if(pos<0 || pos>=getNumObjects()) {
		throw CObjectDetectorException("getObject: Wrong object index.");
	}

	// Return a reference to the object
	return m_detectedObjects[pos];
}

bool LibSUNAI::CObjectDetector::findObjects_ViolaJones(Mat image,String cascade,std::vector<Rect> &objects) {
	CascadeClassifier detector;
	string cascadePath;

	// Set the cascade path
	if(!m_dataPath.empty()) {
		cascadePath=m_dataPath + cascade;
	} else {
		cascadePath=cascade;
	}
	
	// Load the detection cascade
	if(!detector.load(m_dataPath + cascade) ) { 
		throw CObjectDetectorException("findObjects_ViolaJones: Cannot load detection cascade <" + cascadePath + ">. Check detector data path.");		
	};

	// Remove old objects
	objects.clear();

	// Apply the detector
	if(image.channels()>1) {
		Mat frame_gray;
		// Create a Grayscale image
		cvtColor(image, frame_gray, CV_BGR2GRAY );
		equalizeHist( frame_gray, frame_gray );

		// Detect on new image
		detector.detectMultiScale(frame_gray, objects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	} else {
		// Detect on input image
		detector.detectMultiScale(image, objects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	}

	return objects.size()>0;
}

void LibSUNAI::CObjectDetector::addObjects(std::vector<Rect> objVector) {
	vector<Rect>::iterator objIterator;

	// Add all objects
	for(objIterator = objVector.begin(); objIterator != objVector.end(); objIterator++) {
		addObject(*objIterator);
	}
}

void LibSUNAI::CObjectDetector::addObject(CDetectedObject* object) {
	// Add a new object directly
	m_detectedObjects.push_back(object);
}

void LibSUNAI::CObjectDetector::addObject(Rect object) {
	// Add a new object represented by a rectangular region
	m_detectedObjects.push_back(new CDetectedObject(object));
}

void LibSUNAI::CObjectDetector::drawObjects(Mat &image) {
	vector<CDetectedObject*>::iterator objIterator;

	// Draw all objects
	for(objIterator = m_detectedObjects.begin(); objIterator != m_detectedObjects.end(); objIterator++) {
		(*objIterator)->drawObject(image);
	}
}
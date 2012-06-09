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
#include "ObjectDetection/FaceObjectDetector.h"


void LibSUNAI::CDetectedFace::drawObject(Mat &image) {
	Rect region;
	Rect partRegion;

	// If no type is specified, use default function
	if(getType()==Undefined) {
		CDetectedObject::drawObject(image);
		return;
	}
	
	// Get a rectangular region of the object
	getObject(region);

	// Draw the face
	switch(getType()) {	
	case Frontal:
		rectangle(image,region,Scalar( 0, 0, 255 ),4);
		break;
	case Profile:
	case ProfileLeft:
	case ProfileRight:
		rectangle(image,region,Scalar( 0, 255, 0 ),4);
		break;
	}

	// Draw available face parts
	if(getFacePart(LeftEye,partRegion)) {
		Point center(region.x + partRegion.x + partRegion.width*0.5, region.y + partRegion.y + partRegion.height*0.5 );
		int radius = cvRound( (partRegion.width + partRegion.height)*0.25 );
		circle(image, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	}
	if(getFacePart(RightEye,partRegion)) {
		Point center(region.x + partRegion.x + partRegion.width*0.5, region.y + partRegion.y + partRegion.height*0.5 );
		int radius = cvRound( (partRegion.width + partRegion.height)*0.25 );
		circle(image, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	}
	if(getFacePart(Nose,partRegion)) {
		Point center(region.x + partRegion.x + partRegion.width*0.5, region.y + partRegion.y + partRegion.height*0.5 );
		int radius = cvRound( (partRegion.width + partRegion.height)*0.25 );
		circle(image, center, radius, Scalar( 255, 255, 0 ), 4, 8, 0 );
	}

	// Draw available face parts
	if(getFacePart(EyesRegion,partRegion)) {	
		partRegion.x+=region.x;
		partRegion.y+=region.y;
		rectangle(image,partRegion,Scalar( 255, 0, 0 ),3);
	}	
	if(getFacePart(Mouth,partRegion)) {		
		partRegion.x+=region.x;
		partRegion.y+=region.y;
		rectangle(image,partRegion,Scalar( 255, 0, 0 ),3);
	}
}

LibSUNAI::CFaceObjectDetector::CFaceObjectDetector(void) : CObjectDetector(), m_usedImplementation(ViolaJones), m_detectionTarget(AllFaceParts)
{
}


LibSUNAI::CFaceObjectDetector::~CFaceObjectDetector(void)
{
}

bool LibSUNAI::CFaceObjectDetector::findObjects(Mat image) {
	bool returnValue;
	
	clearObjects();

	switch(m_usedImplementation) {
	case ViolaJones:
		// Use Viola&Jones for faces
		returnValue=violaJonesFaceDetector(image);
		break;
	}

	return returnValue;
}

void LibSUNAI::CFaceObjectDetector::setDetectionImplementation(ImplementationID implementation) {
	m_usedImplementation=implementation;
}

void LibSUNAI::CFaceObjectDetector::setDetectionTarget(FaceDetectorTarget target) {
	m_detectionTarget=target;
}

bool LibSUNAI::CFaceObjectDetector::violaJonesFaceDetector(Mat image) {	
	vector<Rect> faces;
	CDetectedFace *newFace=NULL;
	bool retValue=false;

	// Apply each detection method
	if(m_detectionTarget==AllBoundingBox || m_detectionTarget==FrontalBoundingBox || 
		m_detectionTarget==AllFaceParts || m_detectionTarget==FrontalFaceParts) {

		// Use frontal faces cascade
		findObjects_ViolaJones(image,"haarcascades/haarcascade_frontalface_alt2.xml",faces);

		// Set the output value
		if(faces.size()>0) {
			retValue=true;
		}

		// Add the objects to the output
		for( int i = 0; i < faces.size(); i++ ) {
			// Create a new face object from region
			newFace=new CDetectedFace(faces[i]);

			// Add extra information
			newFace->setType(Frontal);

			// Locate Face Parts
			if(m_detectionTarget==AllFaceParts || m_detectionTarget==FrontalFaceParts) {
				findFaceParts_ViolaJones(image,newFace);
			}

			// Add all faces to the list of objects
			addObject((CDetectedObject*)newFace);
		}
	}

	if(m_detectionTarget==AllBoundingBox || m_detectionTarget==ProfileBoundingBox || 
		m_detectionTarget==AllFaceParts  || m_detectionTarget==ProfileFaceParts) {

		// Use profile faces cascade
		findObjects_ViolaJones(image,"haarcascades/haarcascade_profileface.xml",faces);

		// Set the output value
		if(faces.size()>0) {
			retValue=true;
		}

		// Add the objects to the output
		for( int i = 0; i < faces.size(); i++ ) {
			// Create a new face object from region
			newFace=new CDetectedFace(faces[i]);

			// Add extra information
			newFace->setType(Profile);

			// Locate Face Parts
			if(m_detectionTarget==AllFaceParts  || m_detectionTarget==ProfileFaceParts) {
				findFaceParts_ViolaJones(image,newFace);
			}

			// Add all faces to the list of objects
			addObject((CDetectedObject*)newFace);
		}
	}

	return retValue;
}

void LibSUNAI::CFaceObjectDetector::findFaceParts_ViolaJones(Mat image,CDetectedFace *face) {
	std::vector<Rect> parts;

	// Cut the region with the face
	Rect region;
	face->getObject(region);
	Mat faceROI = image(region);

	// Initialize the parts
	face->initFaceParts();

	// Search for eyes region    
	parts.clear();
	findObjects_ViolaJones(faceROI,"haarcascades/haarcascade_mcs_eyepair_small.xml",parts);
	if(parts.size()>0) {		
		// Use the first instance. If there are more than one, we can search for most probable region
		face->setFacePart(EyesRegion,parts[0]);
	}

	// Search for Left eye    
	parts.clear();
	findObjects_ViolaJones(faceROI,"haarcascades/haarcascade_mcs_lefteye.xml",parts);
	if(parts.size()>0) {		
		// Use the first instance. If there are more than one, we can search for most probable region
		face->setFacePart(LeftEye,parts[0]);
	}

	// Search for Right eye    
	parts.clear();
	findObjects_ViolaJones(faceROI,"haarcascades/haarcascade_mcs_righteye.xml",parts);
	if(parts.size()>0) {		
		// Use the first instance. If there are more than one, we can search for most probable region
		face->setFacePart(RightEye,parts[0]);
	}

	// Search for Nose    
	parts.clear();
	findObjects_ViolaJones(faceROI,"haarcascades/haarcascade_mcs_nose.xml",parts);
	if(parts.size()>0) {		
		// Use the first instance. If there are more than one, we can search for most probable region
		face->setFacePart(Nose,parts[0]);
	}

	// Search for Mouth    
	parts.clear();
	findObjects_ViolaJones(faceROI,"haarcascades/haarcascade_mcs_mouth.xml",parts);
	if(parts.size()>0) {		
		// Use the first instance. If there are more than one, we can search for most probable region
		face->setFacePart(Mouth,parts[0]);
	}
}



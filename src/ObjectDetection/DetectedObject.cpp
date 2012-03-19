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
#include "ObjectDetection/DetectedObject.h"

LibSUNAI::CDetectedObject::CDetectedObject(void) : m_type(Undefined), m_object(NULL) {

}

LibSUNAI::CDetectedObject::CDetectedObject(const Rect object) {
	// Allocate new memory
	Rect* pObject=new Rect[1];

	// Copy the information
	pObject->x=object.x;
	pObject->y=object.y;
	pObject->width=object.width;
	pObject->height=object.height;

	// Set attributes
	m_type=RectRegion;
	m_object=(void*)pObject;
}

LibSUNAI::CDetectedObject::CDetectedObject(const Ellipse object) {
	// Allocate new memory
	Ellipse* pObject=new Ellipse[1];

	// Copy the information
	pObject->center.x=object.center.x;
	pObject->center.y=object.center.y;
	pObject->axes.width=object.axes.width;
	pObject->axes.height=object.axes.height;
	pObject->rotationAngle=object.rotationAngle;

	// Set attributes
	m_type=EllipRegion;
	m_object=(void*)pObject;
}

LibSUNAI::CDetectedObject::CDetectedObject(const Mat object) {
	// Allocate new memory
	Mat *pObject=new Mat();

	// Copy the information	
	object.copyTo(pObject[0]);

	// Set attributes
	m_type=Image;
	m_object=(void*)pObject;
}

LibSUNAI::CDetectedObject::CDetectedObject(const ObjCorrespondence object) {
	// Allocate new memory
	ObjCorrespondence* pObject=new ObjCorrespondence[1];

	// Copy the information
	pObject->imageId=object.imageId;
	pObject->Homography=NULL;
	if(object.Homography) {
		pObject->Homography=new Mat();
		object.Homography->copyTo(pObject->Homography[0]);
	}

	// Set attributes
	m_type=ObjectCorrespondence;
	m_object=(void*)pObject;
}

LibSUNAI::CDetectedObject::~CDetectedObject(void)
{
	// Destroy object information
	switch(m_type) {
	case Undefined:		
	case RectRegion:		
	case EllipRegion:
	case Image:
		if(m_object) {
			delete[] m_object;
			m_object=NULL;
		}
		break;
	case ObjectCorrespondence:
		if(m_object) {
			ObjCorrespondence *pObject=(ObjCorrespondence*)m_object;
			if(pObject->Homography) {
				delete pObject->Homography;
			}			
			delete[] m_object;
			m_object=NULL;
		}
		break;
	}
}

LibSUNAI::CDetectedObject::DetectedObjectType LibSUNAI::CDetectedObject::getObjectType(void) { 
	return m_type; 
}

void* LibSUNAI::CDetectedObject::getObjectPtr(void) {
	return m_object;
}

void LibSUNAI::CDetectedObject::getObject(Rect& object) {
	// Check the object value
	if(m_object==NULL) {
		throw CObjectDetectorException("getObject: NULL Object.");		
	}

	// Convert object type to required type
	switch(m_type) {
	case Undefined:
		throw CObjectDetectorException("getObject: Object type is undefined.");		
	case RectRegion:
		object.x=((Rect*)m_object)->x;
		object.y=((Rect*)m_object)->y;
		object.width=((Rect*)m_object)->width;
		object.height=((Rect*)m_object)->height;
		break;
	case EllipRegion:
		//object.x=((Ellipse*)m_object)->axes.width;
		// TODO: Get bounding box for the ellipse parameters
		throw CObjectDetectorException("getObject:  NOT YET IMPLEMENTED");		
		break;
	case Image:
		throw CObjectDetectorException("getObject: Cannot convert an Image to a Rect.");
	case ObjectCorrespondence:
		throw CObjectDetectorException("getObject: Cannot convert an ObjectCorrespondence to a Rect.");
	}
}

void LibSUNAI::CDetectedObject::getObject(Ellipse &object) {
	// Check the object value
	if(m_object==NULL) {
		throw CObjectDetectorException("getObject: NULL Object.");		
	}

	// Convert object type to required type
	switch(m_type) {
	case Undefined:
		throw CObjectDetectorException("getObject: Object type is undefined.");		
	case RectRegion:
		// Get a non rotated ellipse
		object.axes.width=((Rect*)m_object)->width;
		object.axes.height=((Rect*)m_object)->height;
		object.center.x=((Rect*)m_object)->x-cvRound(object.axes.width/2.0);
		object.center.y=((Rect*)m_object)->y-cvRound(object.axes.height/2.0);		
		break;
	case EllipRegion:
		// Copy the ellipse information to the output object
		object.center.x=((Ellipse*)m_object)->center.x;
		object.center.y=((Ellipse*)m_object)->center.y;
		object.axes.width=((Ellipse*)m_object)->axes.width;
		object.axes.height=((Ellipse*)m_object)->axes.height;		
		break;
	case Image:
		throw CObjectDetectorException("getObject: Cannot convert an Image to an Ellipse.");
	case ObjectCorrespondence:
		throw CObjectDetectorException("getObject: Cannot convert an ObjectCorrespondence to an Ellipse.");
	}
}

void LibSUNAI::CDetectedObject::getObject(Mat &object,const Mat* sourceImage=NULL) {
	// Check the object value
	if(m_object==NULL) {
		throw CObjectDetectorException("getObject: NULL Object.");		
	}

	// Convert object type to required type
	switch(m_type) {
	case Undefined:
		throw CObjectDetectorException("getObject: Object type is undefined.");		
	case RectRegion:
		if(sourceImage==NULL) {
			throw CObjectDetectorException("getObject: Cannot convert a Rectangular region to an Image. Consider giving a source image.");	
		}
		// Create a Mat header for the resulting region in the source image
		object=Mat(sourceImage[0],(Rect)*((Rect*)m_object));
	case EllipRegion:
		// TODO: Create a mask of the elliptical region and create a new image with the mask/create a rectangular region
		throw CObjectDetectorException("getObject:  NOT YET IMPLEMENTED");		
		break;
	case Image:
		object=((Mat*)m_object)->clone();
		break;
	case ObjectCorrespondence:
		throw CObjectDetectorException("getObject: Cannot convert an ObjectCorrespondence to an Image.");
	}
}

void LibSUNAI::CDetectedObject::getObject(ObjCorrespondence &object) {
	// Check the object value
	if(m_object==NULL) {
		throw CObjectDetectorException("getObject: NULL Object.");		
	}

	// Convert object type to required type
	switch(m_type) {
	case Undefined:
		throw CObjectDetectorException("getObject: Object type is undefined.");		
	case RectRegion:
		throw CObjectDetectorException("getObject: Cannot convert a Rectangle to an ObjectCorrespondence.");
	case EllipRegion:
		throw CObjectDetectorException("getObject: Cannot convert an Ellipse to an ObjectCorrespondence.");
	case Image:
		// TODO: Return the image of the database
		throw CObjectDetectorException("getObject:  NOT YET IMPLEMENTED");	
		break;
	case ObjectCorrespondence:
		
		break;
	}
}
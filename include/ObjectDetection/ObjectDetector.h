/*
	Copyright 2011-2012 Ágata Lapedriza, David Masip, Xavier Baró

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
#ifndef __OBJECT_DETECTOR__H
#define __OBJECT_DETECTOR__H

#include "LibSUNAI.h"
#include "ArtDatabase/ArtDatabase.h"
#include "ObjectDetection/DetectedObject.h"
#include "ObjectDetection/ObjectDetectorException.h"

namespace LibSUNAI {

	/// <summary>
	///  This class defines the interface for object detection methods
	/// </summary>
	class LIBSUNAI_API CObjectDetector {
	private:
		/// <summary>
		/// List of detected objects
		/// </summary>
		std::vector<CDetectedObject*> m_detectedObjects;

		/// <summary>
		/// Root path to detector data
		/// </summary>
		string m_dataPath;

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		CObjectDetector(void);

		/// <summary>
		///  Default descructor
		/// </summary>
		~CObjectDetector(void);
		
		/// <summary>
		///  Perform the object detection over a given image.
		/// </summary>
		/// <param name="image">Mat object containing the image.</param>	
		/// <returns>A true value if the image has objects or false if no object is found.</returns>	
		virtual bool findObjects(Mat image) = 0;

		/// <summary>
		///  Perform the object detection over a given database.
		/// </summary>
		/// <param name="db">ArtDatabase with a set of images.</param>
		/// <param name="firstIdx">Index of the first element of the database to be considered.</param>	
		/// <param name="lastIdx">Index of the last element of the database to be considered.</param>	
		/// <returns>A true value if the image has objects or false if no object is found.</returns>	
		/// <exception cref="CObjectDetectorException">Thrown when position is incorrect or cannot find the image URL.</exception>
		bool findObjects(CArtDatabase *db,int firstIdx,int lastIdx);

		/// <summary>
		///  Returns the number of detected objects
		/// </summary>	
		/// <returns>Number of detected objects.</returns>	
		int getNumObjects(void);

		/// <summary>
		///  Returns the object at the i-th position
		/// </summary>	
		/// <returns>Detected object.</returns>
		/// <exception cref="CObjectDetectorException">Thrown when position is incorrect or cannot find the image URL.</exception>
		CDetectedObject* getObject(int pos);

		/// <summary>
		///  Clear objects storage
		/// </summary>			
		void clearObjects(void);

		/// <summary>
		///  Draw detected objects to the image
		/// </summary>			
		void drawObjects(Mat& image);

		/// <summary>
		///  Set the detector root path
		/// </summary>			
		void setDataPath(string path);

	protected:
		
		/// <summary>
		///  Add all the elements from a vector of detected objects. Allow multiple types of object vectors.
		/// </summary>
		/// <param name="objVector">Vector of element deffining the objects.</param>
		void addObjects(std::vector<Rect> objVector);

		/// <summary>
		///  Add a new object to the list of detected objects. Allow multiple types of objects
		/// </summary>
		/// <param name="object">Element deffining the object.</param>			
		void addObject(CDetectedObject* object);
		void addObject(Rect object);		

		/// <summary>
		///  Perform the object detection over a given image using the Viola & Jones method.
		///  http://opencv.itseez.com/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html
		/// </summary>
		/// <param name="image">Mat object containing the image.</param>	
		/// <param name="cascade">String with the name of the cascade to be used.</param>	
		/// <param name="objects">Vector containing the objects detected. Each object is represented as a Rect object.</param>	
		/// <returns>A true value if the image has objects or false if no object are found.</returns>	
		bool findObjects_ViolaJones(Mat image,String cascade,std::vector<Rect> &objects);
	};
}

#endif //__OBJECT_DETECTOR__H

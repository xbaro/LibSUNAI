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
		virtual bool findObjects(Mat image) = NULL;

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
		void clear(void);
	};
}

#endif //__OBJECT_DETECTOR__H

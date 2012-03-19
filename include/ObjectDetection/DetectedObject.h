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
#ifndef __DETECTED_OBJECT__H
#define __DETECTED_OBJECT__H

#include "LibSUNAI.h"
#include "ObjectDetection/ObjectDetectorException.h"

using namespace cv;

namespace LibSUNAI {
	/// <summary>
	///  This class stores a detected object. 
	/// </summary>
	class LIBSUNAI_API CDetectedObject {
	
	public:
		/// <summary>
		///  Defines an ellipse
		/// </summary>
		typedef struct _Ellipse{
			/// <summary>
			///  Center of the ellipse
			/// </summary>
			Point center;
			/// <summary>
			///  Length of the ellipse axes
			/// </summary>
			Size axes;
			/// <summary>
			///  Rotation angle
			/// </summary>
			double rotationAngle;		
		} Ellipse;

		/// <summary>
		///  Defines a correspondence between objects
		/// </summary>
		typedef struct _ObjCorrespondence{
			/// <summary>
			///  Index of the database image
			/// </summary>
			int imageId;

			/// <summary>
			///  Homography between query object and object in the database
			/// </summary>
			Mat* Homography;
				
			/// <summary>
			///  Vector of matching points
			/// </summary>
		
		} ObjCorrespondence;

		/// <summary>
		///  Defines the types of object regions
		/// </summary>
		typedef enum DetectedObjectType:unsigned short {
			/// <summary>
			///  Undefined type
			/// </summary>
			Undefined,
			/// <summary>
			///  Rectangular region.
			/// </summary>
			RectRegion,
			/// <summary>
			///  Elliptical region.
			/// </summary>
			EllipRegion,
			/// <summary>
			///  Image
			/// </summary>
			Image,
			/// <summary>
			///  Correspondence with another object
			/// </summary>
			ObjectCorrespondence
		};

		/// <summary>
		///  Default constructor region.
		/// </summary>
		CDetectedObject(void);

		/// <summary>
		///  Constructor for a rectangular region.
		/// </summary>
		CDetectedObject(const Rect object);

		/// <summary>
		///  Constructor for an elliptical region.
		/// </summary>
		CDetectedObject(const Ellipse object);

		/// <summary>
		///  Constructor for an image.
		/// </summary>
		CDetectedObject(const Mat object);

		/// <summary>
		///  Constructor for an object correspondence.
		/// </summary>
		CDetectedObject(const ObjCorrespondence object);

		/// <summary>
		///  Default destructor.
		/// </summary>
		~CDetectedObject(void);

		/// <summary>
		///  Gets the type of stored region
		/// </summary>
		/// <returns>Type of information for the object.</returns>	
		DetectedObjectType getObjectType(void);

		/// <summary>
		///  Returns a generic pointer to the object.
		/// </summary>
		/// <returns>A pointer to the object description.</returns>
		void* getObjectPtr(void);

		/// <summary>
		///  Get a rectangular bounding box.
		/// </summary>
		/// <param name="object">Detected object as a rectangular region.</param>	
		void getObject(Rect &object);

		/// <summary>
		///  Get an eliptical region.
		/// </summary>
		/// <param name="object">Detected object as a mask image.</param>	
		void getObject(Ellipse &object);

		/// <summary>
		///  Get a Mask region.
		/// </summary>
		/// <param name="object">Detected object as a mask image.</param>	
		void getObject(Mat &object,const Mat* sourceImage);	
	
		/// <summary>
		///  Get an Object correspondence.
		/// </summary>
		/// <param name="object">Correspondence information between a query image and the database.</param>	
		void getObject(ObjCorrespondence &object);	

		/// <summary>
		///  Assign a rectangular bounding box.
		/// </summary>
		/// <param name="object">Detected object as a rectangular region.</param>	
		void setObject(const Rect object);

		/// <summary>
		///  Assign an eliptical region.
		/// </summary>
		/// <param name="object">Detected object as a mask image.</param>	
		void setObject(const Ellipse &object);

		/// <summary>
		///  Assign a Mask region.
		/// </summary>
		/// <param name="object">Detected object as a mask image.</param>	
		void setObject(const Mat object);	

		/// <summary>
		///  Assign correspondence information.
		/// </summary>
		/// <param name="object">Correspondence information between the query image and the database.</param>	
		void setObject(const ObjCorrespondence object);	

	private:

		/// <summary>
		///  Type of the stored region
		/// </summary>
		DetectedObjectType m_type;

		/// <summary>
		///  Object storage
		/// </summary>
		void* m_object;
	};
}

#endif //__DETECTED_OBJECT__H


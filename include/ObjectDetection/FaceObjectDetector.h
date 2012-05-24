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
#ifndef __FACE_OBJECT_DETECTOR__H
#define __FACE_OBJECT_DETECTOR__H

#include "LibSUNAI.h"
#include "ObjectDetection/ObjectDetector.h"

namespace LibSUNAI {

	/// <summary>
	///  Defines the types of detected face
	/// </summary>
	typedef enum DetectedFaceType:unsigned short {
		/// <summary>
		///  Undefined type
		/// </summary>
		Undefined,
		/// <summary>
		///  Frontal face.
		/// </summary>
		Frontal,
		/// <summary>
		///  Profile face with unknown direction.
		/// </summary>
		Profile,
		/// <summary>
		///  Profile face looking at left side
		/// </summary>
		ProfileLeft,
		/// <summary>
		///  Profile face looking at right side
		/// </summary>
		ProfileRight
	} DetectedFaceType;

	/// <summary>
	///  Defines the parts for a given face
	/// </summary>
	typedef enum DetectedFaceParts:unsigned short {		
		/// <summary>
		///  Eyes region
		/// </summary>
		EyesRegion,
		/// <summary>
		///  Left eye.
		/// </summary>
		LeftEye,
		/// <summary>
		///  Right eye.
		/// </summary>
		RightEye,
		/// <summary>
		///  Left ear.
		/// </summary>
		LeftEar,
		/// <summary>
		///  Right ear.
		/// </summary>
		RightEar,
		/// <summary>
		///  Nose.
		/// </summary>
		Nose,
		/// <summary>
		///  Mouth.
		/// </summary>
		Mouth		
	} DetectedFaceParts;

	/// <summary>
	///  Class to represent a detected face. Is used to add extra information to detected faces
	/// </summary>
	class LIBSUNAI_API CDetectedFace : public CDetectedObject {
	
	public:
		/// <summary>
		///  Default constructor region.
		/// </summary>
		CDetectedFace(void):CDetectedObject(){};

		/// <summary>
		///  Constructor for a rectangular region.
		/// </summary>
		CDetectedFace(const Rect object):CDetectedObject(object){};

		/// <summary>
		///  Constructor for an elliptical region.
		/// </summary>
		CDetectedFace(const Ellipse object):CDetectedObject(object){};

		/// <summary>
		///  Constructor for an image.
		/// </summary>
		CDetectedFace(const Mat object):CDetectedObject(object){};

		/// <summary>
		///  Constructor for an object correspondence.
		/// </summary>
		CDetectedFace(const ObjCorrespondence object):CDetectedObject(object){};

		/// <summary>
		///  Default destructor.
		/// </summary>
		virtual ~CDetectedFace(void) {};

		/// <summary>
		/// Paint the object to the given image
		/// </summary>
		/// <param name="image">Image where object will be painted.</param>	
		virtual void drawObject(Mat &image);
			
		/// <summary>
		/// Set the type of face
		/// </summary>
		/// <param name="type">Type that will be assigned to the face.</param>	
		void setType(DetectedFaceType type) {
			m_faceType=type;
		};

		/// <summary>
		/// Get the type of face
		/// </summary>
		/// <returns>Type of the current object.</returns>	
		DetectedFaceType getType(void) {
			return m_faceType;
		};

		/// <summary>
		/// Set the region of a certain part of the face
		/// </summary>
		/// <param name="part">The part of the face to be assigned.</param>	
	    /// <param name="region">Region for this part.</param>	
		void setFacePart(DetectedFaceParts part,Rect region) {
			m_faceParts[part]=region;
			m_hasFacePart[part]=true;
		};

		/// <summary>
		/// Get the region of a certain part of the face. 
		/// </summary>
		/// <param name="part">The part of the face to be recovered.</param>	
	    /// <param name="region">Region of this part.</param>	
		/// <returns>True if the part was detected and false otherwise.</returns>	
		bool getFacePart(DetectedFaceParts part,Rect &region) {
			region=m_faceParts[part];
			return m_hasFacePart[part];
		};

		/// <summary>
		/// Removes the region of a certain part of the face
		/// </summary>
		/// <param name="part">The part of the face to be assigned.</param>		    
		void removeFacePart(DetectedFaceParts part) {			
			m_hasFacePart[part]=false;
		};

		/// <summary>
		/// Initialize the face parts to non defined
		/// </summary>
		void initFaceParts(void) {
			int numRegs=7;
			for(int i=0;i<numRegs;i++) {
				m_hasFacePart[i]=false;
			}
		};


	protected:
		/// <summary>
		/// Type of detected face
		/// </summary>
		DetectedFaceType m_faceType;

		/// <summary>
		/// Regions for each of the considered face parts. Check attribute m_hasFacePart to know if it is detected or not 
		/// </summary>
		Rect m_faceParts[7];

		/// <summary>
		/// Flags to indicate wich face parts are detected
		/// </summary>
		bool m_hasFacePart[7];
	};

	/// <summary>
	///  Defines the detection options
	/// </summary>
	typedef enum FaceDetectorTarget:unsigned short {
		/// <summary>
		///  Frontal and profile faces. Only bounding box
		/// </summary>
		AllBoundingBox,
		/// <summary>
		///  Frontal faces. Only bounding box
		/// </summary>
		FrontalBoundingBox,
		/// <summary>
		///  Profile faces. Only bounding box
		/// </summary>
		ProfileBoundingBox,
		/// <summary>
		///  Frontal and profile faces. Bounding box and face parts.
		/// </summary>
		AllFaceParts,
		/// <summary>
		///  Frontal faces. Bounding box and face parts.
		/// </summary>
		FrontalFaceParts,
		/// <summary>
		///  Profile faces. Bounding box and face parts.
		/// </summary>
		ProfileFaceParts
	};

	/// <summary>
	///  Enumerator to encode the available implementations
	/// </summary>
	enum ImplementationID:unsigned short {
		/// <summary>
		///  OpenCV Viola & Jones face detector		
		/// </summary>
		ViolaJones
	};

	/// <summary>
	///  This class implements a face detector object.
	/// </summary>
	class LIBSUNAI_API CFaceObjectDetector : public CObjectDetector {
	private:
		/// <summary>
		///  Selected implementation for face detection
		/// </summary>
		ImplementationID m_usedImplementation;

		/// <summary>
		///  Selected the detection target
		/// </summary>
		FaceDetectorTarget m_detectionTarget;

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		CFaceObjectDetector(void);

		/// <summary>
		/// Default destructor
		/// </summary>
		~CFaceObjectDetector(void);

		/// <summary>
		///  Perform the face detection over a given image.
		/// </summary>
		/// <param name="image">Mat object containing the image.</param>	
		/// <returns>A true value if the image has objects or false if no object is found.</returns>	
		virtual bool findObjects(Mat image);

		/// <summary>
		/// Assign the implementation used to detect the faces
		/// </summary>
		/// <param name="implementation">Implementation value.</param>	
		void setDetectionImplementation(ImplementationID implementation);

		/// <summary>
		/// Assign the target for the detector
		/// </summary>
		/// <param name="target">Target value.</param>	
		void setDetectionTarget(FaceDetectorTarget target);


	private: 
		/// <summary>
		///  Use Viola&Jones face detectors
		/// </summary>
		/// <param name="image">Mat object containing the image.</param>	
		/// <returns>A true value if the image has objects or false if no object is found.</returns>	
		bool violaJonesFaceDetector(Mat image);

		/// <summary>
		/// Use Viola&Jones face-part detectors, depending on the type of face
		/// </summary>
		/// <param name="image">Mat object containing the image.</param>
		/// <param name="face">Detected face.</param>
		void findFaceParts_ViolaJones(Mat image,CDetectedFace *face);
	};

}

#endif //__FACE_OBJECT_DETECTOR__H

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

	// <summary>
	///  This class implements a face detector object.
	/// </summary>
	class LIBSUNAI_API CFaceObjectDetector : public CObjectDetector {
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		CFaceObjectDetector(void);

		/// <summary>
		/// Default destructor
		/// </summary>
		~CFaceObjectDetector(void);
	};

}

#endif //__FACE_OBJECT_DETECTOR__H

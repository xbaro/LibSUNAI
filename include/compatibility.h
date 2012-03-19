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
#ifndef __COMPATIBILITY_H
#define __COMPATIBILITY_H

#ifdef _WIN32
	//#include <SDKDDKVer.h>
	#define WIN32_LEAN_AND_MEAN 
	#include <windows.h>

	// Windows definitions
	#ifdef LIBSUNAI_EXPORTS
		#define LIBSUNAI_API __declspec(dllexport)
		#define LIBSUNAI_TEMPLATE
	#else
		#define LIBSUNAI_API __declspec(dllimport)
		#define LIBSUNAI_TEMPLATE extern
	#endif
	
	#define isinf(x) !_finite(x)
	#define isnan(x) _isnan(x)
	
	#include <math.h>
	#define RAND_VALUE() (static_cast<double>(rand())/static_cast<double>(RAND_MAX))

	// CURL includes
	//#include <windows.h>
	//#include <winhttp.h>
	#include <curl/curl.h>

	// OpenCV includes
	#include <opencv2/opencv.hpp>
	#include <opencv2/highgui/highgui.hpp>
	
#else
	//! Linux definitions
	#include <stdlib.h>
	#define LIBSUNAI_API 
	#define LIBSUNAI_TEMPLATE
	#define RAND_VALUE() ((double)random()/(double)RAND_MAX)
#endif

//! Additional definitions
#ifndef PI
	#define PI 3.14159265358979323846
#endif
#ifndef EPS
	#define EPS 0.000001
#endif
#ifndef IO_CLASSNAME_LEN
	#define IO_CLASSNAME_LEN 50
#endif

#endif //__COMPATIBILITY_H

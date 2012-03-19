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
#ifndef __LIBSUNAI_EXCEPTION__H
#define __LIBSUNAI_EXCEPTION__H

#include <exception>
#include <stdexcept>
#include <string>
#include "LibSUNAI.h"

using namespace std;

namespace LibSUNAI {

	/// <summary>
	///  This class implements the custom exceptions for the project
	/// </summary>
	class LIBSUNAI_API CLibSUNAIException : public std::runtime_error
	{
		friend ostream &operator<<(ostream &output, const CLibSUNAIException &obj) 
		{
			output << obj.what();
			return output;
		}
	public:
		CLibSUNAIException(const std::string& what) : std::runtime_error("CLibSUNAIException:" + what) {}
		~CLibSUNAIException(void) throw() {}
	};
}

#endif // __LIBSUNAI_EXCEPTION__H


/*
	Copyright 2011-2012 �gata Lapedriza, David Masip, Xavier Bar�

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

#ifndef __ART_DATABASE_EXCEPTION__H
#define __ART_DATABASE_EXCEPTION__H

#include "LibSUNAIException.h"

namespace LibSUNAI {

	/// <summary>
	///  This class implements the custom exceptions for the Art Database Management
	/// </summary>
	class LIBSUNAI_API CArtDatabaseException : public CLibSUNAIException {
	public:
		CArtDatabaseException(const std::string& what) : CLibSUNAIException("CArtDatabaseException:"+what){}
		~CArtDatabaseException(void) throw() {}	
	};

}

#endif //__ART_DATABASE_EXCEPTION__H

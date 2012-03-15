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
#ifndef __LABEL_DICTIONARY__H
#define __LABEL_DICTIONARY__H

#include <string>
#include <map>

using namespace std;

namespace LibSUNAI {
	/// <summary>
	///  This class implements a dictionar for class labels encoding.
	/// </summary>
	class CLabelDictionary
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		CLabelDictionary(void);

		/// <summary>
		/// Constructor with a content description
		/// </summary>
		/// <param name="description">Description of the content.</param>	
		CLabelDictionary(string description);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~CLabelDictionary(void);

		/// <summary>
		/// Loads the dictionary data from the given file.
		/// </summary>
		/// <param name="fileName">Filename of the file to be loaded.</param>
		/// <param name="rootPathe">Folder to look for the file. By default is the working path.</param>	
		/// <exception cref="CDadesMuseusException">Thrown when the file cannot be read.</exception>
		void loadFile(string fileName,string rootPath="");

		/// <summary>
		/// Gets the label for the given Id.
		/// </summary>
		/// <param name="id">Numerical value used as class id.</param>	
		/// <returns>The string with the description for the class.</returns>
		string getLabelDesc(int labelId);

		/// <summary>
		/// Gets the id for a given label description.
		/// </summary>
		/// <param name="value">Label description.</param>	
		/// <returns>The integer with the id for the class.</returns>
		int getLabelId(string labelDesc);

	private:
		/// <summary>
		/// String with a description for this dictionary
		/// </summary>
		string m_DictionaryDesc;

		/// <summary>
		/// Table with the labels indexed by id
		/// </summary>
		map<int,string> m_IdData;

		/// <summary>
		/// Table with the labels indexed by description
		/// </summary>
		map<string,int> m_DescData;
	};

}

#endif //__LABEL_DICTIONARY__H

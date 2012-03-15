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
#include "ArtDatabase/LabelDictionary.h"
#include "ArtDatabase/ArtDatabaseException.h"
#include <iostream>
#include <fstream>

LibSUNAI::CLabelDictionary::CLabelDictionary(void) : m_DictionaryDesc("Unknown")
{
}

LibSUNAI::CLabelDictionary::CLabelDictionary(string desc) : m_DictionaryDesc(desc)
{
}

LibSUNAI::CLabelDictionary::~CLabelDictionary(void)
{
}

void LibSUNAI::CLabelDictionary::loadFile(string fileName,string rootPath)
{
	ifstream *inputFile=NULL;
	char line[1024];
	string fullFileName;

	// Build the full filename of the dictionary file
	if(rootPath.empty()) {
		fullFileName=fileName;		
	} else {		
		if(rootPath.at(rootPath.length()-1)=='/' || rootPath.at(rootPath.length()-1)=='\\') {
			fullFileName=rootPath+fileName;
		} else {
			fullFileName=rootPath+"/"+fileName;			
		}
	}

	// Open the file
	inputFile=new ifstream(fullFileName);

	// Parse the file
	if(inputFile->is_open())
	{
		int label;
		char desc[1024];
		while (inputFile->good() )
		{
			// Skip comment lines
			inputFile->getline(line,1023);
			if(line[0]=='#')
				continue;
			
			// Parse the input data
			sscanf_s(line,"%d %[^\r\n]",&label, desc,1023);
			
			// Adds the data to the dictionary, indexed by name and label
			string strDesc(desc);
			m_IdData[label]=strDesc;
			m_DescData[strDesc]=label;
		}
		inputFile->close();	
    } else {	
		string msg("Cannot open labels file \"");
		msg+=fullFileName;
		msg+="\".";
		throw CArtDatabaseException(msg);
	}    		
}

string LibSUNAI::CLabelDictionary::getLabelDesc(int labelId)
{	
	// Check for consistency
	if(m_IdData.count(labelId)!=1) {
		string msg("Dictionary error:");
		msg+=" label " + labelId;
		msg+=" not found in dictionary<" + m_DictionaryDesc + ">";
		throw CArtDatabaseException(msg);
	}
		
	return m_IdData[labelId];	
}

int LibSUNAI::CLabelDictionary::getLabelId(string labelDesc)
{
	// Check for consistency
	if(m_DescData.count(labelDesc)!=1) {
		string msg("Dictionary error:");
		msg+=" description " + labelDesc;
		msg+=" not found in dictionary<" + m_DictionaryDesc + ">";
		throw CArtDatabaseException(msg);
	}
		
	return m_DescData[labelDesc];	
}

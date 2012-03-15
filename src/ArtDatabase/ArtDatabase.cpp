#include "ArtDatabase/ArtDatabase.h"
#include "ArtDatabase/ArtDatabaseException.h"
#include <iostream>
#include <fstream>
//#include <windows.h>
//#include <winhttp.h>
#include <stdio.h>
#include <curl/curl.h>
#include <vector>
#include <sys/stat.h>

using namespace std;

ostream& operator<<(ostream& stream,vector<string> ob)
{
	stream << "[";
	for(vector<string>::iterator it=ob.begin();it!=ob.end();it++) {		
		if(it!=ob.begin()) {
			stream << "; ";
		}
		stream << "\"" << *it << "\"";	
	}
	stream << "]";

	return stream;
}

ostream& operator<<(ostream& stream,vector<int> ob)
{
	stream << "[";
	for(vector<int>::iterator it=ob.begin();it!=ob.end();++it) {
		if(it!=ob.begin()) {
			stream << "; ";
		}
		stream << *it;	
	}
	stream << "]";

	return stream;
}

CArtDatabase::CArtDatabase(string dbPath)  : m_Dictionaries(NULL), m_UseLocalStorage(false), m_LocalStoragePath("")
{
	// Set the database path
	m_dbPath=dbPath;

	// Load the dictionaries
	loadDictionaries();

	// Load image Labels
	loadImageLabels();

	// Load image URLs
	loadImageURLs();
}

CArtDatabase::~CArtDatabase(void)
{
}

size_t CArtDatabase::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t realsize = size * nmemb;
    std::vector<char> *vec = (vector<char>*)data;
    for(size_t i = 0;i < realsize;i++)		
        vec->push_back(((char*)ptr)[i]);
    return realsize;
}

bool CArtDatabase::downloadImage(string url,Mat &image) {
	CURL *curl=NULL;
	CURLcode res;
	bool retVal=false;	
	vector<char> buffer;

	// Initialize the cURL library object/structure
	curl = curl_easy_init(); 

	if(curl) {
		// Open the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

#ifdef _DEBUG
		// Show connection status
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // tell us what is happening
#endif //_DEBUF

        // Tell libcurl where to write the image
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &buffer);		
		
        // Get the image from the specified URL
        res = curl_easy_perform(curl);

		// decode memory buffer using OpenCV		
		if(CURLE_OK == res) {			
			image = imdecode(Mat(1, buffer.size(), CV_8UC1, buffer.data()), CV_LOAD_IMAGE_UNCHANGED);
			if(!image.empty())
				retVal=true;	
		}

        // always cleanup
        curl_easy_cleanup(curl);        
		buffer.clear();
  }

  return retVal;
}

void CArtDatabase::loadDictionaries(void) {
	
	// Remove old dictionaries
	if(m_Dictionaries) {
		for(int i=0;i<DADES_MUSEUS_NUMLABELS;i++) {
			delete m_Dictionaries[i];
		}
	}

	// Allocate the new dictionary objects	
	m_Dictionaries=new CLabelDictionary*[DADES_MUSEUS_NUMLABELS];
	
	// Authors dictionary
	m_Dictionaries[LabelID::Author]=new CLabelDictionary("Author names");
	m_Dictionaries[LabelID::Author]->loadFile("authors.dict",m_dbPath);

	// Techniques dictionary
	m_Dictionaries[LabelID::Technique]=new CLabelDictionary("Techniques");
	m_Dictionaries[LabelID::Technique]->loadFile("techniques.dict",m_dbPath);

	// Forms dictionary
	m_Dictionaries[LabelID::Form]=new CLabelDictionary("Forms");
	m_Dictionaries[LabelID::Form]->loadFile("forms.dict",m_dbPath);

	// Types dictionary
	m_Dictionaries[LabelID::Type]=new CLabelDictionary("Types");
	m_Dictionaries[LabelID::Type]->loadFile("types.dict",m_dbPath);

	// Schools dictionary
	m_Dictionaries[LabelID::School]=new CLabelDictionary("Shools");
	m_Dictionaries[LabelID::School]->loadFile("schools.dict",m_dbPath);

	// Time Frames dictionary
	m_Dictionaries[LabelID::TimeFrame]=new CLabelDictionary("Time frames");
	m_Dictionaries[LabelID::TimeFrame]->loadFile("timeFrames.dict",m_dbPath);
}

void CArtDatabase::loadImageLabels(void) {
	ifstream *inputFile=NULL;
	char line[1024];
	string fullFileName;
	string fileName="labels.txt";

	// Build the full filename of the URLs file
	if(m_dbPath.empty()) {
		fullFileName=fileName;		
	} else {		
		if(m_dbPath.at(m_dbPath.length()-1)=='/' || m_dbPath.at(m_dbPath.length()-1)=='\\') {
			fullFileName=m_dbPath+fileName;
		} else {
			fullFileName=m_dbPath+"/"+fileName;			
		}
	}
	
	// Open the file
	inputFile=new ifstream(fullFileName);

	// Parse the file
	if(inputFile->is_open())
	{
		int imageId;
		vector<int> vec(6);
		while (inputFile->good() )
		{
			// Skip comment lines
			inputFile->getline(line,1023);
			if(line[0]=='#')
				continue;
			
			// Parse the input data (ImageID Author Technique Form Type School TimeFrame)
			sscanf_s(line,"%d %d %d %d %d %d %d",&imageId, &vec[LabelID::Author],
				&vec[LabelID::Technique],&vec[LabelID::Form],&vec[LabelID::Type],
				&vec[LabelID::School],&vec[LabelID::TimeFrame]);			
			
			// Add the labels to the database
			m_ImageLabels[imageId]=vec;
		}
		inputFile->close();	
    } else {	
		string msg("Cannot open Labels file \"");
		msg+=fullFileName;
		msg+="\".";
		throw CArtDatabaseException(msg);
	}    		
}

void CArtDatabase::loadImageURLs(void) {
	ifstream *inputFile=NULL;
	char line[1024];
	string fullFileName;
	string fileName="url.txt";

	// Build the full filename of the URLs file
	if(m_dbPath.empty()) {
		fullFileName=fileName;		
	} else {		
		if(m_dbPath.at(m_dbPath.length()-1)=='/' || m_dbPath.at(m_dbPath.length()-1)=='\\') {
			fullFileName=m_dbPath+fileName;
		} else {
			fullFileName=m_dbPath+"/"+fileName;			
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
			
			// Adds the URL to the list
			string strDesc(desc);
			m_ImageURL[label]=strDesc;
		}
		inputFile->close();	
    } else {	
		string msg("Cannot open URLs file \"");
		msg+=fullFileName;
		msg+="\".";
		throw CArtDatabaseException(msg);
	}    		
}

bool CArtDatabase::getImage(int imagePos,Mat &image) {
	bool imageLoaded=false;
	string fullFileName;

	// Check the image position
	if(imagePos<0) {
		throw CArtDatabaseException("getImage:Image position is out of range");
	}
	if(m_ImageURL.count(imagePos)<1) {
		throw CArtDatabaseException("getImage:Image not found.");
	}
	
	// Try to read the image from the local folder
	imageLoaded=false;
	if(m_UseLocalStorage) {
		char fileName[1024];

		// Creat the filename
		sprintf_s(fileName,"Image_%07d.jpg",imagePos,1023);

		// Create the full filename
		if(m_LocalStoragePath.empty()) {
			fullFileName=string(fileName);
		} else {		
			if(m_LocalStoragePath.at(m_LocalStoragePath.length()-1)=='/' || m_LocalStoragePath.at(m_LocalStoragePath.length()-1)=='\\') {
				fullFileName=m_LocalStoragePath+string(fileName);
			} else {
				fullFileName=m_LocalStoragePath+"/"+string(fileName);			
			}
		}
		// Read the image from the local storage path
		image=imread(fullFileName);
		if(!image.empty()) {
			imageLoaded=true;
		}
	}
	

	// Download the image from internet
	if(!imageLoaded) {
		// Download the image
		if(downloadImage(m_ImageURL[imagePos],image)) {
			imageLoaded=true;			
		}
		// Save the image
		if(imageLoaded && m_UseLocalStorage) {
			imwrite(fullFileName,image);
		}
	}

	return imageLoaded;
}

int CArtDatabase::getNumImages(void) {
	return m_ImageURL.size();	
}

Mat CArtDatabase::getLabelsMatrix(Mat *var_idx,Mat *sample_idx) {
	int numImages=0;
	int dimension=0;	

	// Get the output dimension
	if(var_idx) {
		if(var_idx->size().width>1 && var_idx->size().height>1)
			throw CArtDatabaseException("getLabelsMatrix:var_idx must be a row or column vector");
		dimension=max(var_idx->size().width,var_idx->size().height);
	} else {
		dimension=DADES_MUSEUS_NUMLABELS;
	}

	// Get the output dimension
	if(sample_idx) {
		if(sample_idx->size().width>1 && sample_idx->size().height>1)
			throw CArtDatabaseException("getLabelsMatrix:sample_idx must be a row or column vector");
		numImages=max(sample_idx->size().width,sample_idx->size().height);
	} else {
		numImages=getNumImages();
	}

	// Create the output matrix
	Mat labels=Mat(numImages,dimension,CV_32S);
	vector<int> imageLabels;
	int imgIdx,labIdx;
	for(int i=0;i<numImages;i++) {
		// Get the image position		
		if(sample_idx) {
			imgIdx=sample_idx->at<int>(i);
			getImageLabels(imgIdx,imageLabels);
		} else {
			getImageLabels(i+1,imageLabels);
		}

		// Add selected labels
		for(int l=0;l<dimension;l++) {
			// Get the label position
			labIdx=l;
			if(var_idx) {
				labIdx=var_idx->at<int>(l);
			}
			// Fill the position
			labels.at<int>(i,l)=imageLabels[labIdx];
		}
	}

	return labels;
}

void CArtDatabase::getImageLabels(int imagePos,vector<int> &labels) {
	// Check the image position
	if(imagePos<1) {
		throw CArtDatabaseException("getImageLabels:Image position is out of range");
	}
	if(m_ImageLabels.count(imagePos)<1) {
		throw CArtDatabaseException("getImageLabels:Image not found.");
	}	
	
	// Try to read the image from the local folder	
	labels=m_ImageLabels[imagePos];	
}

void CArtDatabase::getImageLabels(int imagePos,Mat &labels) {	
	
	// Obtain the labels vector
	vector<int> vec;
	getImageLabels(imagePos,vec);
	
	// Create the Mat object
	// labels=Mat(6,1,CV_32S,vec.data()); // NO FUNCIONA
	//Mat(vec.size(),1,CV_32S,vec.data());
	labels=Mat(vec.size(),1,CV_32S);	
	int i=0;
	for(vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		labels.at<int>(i)=*it;
		i++;
	}
}

string CArtDatabase::getLabelName(LabelID labelID) {

	string desc;

	switch(labelID) {
	case LabelID::Author:
		desc="Author";
		break;
	case LabelID::Form:
		desc="Form";
		break;
	case LabelID::School:
		desc="School";
		break;
	case LabelID::Technique:
		desc="Technique";
		break;
	case LabelID::TimeFrame:
		desc="TimeFrame";
		break;
	case LabelID::Type:
		desc="Type";
		break;
	default:
		desc="<Undefined>";
		break;
	}

	return desc;
}

CArtDatabase::LabelID CArtDatabase::getLabelID(const unsigned short labelPos) {
	if(labelPos<0 || labelPos>DADES_MUSEUS_NUMLABELS) {
		throw CArtDatabaseException("getLabelID: position out of range.");
	}
	return (LabelID)labelPos;
}

void CArtDatabase::getProblemDescription(string version,int &numLabels,vector<string> &labelName) {	
	version=DADES_MUSEUS_VERSION;
	numLabels=DADES_MUSEUS_NUMLABELS;
	labelName.clear();	
	for(unsigned short i=0;i<numLabels;i++) {		
		labelName.push_back(getLabelName(getLabelID(i)));
	}
}

string CArtDatabase::getLabelTranslation(unsigned int labelVal,LabelID labelID) {
	return m_Dictionaries[labelID]->getLabelDesc(labelVal);
}

void CArtDatabase::getLabelsTranslation(vector<int> labels,vector<string> &translation,vector<LabelID> labIdx) {
	// Check parameter dimensions
	if(labels.size()!=labIdx.size()) {
		throw CArtDatabaseException("getLabelsTranslation: dimensions missmatch.");
	}

	// Translate the vector
	for(unsigned int i=0;i<labels.size();i++) {			
		translation.push_back(getLabelTranslation(labels[i],labIdx[i]));
	}
}

void CArtDatabase::getLabelsTranslation(Mat labels,vector<string> &translation,vector<LabelID> labIdx) {
	// Check parameter dimensions
	int matSize=labels.size().height*labels.size().width;
	if(matSize!=labIdx.size()) {
		throw CArtDatabaseException("getLabelsTranslation: dimensions missmatch.");
	}

	// Translate the vector
	for(unsigned int i=0;i<labIdx.size();i++) {			
		translation.push_back(getLabelTranslation(labels.at<int>(i),labIdx[i]));
	}
}

void CArtDatabase::getLabelsTranslation(vector<int> labels,vector<string> &translation,LabelID labIdx) {	
	// Translate the vector
	for(unsigned int i=0;i<labels.size();i++) {			
		translation.push_back(getLabelTranslation(labels[i],labIdx));
	}
}

void CArtDatabase::getLabelsTranslation(Mat labels,vector<string> &translation,LabelID labIdx) {
	// Check parameter dimensions
	if(labels.size().width>1 && labels.size().height>1) {
		throw CArtDatabaseException("getLabelsTranslation: dimensions missmatch.");
	}

	// Translate the vector
	int maxSize=max(labels.size().height,labels.size().width);
	for(unsigned int i=0;i<maxSize;i++) {		
		translation.push_back(getLabelTranslation(labels.at<int>(i),labIdx));
	}
}

void CArtDatabase::getLabelsTranslation(vector<int> labels,vector<string> &translation) {
	// Check parameter dimensions
	if(labels.size()!=DADES_MUSEUS_NUMLABELS) {
		throw CArtDatabaseException("getLabelsTranslation: dimensions missmatch.");
	}

	// Create the vector with all labels
	vector<LabelID> labIdx;
	for(unsigned short i=0;i<DADES_MUSEUS_NUMLABELS;i++) {
		labIdx.push_back(getLabelID(i));
	}

	// Delegate the exectution
	getLabelsTranslation(labels,translation,labIdx);
}

void CArtDatabase::getLabelsTranslation(Mat labels,vector<string> &translation) {
	// Check parameter dimensions
	int maxSize=max(labels.size().height,labels.size().width);
	if(maxSize!=DADES_MUSEUS_NUMLABELS) {
		throw CArtDatabaseException("getLabelsTranslation: dimensions missmatch.");
	}

	// Create the vector with all labels
	vector<LabelID> labIdx;
	for(unsigned short i=0;i<DADES_MUSEUS_NUMLABELS;i++) {
		labIdx.push_back(getLabelID(i));
	}

	// Delegate the exectution
	getLabelsTranslation(labels,translation,labIdx);
}

bool CArtDatabase::pathExist(string path){ 
	struct stat buf;
	int result;

	// Get information for the path
	result=stat(path.c_str(),&buf);

	// Check the response
	if(result) {		
		switch (errno) {
			case ENOENT:
				return false;           
			default:
				throw CArtDatabaseException("pathExist: Undefined exception.");
		}
	}

	return true;
}

void CArtDatabase::setLocalStorage(bool value) {
	m_UseLocalStorage=value;
}

void CArtDatabase::setLocalStoragePath(string path) {
	
	if(!pathExist(path)) {
		throw CArtDatabaseException("setLocalStoragePath: given path does not exist.");
	}
	
	// Store the path
	m_LocalStoragePath=path;
}
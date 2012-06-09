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
#include <LibSUNAI.h>
#include <ArtDatabase/ArtDatabase.h>
#include <ObjectDetection/FaceObjectDetector.h>
#include <iostream>

using namespace std;
using namespace LibSUNAI;

#define WINDOW_NAME "DBAnalyzer (Press ESC to exit)"

// Global variables
CArtDatabase* g_artDB=NULL;
CFaceObjectDetector g_faceDetector;
int g_lastId;
string g_outputPrefix;

//////////////////////////////////////////////////////////////////////////////////
// Parameters
//////////////////////////////////////////////////////////////////////////////////
// File with a list of images (if NULL, the ArtDatabase is used).
const char *g_inputFile=NULL;
//const char *g_inputFile="D:\\temp\\testAuthor.txt";

// Output path for results. It MUST exists. (if NULL, images are not stored). Use paths with final slash, eg. "D:\\temp\\output\\")
const char *g_outputPath=NULL; 
//const char *g_outputPath="D:\\temp\\output\\";

// Enables/Disables interactive mode. True enables and false disables.
bool g_interactive=false;
//////////////////////////////////////////////////////////////////////////////////

// Parse input parameters
void parseInputPars(int argc, char* argv[]) {
	if(argc==1) {
		cout << argv[0] << "[options]" << endl;
		cout << "\t-i \tActivates the Interactive mode." << endl;
		cout << "\t-in <file> \tProvide a file with source image paths." << endl;
		cout << "\t-out <dir> \tProvide a directory to store the results." << endl;
	}
	for(int i=0;i<argc;i++) {
		if(strcmp(argv[i],"-i")==0) {
			std::cout << "Interactive mode ON" << endl;
			g_interactive=true;
		}
		if(strcmp(argv[i],"-in")==0) {
			if(argc>i) {
				g_inputFile=argv[i+1];
				std::cout << "Images readed from file: " << g_inputFile << endl;
			} else {
				cout << "Not enough parameters" << endl;
			}
		}

		if(strcmp(argv[i],"-out")==0) {
			if(argc>i) {
				g_outputPath=argv[i+1];
				std::cout << "Results stored to path: " << g_outputPath << endl;
				cout << " IMPORTANT: This path must exist and be readable." << endl;
			} else {
				cout << "Not enough parameters" << endl;
			}
		}
	}
}

// Detection process
bool doDetection(Mat currentImage) {
	char fullFileName[1024];

	if(g_faceDetector.findObjects(currentImage)) {				
		cout << "\t" << g_faceDetector.getNumObjects() << " faces found!" << endl;
		g_faceDetector.drawObjects(currentImage);
	} else {
		cout << endl << "\tNo Face Found" << endl;	
	}
	if(g_outputPath!=NULL) {
		string outpath=string(g_outputPath);
		if(outpath.at(outpath.length()-1)!='/' && outpath.at(outpath.length()-1)!='\\') {
			outpath+="/";
		}
		if(g_outputPrefix.empty()) {
			sprintf(fullFileName,"%s_%05d.jpg",outpath.c_str(),"Image",++g_lastId);
		} else {
			sprintf(fullFileName,"%s%s_%05d.jpg",outpath.c_str(),g_outputPrefix.c_str(),++g_lastId);
		}
		imwrite(fullFileName,currentImage);
	}
	if(g_interactive) {
		imshow(WINDOW_NAME,currentImage);
		if(cvWaitKey(0)==27) {
			return false;
		}
	} else {
		if(cvWaitKey(3)==27) {
			return false;
		}
	}

	return true;
}

// Main programm structure
int main(int argc, char* argv[])
{	
	ifstream *fin=NULL;

	// Parse the input parameters
	parseInputPars(argc,argv);

	try {
		if(g_inputFile) {
			// Analyze from a file of paths
			// Show versions information
			cout << "======================================================" << endl;
			cout << "LibSUNAI version    :" << LibSUNAI::getVersion() << endl;
			cout << "Input file          :" << g_inputFile << endl;
			cout << "======================================================" << endl;
			fin=new ifstream(g_inputFile);
			if(fin==NULL) {
				cout << "ERROR: Cannot open input images file " << endl;
				return -1;
			}
		} else {
			// Create the ArtDatabase object providing the path
			g_artDB=new CArtDatabase("../data/artdb/");
			g_artDB->setLocalStoragePath("../data/artdb/local");
			g_artDB->setLocalStorage(true);

			// Show versions information
			cout << "======================================================" << endl;
			cout << "LibSUNAI version    :" << LibSUNAI::getVersion() << endl;
			cout << "ArtDatabase version :" << g_artDB->getArtDatabaseVersion() << endl;
			cout << "======================================================" << endl;
		}
		
		// Prepare the face detecion object
		g_faceDetector.setDataPath("../data/detector/");			
		g_faceDetector.setDetectionTarget(AllBoundingBox); 

		// Create output path
		// TODO: Create the output directory if it does not exists

		// Create output window
		cvNamedWindow(WINDOW_NAME,0);

		// Analyze each object in the database
		g_lastId=0;
		Mat currentImage;
		if(fin) {
			int numImages=0;
			int idx=0;
			// Obtain the filename
			size_t pos;
			g_outputPrefix=string(g_inputFile);
			// Remove extension
			pos=g_outputPrefix.rfind('.');
			g_outputPrefix=g_outputPrefix.substr(0,pos);
			// Remove path
			replace(g_outputPrefix.begin(),g_outputPrefix.end(), '\\', '/'); 
			pos=g_outputPrefix.rfind('/');
			g_outputPrefix=g_outputPrefix.substr(pos+1);

			// Get the number of images
			numImages=0;
			while(!fin->eof()) {
				char line[1024];
				fin->getline(line,1024);
				numImages++;
			}
			fin->clear();
			fin->seekg(0);

			// Analyze all images
			idx=1;
			while(!fin->eof()) {
				char line[1024];
				fin->getline(line,1024);
				cout << "Image " << idx++ << " of " << numImages << " from the file " << g_inputFile <<". Press ESC to stop";
				currentImage=imread(line,1);
				if(!currentImage.empty()) {				
					if(!doDetection(currentImage)) {
						break;
					}
				}  else {
					cout << "Cannot read the image [" << line << "]";
				}
			}
		} else {
			g_outputPrefix="ArtDatabase";
			for(int i=1;i<g_artDB->getNumImages();i++) {
				cout << "Image " << i << " of " << g_artDB->getNumImages() << " from the database. Press ESC to stop";
				if(g_artDB->getImage(i,currentImage)) {				
					if(!doDetection(currentImage)) {
						break;
					}
				}  else {
					cout << "Cannot read the image";
				}
			}
		}

		cvDestroyAllWindows();
		
		delete g_artDB;
		if(fin) {
			fin->close();
			delete fin;
		}
	} catch(CArtDatabaseException const& dmEx) {
		cout << "An exception occurred:" << endl << dmEx << endl;
	} catch(CLibSUNAIException const& pmEx) {
		cout << "An exception occurred:" << endl << pmEx << endl;
	}

	cout << "\nPres key to continue..." << endl;
	getchar();

	return 0;
}


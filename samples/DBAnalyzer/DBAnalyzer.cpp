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

// Main programm structure
int main(int argc, char* argv[])
{	
	try {
		// Create the ArtDatabase object providing the path
		CArtDatabase* db=new CArtDatabase("../data/artdb/");
		db->setLocalStoragePath("../data/artdb/local");
		db->setLocalStorage(true);

		// Show versions information
		cout << "======================================================" << endl;
		cout << "LibSUNAI version    :" << LibSUNAI::getVersion() << endl;
		cout << "ArtDatabase version :" << db->getArtDatabaseVersion() << endl;
		cout << "======================================================" << endl;

		// Apply the face detector to a test image
		
		// Create a face detecion object
		CFaceObjectDetector faceDet;
		faceDet.setDataPath("../data/detector/");			
		//faceDet.setDetectionTarget(AllFaceParts); //[Default] Frontal & Profile (bounding box and parts)
		//faceDet.setDetectionTarget(FrontalFaceParts); //Frontal (bounding box and parts)

		// Analyze each object in the database
		Mat currentImage;
		for(int i=7;i<db->getNumImages();i++) {
			if(db->getImage(i,currentImage)) {				
				if(faceDet.findObjects(currentImage)) {				
					cout << faceDet.getNumObjects() << " faces found!" << endl;
					faceDet.drawObjects(currentImage);
				} else {
					cout << "No Face Found" << endl;	
				}
				imshow(WINDOW_NAME,currentImage);
				if(cvWaitKey(0)==27) {
					break;
				}
			}  else {
				cout << "Cannot reat the image" << endl;
			}
		}

		cvDestroyAllWindows();
		
		delete db;
		
	} catch(CArtDatabaseException const& dmEx) {
		cout << "An exception occurred:" << endl << dmEx << endl;
	} catch(CLibSUNAIException const& pmEx) {
		cout << "An exception occurred:" << endl << pmEx << endl;
	}

	cout << "\nPres key to continue..." << endl;
	getchar();

	return 0;
}


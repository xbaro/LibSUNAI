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
#include <iostream>

using namespace std;
using namespace LibSUNAI;

#define WINDOW_NAME "DBExplorer (Press ESC to exit)"

typedef struct {
	CArtDatabase* db;
	Mat currentImage;
	int idx;
} tUserData;

// Show an image and all its information
void showImage(tUserData* data) {
	
	// Point to the data	
	CArtDatabase* db=(CArtDatabase*)data->db;

	try {
		// Obtain and show an image	
		if(db->getImage(data->idx,data->currentImage)) {		
			imshow(WINDOW_NAME,data->currentImage);		
		} else {
			cerr << "Cannot reat the image" << endl;
		}

		// Show the 

	/*			
		// Get the labels and descriptions in OpenCV Mat 
		Mat labelsMat;
		vector<string> labelsDescMat;
		db.getImageLabels(1,labelsMat);
		db.getLabelsTranslation(labelsMat,labelsDescMat);

		cout << "---------------------------------------------" << endl;
		cout << "Print the labels obtained as an OpenCV Matrix" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsMat << endl;	
		cout << "---------------------------------------------" << endl;
		cout << labelsDescMat << endl;
		cout << "---------------------------------------------" << endl <<endl;

		// Repeat using standard vector type
		vector<int> labelsVec;
		vector<string> labelsDescVec;
		db.getImageLabels(1,labelsVec);
		db.getLabelsTranslation(labelsVec,labelsDescVec);
				
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels obtained as a Vector" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsVec << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsDescVec << endl;
		cout << "---------------------------------------------" << endl <<endl;

		// Retrieve the matrix of all labels of the fourth first images (1,2,3, and 4)
		Mat sample_idx=(Mat_<int>(1,4) << 1, 2, 3, 4); 
		Mat labelsMatrix=db.getLabelsMatrix(NULL,&sample_idx);
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels for the fourth first images" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsMatrix << endl;
		cout << "---------------------------------------------" << endl;

		// Select a subset of labels (0, 3, and 5 -> Author, Type, and TimeFrame 
		Mat var_idx=(Mat_<int>(1,3) << 0, 3, 5); 
		labelsMatrix=db.getLabelsMatrix(&var_idx,&sample_idx);
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels for the fourth first images" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsMatrix << endl;
		cout << "---------------------------------------------" << endl;
		*/
	} catch(CArtDatabaseException const& dmEx) {
		cout << "An exception occurred:" << endl << dmEx << endl;
	} catch(CLibSUNAIException const& pmEx) {
		cout << "An exception occurred:" << endl << pmEx << endl;
	}
}

// Define the callbacks
void trackBarCB(int value,void* data) {
	tUserData* userData = (tUserData*)data;
	userData->idx=value;
	showImage(userData);
}
void buttonCB_Prev(int state,void* data) {
	tUserData* userData = (tUserData*)data;
	if(userData->idx>0) {
		userData->idx--;
	}
	showImage(userData);
}
void buttonCB_Next(int state,void* data) {
	tUserData* userData = (tUserData*)data;
	if(userData->idx<(userData->db->getNumImages()-1)) {
		userData->idx++;
	}
	showImage(userData);
}

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

		// Create the data structure
		tUserData userData;
		userData.db=db;		

		// Create the interface
		cvNamedWindow(WINDOW_NAME,CV_WINDOW_KEEPRATIO|CV_GUI_EXPANDED);
		cv::createTrackbar("Image ", WINDOW_NAME, NULL, db->getNumImages(),trackBarCB,(void*)&userData);		
		//cv::createButton("<--",buttonCB_Prev, (void*)&userData,0,false);
		//cv::CreateButton("-->",buttonCB_Next, (void*)&userData,0,false);		
		//displayStatusBar(WINDOW_NAME, "Hello World", 5000); 
		//cv::displayOverlay(WINDOW_NAME, "Test text",3000);

		if(userData.db->getImage(9,userData.currentImage)) {		
			imshow(WINDOW_NAME,userData.currentImage);		
		} else {
			cout << "Cannot reat the image" << endl;
		}

		while(cvWaitKey(33) != 27);

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


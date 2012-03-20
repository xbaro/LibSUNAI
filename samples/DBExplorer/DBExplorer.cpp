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

int main(int argc, char* argv[])
{	
	try {
		// Create the DadesMuseus object providing the path
		CArtDatabase dm("../data/artdb/");
		dm.setLocalStoragePath("../data/artdb/local");
		dm.setLocalStorage(true);

		// Show versions information
		cout << "======================================================" << endl;
		cout << "LibSUNAI version    :" << LibSUNAI::getVersion() << endl;
		cout << "ArtDatabase version :" << dm.getArtDatabaseVersion() << endl;
		cout << "======================================================" << endl;

		// Obtain and show an image
		Mat image;
		if(dm.getImage(1,image)) {
			namedWindow("IMAGE");
			imshow("IMAGE",image);
			waitKey(0);
			destroyWindow("IMAGE");
		} else {
			cout << "Cannot reat the image" << endl;
		}
				
		// Get the labels and descriptions in OpenCV Mat 
		Mat labelsMat;
		vector<string> labelsDescMat;
		dm.getImageLabels(1,labelsMat);
		dm.getLabelsTranslation(labelsMat,labelsDescMat);

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
		dm.getImageLabels(1,labelsVec);
		dm.getLabelsTranslation(labelsVec,labelsDescVec);
				
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels obtained as a Vector" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsVec << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsDescVec << endl;
		cout << "---------------------------------------------" << endl <<endl;

		// Retrieve the matrix of all labels of the fourth first images (1,2,3, and 4)
		Mat sample_idx=(Mat_<int>(1,4) << 1, 2, 3, 4); 
		Mat labelsMatrix=dm.getLabelsMatrix(NULL,&sample_idx);
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels for the fourth first images" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsMatrix << endl;
		cout << "---------------------------------------------" << endl;

		// Select a subset of labels (0, 3, and 5 -> Author, Type, and TimeFrame 
		Mat var_idx=(Mat_<int>(1,3) << 0, 3, 5); 
		labelsMatrix=dm.getLabelsMatrix(&var_idx,&sample_idx);
		cout << "---------------------------------------------" << endl;
		cout << "Print the labels for the fourth first images" << endl;
		cout << "---------------------------------------------" << endl;
		cout << labelsMatrix << endl;
		cout << "---------------------------------------------" << endl;
		
	} catch(CArtDatabaseException const& dmEx) {
		cout << "An exception occurred:" << endl << dmEx << endl;
	} catch(CLibSUNAIException const& pmEx) {
		cout << "An exception occurred:" << endl << pmEx << endl;
	}

	cout << "\nPres key to continue..." << endl;
	getchar();
	return 0;
}


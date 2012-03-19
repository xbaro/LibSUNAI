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
#ifndef __ART_DATABASE__H
#define __ART_DATABASE__H

#include <vector>
#include "LibSUNAI.h"
#include "ArtDatabase/ArtDatabase.h"
#include "ArtDatabase/ArtDatabaseException.h"

using namespace cv;
using namespace std;

namespace LibSUNAI {

	LIBSUNAI_API const char* ART_DATABASE_VERSION="1.0";
	LIBSUNAI_API const int ART_DATABASE_NUMLABELS=6;

	/// <summary>
	///  This class stores all the images database
	/// </summary>
	class LIBSUNAI_API CArtDatabase {	
	public:
		/// <summary>
		///  Implements a vertor of integers
		/// </summary>
		typedef vector<int> intVector;

		/// <summary>
		///  Defines the list of labels
		/// </summary>
		typedef enum LabelID:unsigned short {
			/// <summary>
			///  Name of the author
			/// </summary>
			Author,
			/// <summary>
			///  Technique used to create the item (Bronze, Marble, Fresco, Oil in copper,...)
			/// </summary>
			Technique,
			/// <summary>
			///  Form of the art. (ceramics, painting, sculpture, ...)
			/// </summary>
			Form,
			/// <summary>
			///  Type of art. (Religions, landscape, ...)
			/// </summary>
			Type,
			/// <summary>
			///  School of the artist.
			/// </summary>
			School,
			/// <summary>
			///  Períod of the art object. There is no intersection between períods, and they are labeled respecting the order.
			/// </summary>
			TimeFrame
		};

	private:	
		/// <summary>
		///  Callback function used to write download data (cURL library)
		/// </summary>	
		static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);

		/// <summary>
		///  This method allows to download an image from an internet URL
		/// </summary>	
		/// <param name="url">Full qualified URL to the image.</param>	
		/// <param name="image">Header used to point the downloaded image.</param>	
		/// <returns>A true value if the image has been correctly downloaded of false if an error occurred.</returns>	
		bool downloadImage(string url,Mat &image);

		/// <summary>
		///  This method loads all the database dictionaries
		/// </summary>		
		void loadDictionaries(void);

		/// <summary>
		///  This method loads the image labels from file
		/// </summary>			
		void loadImageLabels(void);

		/// <summary>
		///  This method loads the URL where each image is stored.
		/// </summary>			
		void loadImageURLs(void);	

		/// <summary>
		///  This method allows to check if a certain path exists or not.
		/// </summary>	
		/// <param name="path">Path to a file or folder</param>	
		/// <returns>A true value if the path exists or false otherwise.</returns>	
		bool pathExist(string path);

	public:
		/// <summary>
		/// Creates a new object DadesMuseus in order to access de museums image databases
		/// </summary>
		/// <param name="dbPath">Folder where de museums image database is stored.</param>	
		CArtDatabase(string dbPath);	

		/// <summary>
		/// Default descructor
		/// </summary>
		~CArtDatabase(void);

		/// <summary>
		/// Returns the number of images in the database
		/// </summary>
		/// <returns>Number of stored images.</returns>	
		int getNumImages(void);

		/// <summary>
		/// The getImage method returns a certain image of the DataBase. In the case of this image is not
		/// in the images directory, the image is downloaded from internet. Images are numered from 1.
		/// </summary>
		/// <param name="imagePos">Image identifier.</param>
		/// <param name="image">Mat object containing the image.</param>	
		/// <returns>A true value if the image has been correctly obtained of false if an error occurred.</returns>
		/// <exception cref="CArtDatabaseException">Thrown when position is incorrect or cannot find the image URL.</exception>
		bool getImage(int imagePos,Mat &image);	

		/// <summary>
		/// The getImageLabels method returns the labels vector of a certain image of the DataBase.
		/// </summary>
		/// <param name="imagePos">Image identifier.</param>
		/// <param name="labels">Vector with the labels of the image.</param>		
		/// <exception cref="CArtDatabaseException">Thrown when position is incorrect or cannot find the labels.</exception>
		void getImageLabels(int imagePos,Mat &image);	
		void getImageLabels(int imagePos,vector<int> &labels);

		/// <summary>
		/// The getLabelsMatrix method returns a matrix of numSamples x numLabels, with the labels of all images
		/// </summary>
		/// <param name="var_idx">Matrix with the 0-based indexs for the labels of interest. By default is NULL, what means all labels.</param>
		/// <param name="sample_idx">Matrix with the 1-based indexs for the images of interest. By default is NULL, what means all images.</param></param>			
		/// <returns>Matrix with the labels of interest for the images of interest.</returns>
		Mat getLabelsMatrix(Mat *var_idx=NULL,Mat *sample_idx=NULL);

		/// <summary>
		/// The getLabelsTranslation method convert a vector of labels to a vector of their correspondant description
		/// </summary>
		/// <param name="labels">A vector of labels.</param>
		/// <param name="translation">Vector with label descriptions.</param>		
		/// <param name="labIdx">Vector with 0-based label positions. It allow to work with a subset of labels,
		/// and the vector must have the same length as labIdx. By default all labels are used. If labIdx is an int, 
		/// only one label is considered and all the values in labels are translated using this label.</param>
		/// <exception cref="CArtDatabaseException">Thrown when input vectors dimensions are incorrect.</exception>
		void getLabelsTranslation(vector<int> labels,vector<string> &translation,vector<LabelID> labIdx);
		void getLabelsTranslation(Mat labels,vector<string> &translation,vector<LabelID> labIdx);
		void getLabelsTranslation(vector<int> labels,vector<string> &translation,LabelID labIdx);
		void getLabelsTranslation(Mat labels,vector<string> &translation,LabelID labIdx);
		void getLabelsTranslation(vector<int> labels,vector<string> &translation);
		void getLabelsTranslation(Mat labels,vector<string> &translation);

		/// <summary>
		/// This method returns the description name of the given label identifier
		/// </summary>
		/// <param name="labelID">Identifier of the label.</param>
		/// <returns>Label description name.</returns>		
		string getLabelName(LabelID labelID);

		/// <summary>
		/// This method returns the translation of a given label value
		/// </summary>
		/// <param name="labelVal">Value of the label.</param>
		/// <param name="labelID">Identifier of the label.</param>	
		/// <returns>Label description name.</returns>		
		string getLabelTranslation(unsigned int labelVal,LabelID labelID);

		/// <summary>
		/// The getProblemDescription method returns information about the DataBase.
		/// </summary>
		/// <param name="version">Database reader version.</param>
		/// <param name="numLabels">Number of labels.</param>
		/// <param name="labelName">Descriptive name of the labels.</param>		
		void getProblemDescription(string version,int &numLabels,vector<string> &labelName);

		/// <summary>
		/// This method returns the label identifier for a ginven position in the vector.
		/// </summary>	
		/// <param name="labelPos">Label position.</param>	
		/// <returns>Identifier of the label.</returns>
		/// <exception cref="CArtDatabaseException">Thrown when label position is incorrect.</exception>
		LabelID getLabelID(const unsigned short labelPos);

		/// <summary>
		///  This method enables or disables de local storage ability
		/// </summary>			
		/// <param name="value">Boolean value to enable (true) or disable(false) the local storage. In case that
		/// local storage is disabled, all images are downloaded from internet.
		/// </param>	
		/// <see>setLocalStoragePath</see>
		void setLocalStorage(bool value);
	
		/// <summary>
		///  This method allows to change the path where images are sotred when local storage is enabled.
		/// </summary>			
		/// <param name="path">Path where image are searched and stored when local storage is enabled. This path must exist.
		/// </param>	
		/// <see>setLocalStorage</see>
		/// <exception cref="CArtDatabaseException">Thrown when path does not exist.</exception>
		void setLocalStoragePath(string path);

		/// <summary>
		/// Overload the output operator << in order to show vectors
		/// </summary>	
		/// <param name="stream">The output stream.</param>	
		/// <param name="ob">Object to be displayed.</param>	
		/// <returns>Modified output stream.</returns>
		friend ostream& operator<<(ostream& stream,vector<int> ob);
		friend ostream& operator<<(ostream& stream,vector<string> ob);
		//friend ostream& operator<<(ostream& stream,CArtDatabase ob);
		//friend istream& operator>>(istream& stream,CArtDatabase ob);

	private:
		/// <summary>
		/// This flag indicates wether the images are sotred in a local folder or not.
		/// </summary>
		bool m_UseLocalStorage;
		/// <summary>
		/// Path where images are sotrored in case local storage is activated.
		/// </summary>
		string m_LocalStoragePath;
		/// <summary>
		/// Table with image URLs
		/// </summary>
		map<int,string> m_ImageURL;	
		/// <summary>
		/// Table with image Labels
		/// </summary>
		map<int,intVector> m_ImageLabels;
		/// <summary>
		/// Path to the database information	
		/// </summary>	
		string m_dbPath;
		/// <summary>
		/// List of dictionaries for labels translation
		/// </summary>	
		CLabelDictionary **m_Dictionaries;	
	};
}

#endif //__ART_DATABASE__H

#include "FeatureExtractorAverageSum.h"
#include "FeatureAverageSum.h"

FeatureExtractorAverageSum::FeatureExtractorAverageSum(void)
{
}

FeatureExtractorAverageSum::FeatureExtractorAverageSum(Mat& image):FeatureExtractor(image)
{
	
	cvtColor( image, _image , CV_BGR2GRAY );
	threshold(_image ,_image ,100,255, THRESH_BINARY);
}

void FeatureExtractorAverageSum::setImage(Mat& img)
{ 
	cvtColor( img, _image , CV_BGR2GRAY );
	threshold(_image ,_image ,100,255, THRESH_BINARY);
}

void FeatureExtractorAverageSum::extract(vector<Feature*>& avrgsums)
{
	int coloredCount ;
	vector<int> avgsums =   vector<int>();
	
	for(int i = 0 ; i< _image.cols;i++ ) //run on all the colums
	{   
		coloredCount = 0 ;
		for(int j = 1 ; j < _image.rows;j++) //run on all the rows
		{
			if( (_image.at<uchar>(j,i)) == ((uchar)(0)) ) //
			{
				coloredCount++;	
			}
		}
		avgsums.push_back(int(coloredCount));

	}
	FeatureAverageSum* newFeature = new FeatureAverageSum(avgsums);
	avrgsums.push_back(newFeature);
}

FeatureExtractorAverageSum::~FeatureExtractorAverageSum(void)
{
}
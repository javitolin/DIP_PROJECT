#include "stdtls.h"
#include "FeatureAverageSum.h"

FeatureAverageSum::FeatureAverageSum(vector<int> avgsums)
{
	_avgsums.insert(_avgsums.begin(), avgsums.begin(), avgsums.end());


}

double FeatureAverageSum::distance(Feature* a)
{

	FeatureAverageSum* other = (FeatureAverageSum*)a;
	if (other->_avgsums.size() > _avgsums.size()){
		vector<int> sample = stdtls::resample(_avgsums, (int)other->_avgsums.size());
		return stdtls::euclidean(sample, other->_avgsums);
	}
	else{
		vector<int> sample = stdtls::resample(other->_avgsums, _avgsums.size());
		return stdtls::euclidean(sample, _avgsums);
	}

}


void  FeatureAverageSum::sample2Row(Mat mat, int row)
{
	//row is the row in the matrix needed to be assigned
	//we will assign the row in mat to be _avgsums
	int col = 0;
	if (mat.rows <= row)
	{
		throw(new invalid_argument("invalid arguments type, mat hight "));
	}
	for (int i = 0; i<_avgsums.size() && col != mat.cols; col++, i++)
	{
		mat.at<uchar>(row, col, 0) = _avgsums.at(i);
	}

}

vector<float> FeatureAverageSum::vectorize()
{
	vector<float> returnVector = vector<float>();

	for (int i = 0; i < _avgsums.size(); i++){
		returnVector.push_back(float(_avgsums.at(i)));

	}
	return returnVector;
	
}

FeatureAverageSum::~FeatureAverageSum(void)
{

}



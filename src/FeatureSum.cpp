#include "stdtls.h"
#include "FeatureSum.h"

FeatureSum::FeatureSum(vector<int> sums)
{
	_sums.insert(_sums.begin(), sums.begin(), sums.end());

}

double FeatureSum::distance(Feature* a)
{
	FeatureSum* other = (FeatureSum*)a ;

	if ( other->_sums.size() > _sums.size()){
		vector<int> sample = stdtls::resample(_sums, (int)other->_sums.size());
		return stdtls::euclidean(sample, other->_sums);
	}
	else{ 
		vector<int> sample = stdtls::resample(other->_sums, _sums.size());
		return stdtls::euclidean(sample, _sums);
	}

}
void  FeatureSum::sample2Row(Mat mat, int row)
{
	//row is the row in the matrix needed to be assigned
	//we will assign the row in mat to be _sums
	int col = 0;


	for (int i = 0; i<_sums.size() && col != mat.cols; col++, i++)
	{
		mat.at<uchar>(row, col, 0) = _sums.at(i);
	}
}

vector<float> FeatureSum::vectorize()
{
	vector<float> returnVector = vector<float>();
	for (int i = 0; i < _sums.size(); i++){
		returnVector.push_back(float(_sums.at(i)));
	}
	return returnVector;
}
FeatureSum::~FeatureSum(void)
{
}

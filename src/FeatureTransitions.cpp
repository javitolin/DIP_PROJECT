#include "stdtls.h"
#include "FeatureTransitions.h"
FeatureTransitions::FeatureTransitions(vector<int> trans)
{
	_trans.insert(_trans.begin(), trans.begin(), trans.end());
}


double FeatureTransitions::distance(Feature* a)
{
	FeatureTransitions* other = (FeatureTransitions*)a ;

	if ( other->_trans.size() > _trans.size()){
		vector<int> sample = stdtls::resample(_trans, (int)other->_trans.size());
		return stdtls::euclidean(sample, other->_trans);
	}
	else{ 
		vector<int> sample = stdtls::resample(other->_trans, _trans.size());
		return stdtls::euclidean(sample, _trans);
	}
}

void  FeatureTransitions::sample2Row(Mat mat, int row)
{
	//row is the row in the matrix needed to be assigned
	//we will assign the row in mat to be _trans
	int col = 0;


	for (int i = 0; i<_trans.size() && col != mat.cols; col++, i++)
	{
		mat.at<uchar>(row, col, 0) = _trans.at(i);
	}
}
vector<float> FeatureTransitions::vectorize()
{
	vector<float> returnVector = vector<float>();
	for (int i = 0; i < _trans.size(); i++){
		returnVector.push_back(float(_trans.at(i)));
	}
	return returnVector;
}


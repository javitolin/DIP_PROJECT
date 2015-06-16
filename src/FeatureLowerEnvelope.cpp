#include "stdtls.h"
#include "FeatureLowerEnvelope.h"


FeatureLowerEnvelope::FeatureLowerEnvelope(vector<int> lower) :Feature()
{

	_low.insert(_low.begin(), lower.begin(), lower.end());

}

double FeatureLowerEnvelope::distance(Feature* a){

	FeatureLowerEnvelope* other = (FeatureLowerEnvelope*)a ;
	vector<int> sample;

	if (other->_low.size() > _low.size()){
		sample = stdtls::resample(_low, (int)other->_low.size());
		return stdtls::euclidean(sample, other->_low);
	}
	else{
		sample = stdtls::resample(other->_low, _low.size());
		return stdtls::euclidean(sample, _low);
	}
}

void FeatureLowerEnvelope::sample2Row(Mat mat, int row) {
	//row is the row in the matrix needed to be assigned
	//we will assign the row in mat to be _low
	int col = 0;

	for (unsigned int i=0; i<_low.size() && col != mat.cols; col++, i++)
	{
		mat.at<uchar>(row, col, 0) = _low.at(i);
	}
}

vector<float> FeatureLowerEnvelope::vectorize() {
	vector<float> returnVector = vector<float>();
	for (unsigned int i = 0; i < _low.size(); i++){
		returnVector.push_back(float(_low.at(i)));
	return returnVector;
	}
}

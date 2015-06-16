#include "stdtls.h"
#include "FeatureUpperEnvelope.h"


FeatureUpperEnvelope::FeatureUpperEnvelope(vector<int> upper) {
	_upp.insert(_upp.begin(), upper.begin(), upper.end());
}

double FeatureUpperEnvelope::distance(Feature* a){
	FeatureUpperEnvelope* other = (FeatureUpperEnvelope*)a ;

	if ( other->_upp.size() > _upp.size()){
		vector<int> sample = stdtls::resample(_upp, (int)other->_upp.size());
		return stdtls::euclidean(sample, other->_upp);
	}
	else{ 
		vector<int> sample = stdtls::resample(other->_upp, _upp.size());
		return stdtls::euclidean(sample, _upp);
	}
}

void  FeatureUpperEnvelope::sample2Row(Mat mat, int row)
{
	//row is the row in the matrix needed to be assigned
	//we will assign the row in mat to be _upp
	int col = 0;


	for (int i = 0; i<_upp.size() && col != mat.cols; col++, i++)
	{
		mat.at<uchar>(row, col, 0) = _upp.at(i);
	}
}

vector<float> FeatureUpperEnvelope::vectorize() {
	vector<float> returnVector = vector<float>();
	for (int i = 0; i < _upp.size(); i++){
		returnVector.push_back(float(_upp.at(i)));
	}
	return returnVector;
}

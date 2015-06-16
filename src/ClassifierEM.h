#ifndef _EMClassifier_H_
#define _EMClassifier_H_ 

#include <opencv/ml.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include "Feature.h"
#include "stdafx.h"
#include "Classifier.h"
#include "ManuscriptExport.h"
#include "FeatureGaborBlob.h"

using namespace cv;

class ClassifierEM : public Classifier {
	EM   _model ;
	int  _nclusters ;
	Mat  _probability ;

public:
	ClassifierEM(void);
	~ClassifierEM(void);

	void  setNumClusters(int n){
		_model.set("nclusters", n);
	}

	int   predict(Feature* feature, float& probability) ;
	void  train(vector<Feature*> features) ;
};

#endif

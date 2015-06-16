#ifndef _FEATUREUPPERENVELOPE_H_  
#define _FEATUREUPPERENVELOPE_H_  
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include "Feature.h"
#include "FeaturePixel.h"
#include <math.h>
using namespace std;
using namespace cv ;

//It locates the �upper envelope� of a letter, and assign it to a vector. 
//sample2row creates a mapping in the original matrix (image) size and assigns pixels in the upper envelope .
	
class FeatureUpperEnvelope: public Feature {
protected:
	vector<int> _upp ;
public:
	FeatureUpperEnvelope(vector<int> upper);
	~FeatureUpperEnvelope(void){;}

	double distance(Feature* a);
    void sample2Row(Mat  mat, int row);
	vector<float> vectorize()  ; 

};
#endif



#ifndef _TRANSFORMTION_DISTANCE_ONGRAY_H_ 
#define _TRANSFORMTION_DISTANCE_ONGRAY_H_ 

#include "ImageTransformation.h"

class TransformDistanceOnGray : public ImageTransformation {
public:
	TransformDistanceOnGray(void);
	~TransformDistanceOnGray(void);

	DImage* transform() ;
};

#endif 

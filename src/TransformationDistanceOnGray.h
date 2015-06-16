#ifndef _TRANSFORMTION_DISTANCE_ONGRAY_H_ 
#define _TRANSFORMTION_DISTANCE_ONGRAY_H_ 

#include "ImageTransformation.h"

class TransformationDistanceOnGray : public ImageTransformation {
public:
	TransformationDistanceOnGray(void);
	~TransformationDistanceOnGray(void);

	DImage* transform() ;
};

#endif 

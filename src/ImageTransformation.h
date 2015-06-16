#ifndef _IMAGE_TRANSFORMATION_H_ 
#define _IMAGE_TRANSFORMATION_H_ 

#include <opencv/cv.h>
#include "ImageOperator.h"

class ImageTransformation : public ImageOperator {
public:

	virtual DImage* transform() = 0 ;
};

#endif 

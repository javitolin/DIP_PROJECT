#ifndef _IMAGE_FILTER_H_ 
#define _IMAGE_FILTER_H_

#include "ImageOperator.h"
#include "ManuscriptExport.h"

class ImageFilter : public ImageOperator {



public:
	ImageFilter(void) {
	}

	~ImageFilter(void){
	}
	
	virtual DImage* filter() = 0;

	
};

#endif 

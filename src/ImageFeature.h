#ifndef _IMAGE_FEATURE_H_ 
#define _IMAGE_FEATURE_H_ 

#include "ManuscriptExport.h"

class MANUSCRIPT_EXPORT ImageFeature {
public:
	ImageFeature(void){;}
	~ImageFeature(void){;}

	virtual operator = (ImageFeature& ) = 0 ;
	virtual operator - (ImageFeature& ) = 0 ;
	virtual operator + (ImageFeature& ) = 0 ;

	virtual double distance(ImageFeature& a) = 0 ;
}
#endif

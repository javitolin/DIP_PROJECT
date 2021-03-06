#ifndef _OTSUGRAYBINARIZER_H 
#define _OTSUGRAYBINARIZER_H 

#include "ImageBinarizer.h"
#include "ManuscriptExport.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Otsul binarizer class implements the Otsu binarization algorithm </summary>
///
/// <remarks>	El Sana, 2/15/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class BinarizerOtsuGray : public ImageBinarizer {

public:
	DImage* binarize();
};

#endif 

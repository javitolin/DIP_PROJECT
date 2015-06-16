#ifndef __BinaryComponentExtractor_H_
#define __BinaryComponentExtractor_H_

#include "ComponentExtractor.h"
#include "ManuscriptExport.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Binary component extractor class </summary>
///
/// <remarks>	El Sana, 2/15/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class ComponentExtractorBinary : public ComponentExtractor {

public:
	ComponentExtractorBinary()  { ; }
	ComponentExtractorBinary(DImage *img) :ComponentExtractor(img){ ; }
	~ComponentExtractorBinary(void) { ; }

	void collectComponents(vector<vector<Point> >& contours, vector<Vec4i> hierarchy, vector<ConnectedComponent*>& component);
	void extract(vector<ConnectedComponent*>&) ;
};

#endif 

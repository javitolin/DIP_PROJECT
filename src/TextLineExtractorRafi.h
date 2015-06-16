#ifndef _RAFI_TEXT_LINE_EXTRACTOR_
#define _RAFI_TEXT_LINE_EXTRACTOR_

#include "TextLineExtractor.h"
#include "BinaryComponentTree.h"
#include "ManuscriptExport.h"

class TextLineExtractorRafi :public TextLineExtractor{
	Mat filterDocument();
	int findRootThreshold();
	BinaryComponentTree buildComponentTree();
	void recursiveTraversal(BinaryComponentNode* current_node, vector<ConnectedComponent*>& result, double wordSizeEst);
	vector<ConnectedComponent*> TraverseTree();
public:
	TextLineExtractorRafi();
	void extract();
};
#endif

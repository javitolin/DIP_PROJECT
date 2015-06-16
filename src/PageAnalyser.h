#ifndef _PAGE_ANALYSER_H_ 
#define _PAGE_ANALYSER_H_

#include <iostream>
#include <vector>
#include "PageImage.h"
#include "ManuscriptExport.h"
using namespace std ;


class MANUSCRIPT_EXPORT PageAnalyser {
	PageImage _page ;

public:
	PageAnalyser(void){
	}

	virtual ~PageAnalyser(void){
	}

	virtual void determineMain() = 0 ;
	virtual void determineMargins() = 0 ; 
	virtual void setMargineNotes() = 0 ;
};

#endif 
#ifndef _ITAYBINARIZER_H 
#define _ITAYBINARIZER_H 

#include "cv_macros.h"
#include "ImageBinarizer.h"
#include "ManuscriptExport.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Itay binarizer class implements the Itay binarization algorithm </summary>
///
/// <remarks>	El Sana, 2/15/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MANUSCRIPT_EXPORT BinarizerItay : public ImageBinarizer {

protected:
	int    _maxStrokeWidth  ;
	int    _windowSize ;
	int    _areaThreshold ;
	double _edgeThreshold ;

	const static int EPSILON = 0.0001 ;
	const static int BACKGROUND =0 ;
	const static int FOREGROUND = 1;
public:
	BinarizerItay(void);
	~BinarizerItay(void);

	IplImage* Local_Min_Max(const IplImage * imgSrc,IplImage * imgCnt,IplImage * E_bin,int window);
	Mat normalizeContrast(const Mat src_img); 

	void considerComponents(Mat initial_binary, Mat binary );

	Mat   localMaskedThreshold(const Mat img, Mat zeroed_img, Mat E_bin,int window);
	void  zeroImageMask(IplImage *zeroedImg,const IplImage *E_bin);
	float sumWindow(const Mat integral_img, const Point& pixel, int window) ;

	//Connected component labeling
	inline bool isEdge(Mat img, int row, int col) ;
	//double  CCLabel_Flood(CvMat * img);
	double lableFloodConnectedComponents(Mat img);

	//void Filter_Comps(CvMat *CCLabel, IplImage * img,IplImage * E_bin,int *areaComp,int numLabels);
	void filterComponents(Mat labeled_img, Mat img, Mat E_bin, int *area_comp, int nlabels);
	
	// Run-length
	// int run_length_hist(IplImage * src);
	int run_length_hist(Mat src);

	//Remove small blobs according to stroke width
	//void remove_small_blobs(IplImage *bin,CvMat *labelImg,int *areaComp);
	void removeSmallBlobs(Mat bin, Mat labeled_img, int *area_comp);

	//Thin image
	//IplImage * getSkel(const IplImage * img);
	Mat getSkel(const Mat img);
	//Local Radius
	//IplImage * getLocalRadius(IplImage * img,IplImage * skl);
	Mat getLocalRadius(Mat img, Mat skl);
	//Conversion between types

	DImage* binarize();
};

#endif 
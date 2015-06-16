/*#include "stdafx.h"
#include "BinarizerItay.h"
#include "DImage.h"
#include "LevelSetBinarize.h"

BinarizerItay::BinarizerItay(void){
	_windowSize=28;
	_areaThreshold = 4 ;
	_edgeThreshold = 0.3 ;
}


BinarizerItay::~BinarizerItay(void){
	;
}

// img is a CV_8U gray scale image 
Mat BinarizerItay::normalizeContrast(const Mat img){
	//Calculate minimum and maximum in a 3*3 neighberhood
	Mat  img_normalized(img.size(), CV_32FC1);
	Mat local_max(img.size(), CV_8UC1);
	Mat local_min(img.size(), CV_8UC1);
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));                                  
	erode(img, local_min, kernel);
	dilate(img, local_min, kernel);

	for( int row = 0; row < img.rows; row++ )
		for ( int col = 0 ; col < img.cols; col++ ){
			float min = (float)local_min.atuchar(row, col);
			float max = (float)local_max.atuchar(row, col);
			img_normalized.atfloat(row, col) = (max - min)/(max+min+EPSILON) ;
		}
	return img_normalized ;
}

float BinarizerItay::sumWindow(const Mat integral_img, const Point& pixel, int window){
	assert(window != 0);

	int max_col = min(pixel.x + window,  integral_img.cols-1);
	int min_col = max(pixel.x - window -1,  1);
	int max_row = min(pixel.y + window,  integral_img.rows-1);
	int min_row = max(pixel.x - window,  1);
	
	if ( integral_img.type() == CV_32FC1 ) 
		return (integral_img.atfloat(max_col, max_row) - integral_img.atfloat(min_col, max_row) + 
			    integral_img.atfloat(min_col, min_row) - integral_img.atfloat(max_col, min_row));
	else
		return (integral_img.atdouble(max_col, max_row) - integral_img.atdouble(min_col, max_row) + 
			    integral_img.atdouble(min_col, min_row) - integral_img.atdouble(max_col, min_row));
}

Mat BinarizerItay::localMaskedThreshold(const Mat img, Mat zeroed_img, Mat E_bin,int window){
	Mat result(img.size(), CV_8UC1);

	//calculate number of high contrast pixels (in E_bin)
	Mat local_num(E_bin.rows+1,E_bin.cols+1,CV_32FC1);
	integral(E_bin, local_num);

	//Calute local sum in non-zero elements of zeroedImg
	Mat local_sum(zeroed_img.rows+1, zeroed_img.cols+1, CV_32FC1); 
	Mat local_sqs(zeroed_img.rows+1, zeroed_img.cols+1, CV_64FC1); 
	integral(zeroed_img, local_sum, local_sqs);

	for( int row = 0; row < img.rows; row++ )
		for( int col = 0; col < img.cols; col++ ){
			float curr_num = sumWindow(local_num, Point(row+1,col+1), window); //number of high contrast pixels
			float curr_sum = sumWindow(local_sum, Point(row+1,col+1), window); //sum of gray scale pixels (only high contrast pixels)
			float curr_sqs = sumWindow(local_sqs, Point(row+1,col+1), window); //sum of squared gray scale pixels (only high contrast pixels)

			if (curr_num == 0)
					result.atuchar(col, row) = 0 ;//if there is no high contrast pixels in the hood
			else {
				float mean   = curr_sum/curr_num ; //curr_num is not zero
				float mean_sq= curr_sqs/curr_num;  //curr_num is not zero
				double threshold = mean + 0.5*sqrt(mean_sq - mean*mean) ;
				result.atuchar(row, col) = ( threshold > (double)img.atuchar(row, col))? 1 : 0 ;
			}
		}
	return result;
}


inline bool BinarizerItay::isEdge(Mat img, int row, int col){
	return ( img.atuchar(row, col-1) == 0   || img.atuchar(row, col+1) == 0   || img.atuchar(row-1, col) == 0   || img.atuchar(row+1, col) == 0 ||
			 img.atuchar(row-1, col-1) == 0 || img.atuchar(row+1, col-1) == 0 || img.atuchar(row-1, col+1) == 0 || img.atuchar(row+1, col+1) == 0);
}

// Start label from 2, as 0 and 1 are already occupied
// img is a CV_32FC1
double BinarizerItay::lableFloodConnectedComponents(Mat img){

	double label = 2;
	for ( int row = 0; row < img.rows; row++)
		for ( int col = 0 ; col < img.cols; col++){
			if ( img.atuint(row, col) == 1 ){
				floodFill(img, Point(col, row), Scalar(label), (Rect *)0, Scalar(0), Scalar(0), 8);
				label++ ;
			}
		}
	return label-2 ;
}

//*************
//Labels start from 2 (1 and 1 are occupied)
//Updates both CClabel and img
	
// (labeled_img, binary, initial_binary, area_comp, count);
void BinarizerItay::filterComponents(Mat labeled_img, Mat binary_img, Mat initial_binary, int *area_comp, int nlabels){

	//features for filtering
	int *edge_comp=new int[nlabels];
	int *contrast_comp=new int[nlabels];

	memset(area_comp, 0, sizeof(int)*nlabels);
	memset(edge_comp, 0, sizeof(int)*nlabels);
	memset(contrast_comp, 0, sizeof(int)*nlabels);
	
	// Calculate statistics 
	for (int row = 1; row < binary_img.rows; row++ )
		for ( int col = 1 ; col < binary_img.cols; col++){
			if ( binary_img.atuchar(row, col) == 1 ){
				int label = labeled_img.atuint(row, col);
				area_comp[label]++;
				if (isEdge(binary_img, row, col))
					edge_comp[label]++;

				if ( initial_binary.atuchar(row, col) == 1 )
					contrast_comp[label] ++ ;
			}
		}

	// Filter Area 
	for (int row = 1; row < binary_img.rows; row++ )
		for ( int col = 1 ; col < binary_img.cols; col++){
			if ( binary_img.atuchar(row, col) == 1 ){
				int label = labeled_img.atuint(row, col) ;
				double edge_rate = (double)(contrast_comp[label])/(double)(edge_comp[label]) ;
				// Filter out small components and weak edges 
				if ( area_comp[label] < _areaThreshold || edge_rate < _edgeThreshold ){
					binary_img.atuchar(row, col) = 0 ;
					labeled_img.atuint(row, col) = 0 ;
				}
			}
		}
}


int BinarizerItay::run_length_hist(Mat img){
	int _maxBinSize = 1000 ;
	int *hist_hor=new int[_maxBinSize];
	int *hist_ver=new int[_maxBinSize];

	memset(hist_hor, 0, _maxBinSize*sizeof(int));
	memset(hist_ver, 0, _maxBinSize*sizeof(int));

	// Horizontal Scan 
	int row = 0 ;
	int col = 0 ;
	int pixel_count ;
	while (row < img.rows ){
		pixel_count = 0 ;
		// Skip background pixels 
		while ( col < img.cols && img.atuchar(row, col) == BACKGROUND )
			col++ ;
		//Count number of horizontal pixels
		while ( col < img.cols && img.atuchar(row, col) == FOREGROUND ){
			pixel_count ++ ;
			col ++ ;
		}

		if (pixel_count > 0  && pixel_count < _maxBinSize )
			hist_hor[pixel_count]++;

		// Reached the end of the row move to the next one 
		if ( col == img.cols ){
			col = 0 ;
			row++ ;
		}
	}

	// Vectical Scan 
	row = col = 0 ;
	while ( col < img.cols ){
		pixel_count = 0 ;
		// Skip background pixels 
		while ( row < img.rows && img.atuchar(row, col) == BACKGROUND ) 
			row++ ;

		//Count number of vertical pixels
		while ( row < img.rows && img.atuchar(row, col) == FOREGROUND ){
			pixel_count ++ ;
			row ++ ;
		}

		if ( pixel_count > 0 && pixel_count < _maxBinSize )
			hist_ver[pixel_count]++;
			
		if ( row == img.rows ){
			row = 0 ;
			col ++ ;
		}
	}

	int max_val = 0 ;
	int max_idx = 0 ;
	for ( int i = 0 ; i < _maxBinSize; i++ ){
		int sum_hist = hist_hor[i] + hist_ver[i] ;
		if ( sum_hist > max_val ){
			max_val = sum_hist ;
			max_idx = i ;
		}
	}

	delete hist_hor;
	delete hist_ver;
	return max_idx ;
}//run-length


void BinarizerItay::removeSmallBlobs(Mat bin, Mat labeled_img, int *area_comp){
		
	_maxStrokeWidth = 40 ;
	int stroke_width = run_length_hist(bin);//Get Stroke width
	int min_area     = min(stroke_width*stroke_width, _maxStrokeWidth);

	for ( int row = 0; row < bin.rows; row++ )
		for ( int col = 0 ; col < bin.cols; col++)
			if ( bin.atuchar(row, col) == 1 && area_comp[labeled_img.atuint(row, col)-2] < min_area){
				// Filter it out 
				bin.atuchar(row, col) = 0 ;
				labeled_img.atuint(row, col) = 0 ; 
			}
}


Mat BinarizerItay::getSkel(const Mat img){

	Mat dst_img(img.size(), CV_32FC1);
	Mat max_img(img.size(), CV_32FC1);

	Mat tmp_img(img.size(), CV_8UC1);

	img.copyTo(tmp_img);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3)); 
	dilate(tmp_img, tmp_img, kernel);
	distanceTransform(tmp_img, dst_img, CV_DIST_L1, 3);
	dilate(dst_img, max_img, kernel);

	Mat skel_img = Mat::zeros(img.size(), CV_8UC1);
	for( int row = 0; row < skel_img.rows; row++)
		for( int col = 1; col < skel_img.cols; col++)
			if (( tmp_img.atuchar(row, col) == 1 ) && ( dst_img.atfloat(row, col) = max_img.atfloat(row, col)))
				skel_img.atuchar(row, col) = 1 ;

	return skel_img;
}


Mat BinarizerItay::getLocalRadius(Mat img, Mat skl){
	const int MIN_RAD=2;
	const int MAX_RAD=50;

	Mat dst_img(img.size(), CV_32FC1);
	Mat dst_skl(skl.size(), CV_32FC1);

	Mat edge_img = Mat::ones(img.size(), CV_8UC1);
	Mat skl_img  = Mat::ones(img.size(), CV_8UC1); //Inverted skl

	for( int row = 1; row < edge_img.rows-1; row++)
		for( int col = 1 ; col < edge_img.cols-1 ; col++){
			if ( img.atuchar(row, col) == FOREGROUND && isEdge(img, row, col))
				edge_img.atuchar(row, col) = 0 ;
			skl_img.atuchar(row, col) = 1 - (int)skl.atfloat(row, col);
		}

	cv::distanceTransform(edge_img, dst_img, CV_DIST_L2, 3);
	cv::distanceTransform(skl_img, dst_skl, CV_DIST_L2, 3);

	Mat radius_img(img.size(), CV_8UC1);//Local radius for Level set
	radius_img.setTo(MIN_RAD);

	double dT=0,dS=0,radius=6;
	for ( int row = 1 ; row < edge_img.rows-1; row++)
		for( int col = 1 ; col < edge_img.cols-1; col++){
			dT = dst_img.atfloat(row, col);
			dS = dst_skl.atfloat(row, col);
			if ( img.atuchar(row, col) == FOREGROUND )
				radius=(dT+dS)*2;
			else 
				radius = ( dT == dS )? dT*2 : (dS-dT)*2;
			radius = max(MIN_RAD,radius);//Not smaller then MIN_RAD
			radius = min(radius,MAX_RAD);//Not larger then MAX_RAD

			radius_img.atuchar(row, col) = cvCeil(radius);
		}
	return radius_img;
}


void BinarizerItay::considerComponents(Mat initial_binary, Mat binary ){
	Mat labeled_img(initial_binary.size(), CV_32SC1);
	binary.copyTo(labeled_img);

	double count = lableFloodConnectedComponents(labeled_img);
	int *area_comp = new int[(int)count];
	filterComponents(labeled_img, binary, initial_binary, area_comp, count);
	removeSmallBlobs(initial_binary, labeled_img, area_comp);

}

DImage* BinarizerItay::binarize(){
	Mat img ;
	if ( _image->getMat().channels > 1)
		cvtColor(_image->getMat(), img, CV_BGR2GRAY);
	else
		img = _image->getMat().clone() ;

	// From now on img is a 8bit gray scale image 
	Mat img_contrast = normalizeContrast(img);
	Mat initial_binary(img.size(), CV_8UC1);
	threshold(img_contrast, initial_binary, 0, 1, CV_THRESH_BINARY|CV_THRESH_OTSU);

	Mat zeroed_img = Mat::zeros(img.size(), CV_8U); 
	img.copyTo(zeroed_img, initial_binary); 
	Mat binary = localMaskedThreshold(img, zeroed_img, initial_binary, _windowSize);

	considerComponents(initial_binary, binary );
	
	Mat skl=getSkel(binary);
	Mat radius = getLocalRadius(binary,skl);

	//int _niter = 35 ;
	//Level_Set_Bin *lb =new Level_Set_Bin(mat,binary,radius);
	//lb->FastEvolveAC(niter);

	LevelSetBinarize leve_set(img, binary, radius) ;
	leve_set.evolveActiveContour(35);

	return new DImage(binary);
}
*/

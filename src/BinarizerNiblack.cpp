/*#include "BinarizerNiblack.h"


BinarizerNiblack::BinarizerNiblack(){
	_window = Size(20,20);
	_k = 0.2 ;
}

BinarizerNiblack::BinarizerNiblack(int width, int height){
	_window.width  = width ;
	_window.height = height ;
}

BinarizerNiblack::~BinarizerNiblack(void){
}


double BinarizerNiblack::setMeanStdMaps(Mat img, Size window){
	Mat mat_sum, mat_ssq;
	Mat ext_img ;
	ext_img = img ;
	copyMakeBorder(img, ext_img, window.height/2, window.height/2, window.width/2, window.width/2, BORDER_REPLICATE);
	cv::integral(ext_img, mat_sum, mat_ssq, CV_64F);
	_max_std = 0 ;
	for (int row = window.height; row < ext_img.rows; row++)
		for ( int col = window.width; col < ext_img.cols; col++){
			double sum = 0 ;
			double ssq = 0;

			sum = mat_sum.atdouble(row, col) - mat_sum.atdouble(row - window.height, col) - mat_sum.atdouble(row, col - window.width) + mat_sum.atdouble(row - window.height, col - window.width) ;
			ssq = mat_ssq.atdouble(row, col) - mat_ssq.atdouble(row - window.height, col) - mat_ssq.atdouble(row, col - window.width) + mat_ssq.atdouble(row - window.height, col - window.width) ;

			int map_row = row - window.height ;
			int map_col = col - window.width ;
			_mean.atfloat(map_row, map_col) = sum / window.area() ;
			_std.atfloat(map_row, map_col)  = sqrt(ssq - sum*sum/window.area())/window.area();

			if ( _std.atfloat(map_row, map_col ) > _max_std )
				_max_std = _std.atfloat(map_row, map_col ) ;
		}
	return _max_std ;
}

void BinarizerNiblack::setThreshold(Size size, float k){
	
	for ( int row = 0; row < _threshold.rows; row++)
		for ( int col = 0 ; col < _threshold.cols; col++)
			_threshold.atfloat(row, col) = _mean.atfloat(row, col) + k * _std.atfloat(row, col);
}


DImage* BinarizerNiblack::binarize(){
	Mat img    = _image->getMat() ;
	Mat binary = Mat(img.size(), CV_8U);

	// initialize auxilary matices 
	_mean.create(img.size(), CV_32F);
	_std.create(img.size(), CV_32F);
	_threshold.create(img.size(), CV_32F);

	setMeanStdMaps(img, _window);
	setThreshold(_window, _k);
	for ( int row = 0; row < _threshold.rows; row++)
		for ( int col = 0 ; col < _threshold.cols; col++){
			if ( img.atuchar(row, col) >= _threshold.atfloat(row, col) )
				binary.atuchar(row, col) = 255 ;
			else 
				binary.atuchar(row, col) = 0 ;
		}
	return new DImage(binary);		
}*/

#include "stdafx.h"
#include "DImage.h"
#include "ImageTools.h" 
#include "TransformationDistanceOnGray.h"


TransformationDistanceOnGray::TransformationDistanceOnGray(void){
}



TransformationDistanceOnGray::~TransformationDistanceOnGray(void){
}

DImage* TransformationDistanceOnGray::transform(){
	Mat img = _image->getMat() ;
	Mat distance_transform ;
	int histogram[256] ;

	ImageTools::histogramGrayImage(img, histogram);

	// Sum all the thresolds in a single matrix (section 6 of the paper fo uniform)
	// initializate with zeros, otherwise will be garbage
	Mat gray_distance = Mat::zeros(img.size(), CV_32FC1);

	// now, passes all the thresholds
	for(int treshold = 1; treshold < 255; treshold++){
		if ( histogram[treshold] > 0 ) {
			Mat threshold_img, distance_img;
			// generate the threshold and the binary transformation
			threshold( img, threshold_img,  treshold,  255, THRESH_BINARY);
			distanceTransform(threshold_img, distance_img, CV_DIST_L2, CV_DIST_MASK_PRECISE);
			// add to the cumulative
			gray_distance += distance_img;
		}
	}

	// since our output matrix is a matrix of doubles containing distances, we need to convert it to an uchar scale to display
	normalize(gray_distance, distance_transform, 255.0, 0.0, NORM_MINMAX, CV_8UC1);

	return new DImage(distance_transform) ;
}

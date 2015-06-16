#include "stdafx.h"
#include "DImage.h"
#include "TransformDistanceOnGray.h"

TransformDistanceOnGray::TransformDistanceOnGray(void){
}



TransformDistanceOnGray::~TransformDistanceOnGray(void){
}

DImage* TransformDistanceOnGray::transform(){
	Mat img = _image->getMat() ;
	Mat distance_transform ;
	

	return new DImage(distance_transform) ;
}

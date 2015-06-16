#include "stdafx.h"
#include "ImageTools.h"
#include "FeatureExtractorBlob.h"
#include "DImage.h"
#include "BinarizerOtsu.h"
#include "BinaryComponentExtractor.h"
//#include "ImageKernel.h"

// Task Replace the binadry blob image (pixels) by the pixels from the original response (32bit)
FeatureGaborBlob* FeatureExtractorBlob::getComponentFeature(ConnectedComponent* component){
	FeatureGaborBlob* feature = new FeatureGaborBlob();
	feature->setBlob(component->getImage());
	return feature ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Feature visualization image. </summary>
///
/// <remarks>	Jihad, 19/12/2014. </remarks>
///
/// <param name="size">	The size. </param>
/// <param name="list">	[in,out] [in,out] If non-null, the list. </param>
///
/// <returns>	A Mat. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Mat FeatureExtractorBlob::featureVisualizationImage(Size size, vector<Feature*>& list, vector<Mat>& images){
	Mat mat(size, CV_32FC3);
	vector<Feature*>::iterator iter ;
	for ( iter = list.begin(); iter != list.end(); iter++ ){
		FeatureGaborBlob* feature = (FeatureGaborBlob*)(*iter) ;
		Mat blob = feature->getColorCodedBlob() ;
		int orgx = feature->getOrg().x ;
		int orgy = feature->getOrg().y ;
		Mat roi  = mat(Range(orgy, orgy + blob.rows), Range(orgx, orgx + blob.cols));
		Mat mask = feature->getContour().getMask() ;
		blob.copyTo(roi, mask) ;

		///
		Mat image(size, CV_32FC3);
		Mat image_roi = image(Range(orgy, orgy + blob.rows), Range(orgx, orgx + blob.cols));
		Mat image_mask = feature->getContour().getMask() ;
		blob.copyTo(image_roi, image_mask) ;
		images.push_back(image);
	}
	return mat ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Process the response and generate a feature list, one feature for each blob </summary>
///  
/// <remarks>	Jihad, 09/12/2014. </remarks>
///
/// <param name="response">	[in,out] The response. </param>
/// <param name="list">	   	[in,out] [in,out] If non-null, the list. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void FeatureExtractorBlob::processResponse(DImage& response, float angle, vector<Feature*>& list){
	vector<ConnectedComponent*> component_list ;

	// Binarize the response image 
	BinarizerOtsu binarizer(1024, 0, 1.0);
	DImage* binary_response = response.binarize(binarizer);

	// Extarct the connected components from the binary image 
	BinaryComponentExtractor component_extractor;
	binary_response->extractComponents(component_extractor, component_list);

	for (int i = 0 ; i < component_list.size() ; i++){
		FeatureGaborBlob* feature = new FeatureGaborBlob();
		Mat  blob = extractBlobFromResponse(response,  component_list[i]->getContour());
		Rect rect = component_list[i]->getContour().getBoundRect() ;
		feature->setOrg(Point(rect.x, rect.y));
		feature->setAngle(angle);
		feature->setBlob(blob);
		feature->setContour(component_list[i]->getContour());
		list.push_back(feature);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Extracts the BLOB from response. Copy the blob into a new image by generating a mask
/// and copying the relevant pixels. </summary>
///
/// <remarks>	Jihad, 17/12/2014. </remarks>
///
/// <param name="response">	[in,out] The response. </param>
/// <param name="contour"> 	[in,out] The contour. </param>
///
/// <returns>	The extracted BLOB from response. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Mat FeatureExtractorBlob::extractBlobFromResponse(DImage& response,  Contour& contour){
	Mat mask = contour.getMask() ; 

	// Copy the pixel of the component from the corresponding rectangle in the respose image 
	Mat roi(response.getMat(), contour.getBoundRect());
	Mat blob = Mat::zeros(contour.getBoundRect().size(), CV_32F);
	roi.copyTo(blob, mask);
	return blob ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Generates an orientation list. </summary>
///
/// <remarks>	Jihad, 09/12/2014. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void FeatureExtractorBlob::generateOrientationList(){
	_orientationList.clear() ;
	float orientationStep = 180.0f/_numOrientation ;
	_curOrientation = 0 ; 
	for ( int i = 0 ; i < _numOrientation ; i++ ){
		Mat response = computeGaborResponse(_curOrientation);

		_orientationList.push_back(response);
		_curOrientation += orientationStep ; 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the gabor response. </summary>
///
/// <remarks>	Jihad, 09/12/2014. </remarks>
///
/// <param name="orientation">	The orientation. </param>
///
/// <returns>	The calculated gabor response. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Mat  FeatureExtractorBlob::computeGaborResponse(float orientation){
	double min, max ;
	Mat response, positive_response ;
	Ptr<FilterEngine> g = createGaussianFilter(0,Size(7,7),1,1,3);
	g.obj->apply(_image,response);
	//Mat kernel = GaussianKernel(Size(7,7),1, 1,3);
	//cv::filter2D(_image, response, CV_32F, kernel);
	//ImageTools::display("Kernel ", kernel);
	ImageTools::display("Image ", _image);
	ImageTools::display("Response ", response);
	waitKey() ;
	//minMaxLoc(response, &min, &max); 
	//response.convertTo(positive_response, CV_32F,1.0/(max-min), -min/(max-min));
	return response ;
}

void FeatureExtractorBlob::extract(vector<Feature*>& list){
	float orientationStep = 180.0f/_numOrientation ;
	float orientation = 0 ; 
	for ( int i = 0 ; i < _numOrientation ; i++ ){
		// Compute Gabor Response ;
		Mat response = computeGaborResponse(orientation);

		// Process the computed Reponse 
		DImage dimg(response);
		processResponse(dimg, orientation, list);
		orientation += orientationStep ; 
	}
}


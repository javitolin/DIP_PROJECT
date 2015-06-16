#include "stdafx.h"
#include "TextLineExtractorRafi.h"
#include "ComponentExtractorBinary.h"
#include "FilterAnisotropic.h"
#include "DImage.h"
#include "BinarizerOtsu.h"
using namespace cv;

TextLineExtractorRafi::TextLineExtractorRafi(){ ; }

BinaryComponentTree TextLineExtractorRafi::buildComponentTree(){

	Mat im_original = _image.clone();

	Mat im_filtered = filterDocument();
	
	BinaryComponentTree t;

	const int INTERVAL = 25;

	// Build Tree Structure;
	for (int i = 255; i > 0; i = i - INTERVAL){

		Mat floatGray, norm, res;
		im_filtered.convertTo(floatGray, CV_32FC1);

		vector<ConnectedComponent*> component_list;

		cv::normalize(floatGray, norm, 0, 255, NORM_MINMAX, CV_8UC1);

		threshold(norm, res, i, 255, CV_THRESH_BINARY_INV); // binarize.

		//namedWindow("THS " +to_string(i), CV_WINDOW_NORMAL);
		//imshow("THS " + to_string(i), res);
		
	
		DImage* binary_response = new DImage(res);

		// Extarct the connected components from the binary image 
		ComponentExtractorBinary component_extractor;
		binary_response->extractComponents(component_extractor, component_list);

		// Add new tree layer for each components in the given threshold:
		for (int j = 0; j < component_list.size(); j++){
			t.insert(new BinaryComponentNode(component_list[j]));
		}
		
	}

	return t;
}
//=================================================================
// Description: Traversing the tree to find lines.
// Return: vector<ConnectedComponent*> contains Lines.
//=================================================================
vector<ConnectedComponent*> TextLineExtractorRafi::TraverseTree() {
	
	BinaryComponentTree t = buildComponentTree();
	vector<ConnectedComponent*> result;

	FilterAnisotropic fl;
	Mat img = _image.clone();
	fl.setImage(new DImage(img));

	vector<double> wordSizeEst = fl.wordSizeEstimation();
	
	BinaryComponentNode* root = t.getRoot();
	
	recursiveTraversal(root, result, wordSizeEst[1]);

	img = fl.eraseMargins(img);

	for (int i = 0; i < result.size(); i++){
		Rect r = result[i]->getBoundRect();
		rectangle(img, r, 0, 4);
	}
	
	namedWindow("result", CV_WINDOW_NORMAL);
	imshow("result", img);
	return result;
}

//=========================================================================
// Description: Recursive Traversal Function.
//=========================================================================
void TextLineExtractorRafi::recursiveTraversal(BinaryComponentNode* current_node, vector<ConnectedComponent*>& result, double wordSizeEst){
	/*
	Mat current_mat = (current_node->getComponent())->getImage();
	vector<vector<Point>> edges;
	vector<Vec4i> hierarchy; 
	findContours(current_mat, edges, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	*/
	vector<BinaryComponentNode*> children = current_node->getChildren();
	Rect c = current_node->getComponent()->getBoundRect();
	if (c.height > 3*wordSizeEst){
		if (children.size() > 0){
			for (int i = 0; i < children.size(); i++){
				recursiveTraversal(children[i], result, wordSizeEst);
			}
		}
	}
	else result.push_back(current_node->getComponent());
}



//======================================================================
// Description: This function applys the Scale-Space Anisotropic filter.
// Return: A filtered image as Mat instance.
//=======================================================================
Mat TextLineExtractorRafi::filterDocument(){
	FilterAnisotropic fl;
	Mat img = _image.clone();
	DImage* tmp = new DImage(img);
	fl.setImage(tmp);
	DImage* filterd_img =  tmp->filter(fl);	
	return filterd_img->getMat();
}

//===================================================================================================================
//Description: This function extracts text lines form a page-
//Based on the article: "Using Scale-Space Anisotropic Smoothing for Text Line Extraction in Historical Documents."
//===================================================================================================================
void TextLineExtractorRafi::extract(){
	vector<ConnectedComponent*> result = TraverseTree();
}
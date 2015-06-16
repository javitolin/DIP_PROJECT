#include "stdafx.h"

#include "Feature.h"
#include "MetricDTW.h"


double  MetricDTW::distance(Feature* a, Feature* b){
		return a->distance(b) ;
}

double MetricDTW::distance(vector<Feature*>& a, vector<Feature*>& b){
	_dist_map.create(a.size(), b.size(), CV_64F);
	//computeDistanceMap(a,b);
	//findMatchPath();
	return 0.0; // only for compilation
}



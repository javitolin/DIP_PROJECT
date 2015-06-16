#ifndef _STDTLS_H_ 
#define _STDTLS_H_

#include <vector>
#include <assert.h>
#include <math.h>

using namespace std ;

namespace stdtls {
	template <typename T>
	vector<T> resample(vector<T>& array, unsigned int size){
		vector<T> sample ;
		assert(size > array.size());
		//_ASSERT(size > array.size());
		float step = (float)array.size()/double(size); 
		for ( unsigned int i = 0 ; i < size ; i++ ){
			int   lower = (int)(i*step) ;
			float delta = i*delta - lower; 
			sample[i] = (T)(array[lower]*(1-delta) + array[lower+1]*delta); 
		}
		return sample ;
	}

	template <typename T>
	double euclidean(vector<T>& a, vector<T>& b){
		T sum = 0 ;
		for ( unsigned int i = 0 ; i < a.size() ; i++ )
			sum += (a[i] - b[i]) * (a[i] - b[i]) ;

		return sqrt(sum); 
	}
}

#endif 

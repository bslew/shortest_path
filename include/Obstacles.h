/*
 * Obstacles.h
 *
 *  Created on: Jan 1, 2022
 *      Author: blew
 */

#ifndef OBSTACLES_H_
#define OBSTACLES_H_

#include <iostream>
#include <fstream>
#include <vector>

namespace minpath {
	
	/*
	 *
	 */
	template <class T>
	class Obstacles : public std::vector< std::vector<T> > {
		public:
			Obstacles();
			virtual ~Obstacles();
			Obstacles<T>& load(std::string fname);
			void mkBoundary(T margin=1);
			/*!
				\brief create list of obstacles in a shape of rectangular boundary
				\details 
				@param imin - minimal x coordinate at which boundary starts
				@param jmin - minimal y coordinate at which boundary starts
				@param imax - maximal x coordinate at which boundary ends
				@param jmax - maximal y coordinate at which boundary ends
				@return
			
				\date May 2, 2022, 10:58:40 AM
			*/
			void mkBoundary(T imin, T imax, T jmin, T jmax);
	};





} /* namespace minpath */



template <class T>
minpath::Obstacles<T>::Obstacles() : std::vector< std::vector<T> >() {
	// TODO Auto-generated constructor stub
	
}

template <class T>
minpath::Obstacles<T>::~Obstacles() {
	// TODO Auto-generated destructor stub
}

template <class T>
minpath::Obstacles<T>& minpath::Obstacles<T>::load(std::string fname) {
	std::ifstream ifs(fname);
	T x,y;
	long Nobst=0;
	while (ifs.good()) {
		ifs >> x >> y;
		std::vector<T> xy={x,y};
		this->push_back(xy);
		Nobst++;
	}
	ifs.close();
	return *this;
}


template <class T>
void minpath::Obstacles<T>::mkBoundary(T margin) {
		
}

template <class T>
void minpath::Obstacles<T>::mkBoundary(T imin, T imax, T jmin, T jmax) {
		for (T i=imin;i<=imax;i++) {
			std::vector<T> xy={i,jmin};
			this->push_back(xy);
			xy={i,jmax};
			this->push_back(xy);
		}
		for (T j=jmin+1;j<jmax;j++) {
			std::vector<T> xy={imin,j};
			this->push_back(xy);
			xy={imax,j};
			this->push_back(xy);
		}
		
}


#endif /* OBSTACLES_H_ */

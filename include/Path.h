/*
 * path.h
 *
 *  Created on: Jan 1, 2022
 *      Author: blew
 */

#ifndef PATH_H_
#define PATH_H_

#include <iostream>
#include <vector>

namespace minpath {
	
	/*
	 *
	 */
	class Path {
		public:
			Path(int x0=0, int y0=0, int xf=0, int yf=0, int verb=0);
			virtual ~Path();
			
//			void step(int x,int y);
			void step(int x,int y);
			
			void accept_step(int x,int y);
			bool rOK();
			bool lOK();
			bool uOK();
			bool dOK();
			void set_obstacles(std::vector< std::vector<int> >& obst);
//			void set_obstacles(std::vector< std::vector<long> >& obst);
			
			
			bool is_valid(int x, int y);
			void set_max_steps(long n) { max_steps=n; }
			
			void print_status();
			
		private:
			int x0,y0,xf,yf,x,y;
			long max_steps;
			long steps;
			bool solvable;
			bool walk_done;
			int verbosity;
			std::vector< std::vector<int> > obstacles;
	};

} /* namespace minpath */

#endif /* PATH_H_ */

/*
 * path.cpp
 *
 *  Created on: Jan 1, 2022
 *      Author: blew
 */

#include "Path.h"

namespace minpath {

Path::Path(int x0, int y0, int xf, int yf, int verb) {
    this->x = x0;
    this->y = y0;
    this->x0 = x0;
    this->y0 = y0;
    this->xf = xf;
    this->yf = yf;
    solvable = true;
    walk_done = false;
    max_steps = 10000;
    steps = 0;
    verbosity = verb;
}

Path::~Path() {}

/*
        void Path::step(int x, int y) {
                if (verbosity>0) {
                        std::cout << "step:" << step << "\n";
                }
                if (x==xf and y==yf) return;
                if (steps>=max_steps) { std::cout << "reached max steps\n";
   exit(0); } if (is_valid(x, y)) {

                        if (x<xf) { // need to go right
                                if (rOK()) { // can go right
                                        step(++x,y);
                                }
                                else { // cannot go right, let's try vertical
                                        if (y<yf) { // need to go up
                                                if (uOK()) { // can go up
                                                        step(x,++y);
                                                }
                                                else { // cannot go up
                                                        if (dOK()) { // try down
                                                                step(x,--y);
                                                        }
                                                        else { // cannot go down
                                                                if (lOK()) { //
   try left step(--x,y);
                                                                }
                                                                else { // cannot
   move solvable=false;
                                                                }
                                                        }
                                                }
                                        }
                                        else { // need to go down

                                        }

                                }
                        }
                }
                else {
                        return;
                }
        }
*/

void Path::step(int x, int y) {
    if (steps >= max_steps) {
        std::cout << "reached max steps\n";
        exit(0);
    }
    print_status();
    if (verbosity > 1) {
        std::cout << "going to x: " << x << ", y: " << y << "\n";
    }

    if (x == xf and y == yf) {
        accept_step(x, y);
        print_status();
        solvable = true;
        if (verbosity > 0) {
            std::cout << "walk done in " << --steps << " steps"
                      << "\n";
        }
        return;
    } else {
        if (is_valid(x, y) and walk_done == false) {
            accept_step(x, y);

            if (this->x < xf) { // need to go right
                step(++x, y);
            } else {
                if (this->x > xf)
                    step(--x, y);
            }

            if (this->y < yf) { // need to go up
                step(x, ++y);
            } else {
                if (this->y > yf)
                    step(x, --y);
            }
        } else {
            if (walk_done)
                return;

            return;
        }
    }
}

bool Path::rOK() {}

bool Path::lOK() {}

bool Path::uOK() {}

bool Path::dOK() {}

// void Path::set_obstacles(std::vector<std::vector<int> > &obst) {
// 	obstacles=obst;
// }
//	void Path::set_obstacles(std::vector<std::vector<long> > &obst) {
//		obstacles=obst;
//	}

bool Path::is_valid(int x, int y) {
    for (auto p : obstacles) {
        //			std::cout <<
        //p[0]<<","<<p[1]<<","<<x<<","<<y<<"\n";
        if (x == p[0] and y == p[1])
            return false;
    }
    return true;
}

void Path::accept_step(int x, int y) {
    this->x = x;
    this->y = y;
    steps++;
}

void Path::print_status() {
    if (verbosity > 0) {
        std::cout << "step:" << steps - 1 << ", x: " << this->x
                  << ", y: " << this->y << "\n";
    }
}

} /* namespace minpath */

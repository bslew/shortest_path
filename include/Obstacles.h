/*
 * Obstacles.h
 *
 *  Created on: Jan 1, 2022
 *      Author: blew
 */

#ifndef OBSTACLES_H_
#define OBSTACLES_H_

#include <fstream>
#include <iostream>
#include <vector>

namespace minpath {

template <class T> class Obstacles {
  public:
    Obstacles<T>& load(std::string fname);
    Obstacles<T>& add(T x, T y);
    size_t size();
    std::vector<std::vector<T>>& coords() { return _coords; }

  private:
    std::vector<std::vector<T>> _coords;
};

} /* namespace minpath */

template <class T>
minpath::Obstacles<T>& minpath::Obstacles<T>::load(std::string fname) {
    std::ifstream ifs(fname);
    T x, y;
    long Nobst = 0;
    while (ifs.good()) {
        ifs >> x >> y;
        std::vector<T> xy = {x, y};
        _coords.push_back(xy);
        Nobst++;
    }
    ifs.close();
    return *this;
}

template <class T>
inline minpath::Obstacles<T>& minpath::Obstacles<T>::add(T x, T y) {
    _coords.push_back({x, y});
    return *this;
}

template <class T> inline size_t minpath::Obstacles<T>::size() {
    return _coords.size();
}

#endif /* OBSTACLES_H_ */

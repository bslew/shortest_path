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
    Obstacles();
    Obstacles(const std::vector<std::vector<T>>& coords);
    Obstacles<T>& load(std::string fname);
    Obstacles<T>& add(T x, T y);
    size_t size();
    std::vector<std::vector<T>>& coords() { return _coords; }

  private:
    std::vector<std::vector<T>> _coords;
};

} /* namespace minpath */

template <class T> minpath::Obstacles<T>::Obstacles() {}

template <class T>
minpath::Obstacles<T>::Obstacles(const std::vector<std::vector<T>>& coords)
    : _coords(coords) {}

template <class T>
minpath::Obstacles<T>& minpath::Obstacles<T>::load(std::string fname) {
    std::ifstream ifs(fname);
    while (ifs.good()) {
        T x, y;
        if (ifs >> x >> y) {
            std::cout << x << " " << y << "\n";
            std::vector<T> xy = {x, y};
            _coords.push_back(xy);
        }
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

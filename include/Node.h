/*
 * node.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef NODE_H_
#define NODE_H_

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace minpath {

template <typename T> void printVec(std::vector<T> V, std::string title);
template <typename T> void printSet(std::set<T> S, std::string title);

template <class T> class Node;
template <typename T>
std::ostream& operator<<(std::ostream& out, Node<T> const& curr);

template <class T> class Node {
  public:
    typedef enum { regular, start, end } NodeType;

    // struct neighbor_t {
    //     size_t id = 0;
    //     T dist{1};
    //     const bool operator<(const neighbor_t& r) const { return (id < r.id);
    //     }
    // };

    typedef struct {
        std::map<size_t, T> neighbors;
        NodeType node_type = NodeType::regular;
        bool was_visited = false;
        T i{0}, j{0}; // node coordinates
        size_t id{0};
        T min_dist_from_start{0};
        size_t idPrevStep{0};
    } node_data_t;

    Node(size_t id = 0);
    virtual ~Node();
    void initialize(size_t id);
    void setStart() { _node.node_type = start; }
    void setEnd() { _node.node_type = end; }
    void setId(size_t id) { _node.id = id; }
    void setMinDistFromStart(T dist) { _node.min_dist_from_start = dist; }
    T getMinDistFromStart() const { return _node.min_dist_from_start; }
    void setPrevStepId(T id) { _node.idPrevStep = id; }
    T getPrevStepId() const { return _node.idPrevStep; }
    T getDist(const Node<T>& neighbor) const;
    size_t getNodeId() const { return _node.id; }
    void addNeighbour(Node<T>& n, T dist = 1);
    void addNeighbour(size_t id, T dist) { _node.neighbors[id] = dist; }
    void removeNeighbour(size_t id);
    bool isStart() const { return _node.node_type == start; }
    bool isEnd() const { return _node.node_type == end; }
    bool isRegular() const { return _node.node_type == regular; }
    bool wasVisited() const { return _node.was_visited; }
    void setVisited(bool v) { _node.was_visited = v; }
    std::vector<T> coords() { return {_node.i, _node.j}; }
    void print_info() {}
    const std::map<size_t, T>& getNeighbors() const { return _node.neighbors; }
    std::map<size_t, T>& neighbors() { return _node.neighbors; }
    node_data_t& data() { return _node; }
    const node_data_t& getNodeData() const { return _node; }

    node_data_t _node;
};

} /* namespace minpath */

template <class T> inline minpath::Node<T>::Node(size_t id) { initialize(id); }

template <class T> inline minpath::Node<T>::~Node() {
    // TODO Auto-generated destructor stub
}

template <class T> void minpath::Node<T>::addNeighbour(Node<T>& n, T dist) {
    this->_node.neighbors[n.getNodeId()] = dist;
}

template <class T> inline void minpath::Node<T>::removeNeighbour(size_t id) {
    _node.neighbors.erase(id);
}

template <class T>
inline T minpath::Node<T>::getDist(const Node<T>& neighbor) const {
    auto neighId = neighbor.getNodeId();
    const auto& neigh = getNeighbors();
    assert(neigh.contains(neighId));
    return neigh[neighId];
}

template <class T> inline void minpath::Node<T>::initialize(size_t id) {
    _node.id = id;
    _node.node_type = Node::regular;
    _node.was_visited = false;
}

template <typename T>
inline std::ostream& minpath::operator<<(std::ostream& out,
                                         const Node<T>& curr) {

    std::cout << "---------" << std::endl;
    std::cout << "Node> id: " << curr.getNodeData().id << std::endl;
    std::cout << "Node> coords: " << curr.getNodeData().i << ","
              << curr.getNodeData().j << std::endl;
    std::cout << "Node> min.path from start: " << curr.getMinDistFromStart()
              << std::endl;
    std::cout << "Node> Prev Step Node: " << curr.getPrevStepId() << std::endl;
    std::cout << "Node> was visited: " << curr.wasVisited() << std::endl;
    ;
    std::cout << "Node> is start: " << curr.isStart() << std::endl;
    ;
    std::cout << "Node> is end: " << curr.isEnd() << std::endl;
    ;
    std::cout << "Node> has " << curr.getNodeData().neighbors.size()
              << " neighbors" << std::endl;
    auto neigh = curr.getNodeData().neighbors;
    std::cout << "Node> neighbors distances: ";
    for (auto& [id, dist] : neigh) {
        std::cout << id << "(" << dist << "), ";
    }
    std::cout << std::endl;
    std::cout << "---------" << std::endl;

    return out;
}

template <typename T>
void minpath::printVec(std::vector<T> V, std::string title) {
    std::cout << title << "\n";
    for (auto v : V) {
        std::cout << v << ", ";
    }
    std::cout << "\n";
}

template <typename T> void minpath::printSet(std::set<T> S, std::string title) {
    std::vector<T> V(S.begin(), S.end());
    printVec(V, title);
}

#endif /* NODE_H_ */

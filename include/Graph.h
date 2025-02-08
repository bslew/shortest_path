/*
 * Graph.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "Node.h"
#include "Obstacles.h"
#include <algorithm>
#include <assert.h>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <unordered_set>

namespace minpath {

template <class T> class Graph;
template <typename T>
std::ostream& operator<<(std::ostream& out, Graph<T> const& curr);

/*
 *
 */
template <class T> class Graph {
  public:
    Graph(long Nnodes = 0);
    virtual ~Graph();

    void addNode(minpath::Node<T>& node);

    /*!
            \brief return set of
            \details
            @param
            @return

            \date Jun 2, 2022, 2:25:43 PM
     */
    std::unordered_set<size_t> getUnvisitedNodes();
    std::map<size_t, minpath::Node<T>>& nodes() { return _nodes; }
    std::map<size_t, minpath::Node<T>> getNodes() const { return this->_nodes; }
    unsigned long getNodesNum() const { return this->getNodes().size(); }
    void build() {}
    minpath::Node<T>& node(size_t index) { return _nodes[index]; }

    /*!
            \brief set all nodes as unvisited and set their distances to initial
       node to infinity \details

            \date May 12, 2022, 1:53:47 PM
     */
    void initializeGraph();
    size_t getStartNodeId();
    size_t getStopNodeId();
    /*!
            \brief get a set of unvisited neighbors while updating distances to
       nodes in the graph \details
            @param cur - pointer of the node for which we want to obtain a set
       of unvisited neighbors
            @return set of unvisited neighbors.

            \date May 12, 2022, 1:50:20 PM
     */

    std::map<size_t, T> unvisitedNeighbors(minpath::Node<T>& cur);

    T get_min_dist(std::unordered_set<size_t>& nodes);
    minpath::Node<T> get_min_dist_node(std::unordered_set<size_t>& nodes);

    std::vector<size_t> getNodesAtDistance(T dist);

    T solve();
    T solveMinimalStepsCount();

    std::map<size_t, minpath::Node<T>> _nodes;
    static constexpr T max_dist = std::numeric_limits<T>::max();
};

/*!
        \brief class that initializes the underlying graph as rectangular field
        \details
        The graph connections have unit distances and each rectangular field has
        horizontal and vertical connections to neighbors unless the cells are
        at the field boundary.

        \date May 12, 2022, 11:47:01 AM
 */
template <class T> class FieldGraph : public Graph<T> {
  public:
    FieldGraph() : Graph<T>(){};
    FieldGraph(T ist, T jst, T ien, T jen, T imin, T imax, T jmin, T jmax,
               Obstacles<T>* obst);
    //			void build(T imin, T imax, T jmin, T jmax, Obstacles<T>&
    // obst);
    void build();
    std::list<minpath::Node<T>*> get_linked_nodes(T i, T j);

    T imin, imax, jmin, jmax, ist, jst, ien, jen; // coordinate space
    Obstacles<T>* obst;
};

} /* namespace minpath */

/* ********************************************************************************************
 */
/* ********************************************************************************************
 */
/* ********************************************************************************************
 */
/* ********************************************************************************************
 */
template <class T> minpath::Graph<T>::Graph(long nNodes) {}

template <class T>
inline minpath::FieldGraph<T>::FieldGraph(T ist, T jst, T ien, T jen, T imin,
                                          T imax, T jmin, T jmax,
                                          Obstacles<T>* obst) {
    this->ist = ist;
    this->jst = jst;
    this->ien = ien;
    this->jen = jen;

    this->imin = imin;
    this->imax = imax;
    this->jmin = jmin;
    this->jmax = jmax;

    this->obst = obst;
    build();
}

template <class T> minpath::Graph<T>::~Graph() {}

template <class T> inline void minpath::FieldGraph<T>::build() {
    typename minpath::Node<T> n;

    // allocate memory for all graph nodes
    // i - iterates columns in the field
    // j - iterates rows in the field
    T Ncols = imax - imin + 1;
    T Nrows = jmax - jmin + 1;
    std::vector<std::vector<minpath::Node<T>>> field(
        Ncols); // first coord iterates column
    std::vector<minpath::Node<T>> field_col(Nrows);
    // initialize graph distances, ids and coordinates
    for (long i = 0; i < Ncols; i++) {
        field[i] = field_col;
        for (long j = 0; j < Nrows; j++) {
            field[i][j].data().min_dist_from_start =
                minpath::Graph<T>::max_dist;
            field[i][j].setId(i * Nrows + j);
            field[i][j].data().i = i + imin;
            field[i][j].data().j = j + jmin;
            if (i + imin == ist and j + jmin == jst)
                field[i][j].setStart();
            if (i + imin == ien and j + jmin == jen)
                field[i][j].setEnd();
        }
    }

    //
    // make connections
    //

    // lower left corner
    field[0][0].addNeighbour(field[0][1]);
    field[0][0].addNeighbour(field[1][0]);
    field[0][0].addNeighbour(field[1][1]);

    // lower right corner
    field[Ncols - 1][0].addNeighbour(field[Ncols - 2][0]);
    field[Ncols - 1][0].addNeighbour(field[Ncols - 1][1]);

    // upper left corner
    field[0][Nrows - 1].addNeighbour(field[1][Nrows - 1]);
    field[0][Nrows - 1].addNeighbour(field[0][Nrows - 2]);

    // upper right corner
    field[Ncols - 1][Nrows - 1].addNeighbour(field[Ncols - 2][Nrows - 1]);
    field[Ncols - 1][Nrows - 1].addNeighbour(field[Ncols - 1][Nrows - 2]);

    // bottom line
    for (long i = 1; i < Ncols - 1; i++) {
        field[i][0].addNeighbour(field[i - 1][0]);
        field[i][0].addNeighbour(field[i + 1][0]);
        field[i][0].addNeighbour(field[i][1]);
    }

    // top line
    for (long i = 1; i < Ncols - 1; i++) {
        field[i][Nrows - 1].addNeighbour(field[i - 1][Nrows - 1]);
        field[i][Nrows - 1].addNeighbour(field[i + 1][Nrows - 1]);
        field[i][Nrows - 1].addNeighbour(field[i][Nrows - 2]);
    }

    // left line
    for (long j = 1; j < Nrows - 1; j++) {
        field[0][j].addNeighbour(field[0][j - 1]);
        field[0][j].addNeighbour(field[0][j + 1]);
        field[0][j].addNeighbour(field[1][j]);
    }

    // right line
    for (long j = 1; j < Nrows - 1; j++) {
        field[Ncols - 1][j].addNeighbour(field[Ncols - 1][j - 1]);
        field[Ncols - 1][j].addNeighbour(field[Ncols - 1][j + 1]);
        field[Ncols - 1][j].addNeighbour(field[Ncols - 2][j]);
    }

    // inside nodes
    for (long i = 1; i < Ncols - 1; i++) {
        for (long j = 1; j < Nrows - 1; j++) {
            field[i][j].addNeighbour(field[i - 1][j]);
            field[i][j].addNeighbour(field[i + 1][j]);
            field[i][j].addNeighbour(field[i][j - 1]);
            field[i][j].addNeighbour(field[i][j + 1]);
        }
    }

    for (long i = 0; i < Ncols; i++) {
        for (long j = 0; j < Nrows; j++) {
            Graph<T>::addNode(field[i][j]);
        }
    }

    if (this->obst != 0) {
        std::cout << "Warning: currently we do not support building graphs "
                     "with obstacles (ignoring)"
                  << std::endl;
    }
}
/* ********************************************************************************************
 */
template <class T> void minpath::Graph<T>::addNode(minpath::Node<T>& node) {
    this->_nodes[node.getNodeId()] = node;
}
/* ********************************************************************************************
 */
template <class T>
inline std::list<minpath::Node<T>*>
minpath::FieldGraph<T>::get_linked_nodes(T i, T j) {
    std::list<minpath::Node<T>*> neigh;
    return neigh;
}
/* ********************************************************************************************
 */
template <typename T>
inline std::ostream& minpath::operator<<(std::ostream& out,
                                         const Graph<T>& curr) {

    unsigned long Nnodes = curr.getNodesNum();
    std::cout << "Graph has " << Nnodes << " nodes." << std::endl;
    for (auto [_, node] : curr.getNodes()) {
        std::cout << "NODE" << std::endl;
        std::cout << node;
        std::cout << std::endl;
    }
    return out;
}

template <class T> inline void minpath::Graph<T>::initializeGraph() {

    // initialize graph distances and other data
    long int id = 0;
    for (auto& [_, n] : _nodes) {

        if (n.isStart()) {
            n.data().min_dist_from_start = 0;
        } else {
            n.data().min_dist_from_start = max_dist;
        }

        n.data().was_visited = false;
    }
}

template <class T> inline size_t minpath::Graph<T>::getStartNodeId() {
    for (auto& [id, n] : nodes()) {
        if (n.isStart()) {
            return n.getNodeId();
        }
    }
    return -1;
}

template <class T>
inline std::map<size_t, T>
minpath::Graph<T>::unvisitedNeighbors(minpath::Node<T>& cur) {
    std::map<size_t, T> uv;

    for (auto& [id, dist] : cur.neighbors()) {
        if (!node(id).wasVisited()) {
            uv[id] = dist;
        }
    }

    return uv;
}

template <class T>
inline std::unordered_set<size_t> minpath::Graph<T>::getUnvisitedNodes() {
    std::unordered_set<size_t> unvisited_nodes;

    for (auto& [_, n] : this->nodes()) {
        if (!n.wasVisited()) {
            unvisited_nodes.insert(n.getNodeId());
        }
    }

    return unvisited_nodes;
}

template <class T> inline size_t minpath::Graph<T>::getStopNodeId() {
    for (auto& [_, n] : this->nodes()) {
        if (n.isEnd()) {
            return n.getNodeId();
        }
    }
    return -1;
}
/* ********************************************************************************************
 */
template <class T>
inline T minpath::Graph<T>::get_min_dist(std::unordered_set<size_t>& nodes) {
    auto n = get_min_dist_node(nodes);
    return n.getMinDistFromStart();
}
/* ********************************************************************************************
 */
template <class T>
inline minpath::Node<T>
minpath::Graph<T>::get_min_dist_node(std::unordered_set<size_t>& nodesIds) {

    assert(_nodes.size() >= 0);

    size_t minDistanceNodeId = 0;
    T minDistance = node(minDistanceNodeId).getMinDistFromStart();
    for (auto idx : nodesIds) {
        if (node(idx).getMinDistFromStart() < minDistance) {
            minDistanceNodeId = idx;
            minDistance = node(minDistanceNodeId).getMinDistFromStart();
        }
    }
    return node(minDistanceNodeId);
}

template <class T>
inline std::vector<size_t> minpath::Graph<T>::getNodesAtDistance(T dist) {
    std::vector<size_t> nodesAtRequestedDistance;
    for (auto [id, node] : getNodes()) {
        if (node.getMinDistFromStart() == dist) {
            nodesAtRequestedDistance.push_back(id);
        }
    }
    return nodesAtRequestedDistance;
}

/* ********************************************************************************************
 */
template <class T> inline T minpath::Graph<T>::solve() {

    initializeGraph();

    // std::cout << "initialized graph\n" << *this << "\n";

    // auto unvisited_nodes = getUnvisitedNodes();
    // unsigned startNodeIdx = getStartNodeId();
    // minpath::Node<T> cur = node(startNodeIdx);
    // minpath::Node<T> endNode = node(getStopNodeId());

    // bool unvisitedMinDistEqInf = false;
    //
    // while (!endNode.wasVisited() && !unvisitedMinDistEqInf) {

    //     // get set of unvisited neighbors
    //     // and on the graph update their distances from the starting node
    //     auto unvisitedNeigh = unvisitedNeighbors(cur);

    //     cur.setVisited(true);
    //     if (!cur.isEnd()) {
    //         unvisited_nodes.erase(cur.getNodeId());
    //         cur = get_min_dist_node(unvisitedNeigh);
    //         unvisitedMinDistEqInf =
    //             get_min_dist(unvisited_nodes) == max_dist;
    //     }
    //     std::cout << "Current node:\n " << cur << "\n";
    // }
    // return cur.getMinDistFromStart();
    return 0;
}

template <class T> inline T minpath::Graph<T>::solveMinimalStepsCount() {

    initializeGraph();

    std::cout << "initialized graph\n"
              << "\n";

    auto unvisited_nodes = getUnvisitedNodes();
    auto startNodeIdx = getStartNodeId();
    minpath::Node<T>& cur = node(startNodeIdx);
    auto curDist = cur.getMinDistFromStart();
    auto newDist = curDist;
    minpath::Node<T>& endNode = node(getStopNodeId());

    // bool unvisitedMinDistEqInf = false;
    //
    std::vector<size_t> nodesToVisit = {startNodeIdx};
    while (!endNode.wasVisited() && !unvisited_nodes.empty()) {

        std::vector<T> distances;
        for (auto curId : nodesToVisit) {
            auto& cur = node(curId);
            std::cout << "Current node:\n " << cur << "\n";
            cur.setVisited(true);

            for (auto& [nId, dist] : cur.neighbors()) {
                auto& nNode = node(nId);
                if (!nNode.wasVisited()) {
                    distances.push_back(curDist + dist);
                    nNode.setMinDistFromStart(curDist + dist);
                    nNode.setVisited(true);
                    std::cout << "Current neighbor:\n " << nNode << "\n";
                }
            }
        }
        curDist = *std::min_element(distances.begin(), distances.end());

        // find nodes to visit
        std::cout << "Processing nodes at distance " << curDist << "\n";

        nodesToVisit = getNodesAtDistance(curDist);

        if (endNode.wasVisited()) {
            std::cout << "End node visited..stopping walk\n";
            break;
        }
    }
    return curDist;
}

#endif /* GRAPH_H_ */

/*
 * Graph.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "Logger.h"
#include "Node.h"
#include "Obstacles.h"
#include <algorithm>
#include <assert.h>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <unordered_set>
#include <vector>

namespace minpath {

template <class T> class Graph;
template <typename T>
std::ostream& operator<<(std::ostream& out, Graph<T> const& curr);
template <class T> class Graph {
  public:
    Graph(minpath::Logger logger = minpath::getLogger());
    virtual ~Graph();

    void addNode(const minpath::Node<T>& node);
    void addNodes(const std::vector<minpath::Node<T>>& nodes);
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
    minpath::Node<T>& node(size_t id) { return _nodes[id]; }

    /*!
    \brief set all nodes as unvisited and set their distances to initial
    node to infinity \details

    \date May 12, 2022, 1:53:47 PM
     */
    void initializeGraph();
    size_t getStartNodeId();
    size_t getStopNodeId();

    /*!
    \brief get a set of unvisited neighbors while updating distances to nodes in
    the graph \details
    @param cur - pointer of the node for which we want to obtain a set of
    unvisited neighbors
    @return set of unvisited neighbors.

    \date May 12, 2022, 1:50:20 PM
     */
    std::map<size_t, T> unvisitedNeighbors(minpath::Node<T>& cur);

    T get_min_dist(std::unordered_set<size_t>& nodes);
    minpath::Node<T> get_min_dist_node(std::unordered_set<size_t>& nodes);

    std::set<size_t> getNodesAtDistance(T dist);

    std::optional<T> solveMinimalDistance();
    std::vector<size_t> getShortestPathNodes();
    void dumpGraph(std::string dumpFile);
    void dumpShortestPath(std::string dumpFile);
    // T solveMinimalStepsCount();

    std::map<size_t, minpath::Node<T>> _nodes;
    static constexpr T max_dist = std::numeric_limits<T>::max();
    minpath::Logger _logger;
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
    FieldGraph(minpath::Logger logger = minpath::getLogger())
        : Graph<T>(logger){};
    FieldGraph(T ist, T jst, T ien, T jen, T imin, T imax, T jmin, T jmax,
               const Obstacles<T>& obstacles, bool diagonal = false,
               minpath::Logger logger = minpath::getLogger());
    void build(bool diagonal = false);
    std::list<minpath::Node<T>*> get_linked_nodes(T i, T j);

    T imin, imax, jmin, jmax, ist, jst, ien, jen; // coordinate space
    Obstacles<T> _obstacles;
};

} /* namespace minpath */

template <class T>
minpath::Graph<T>::Graph(minpath::Logger logger) : _logger(logger) {}

template <class T>
inline minpath::FieldGraph<T>::FieldGraph(T ist, T jst, T ien, T jen, T imin,
                                          T imax, T jmin, T jmax,
                                          const Obstacles<T>& obstacles,
                                          bool diagonal, minpath::Logger logger)
    : Graph<T>(logger) {
    this->ist = ist;
    this->jst = jst;
    this->ien = ien;
    this->jen = jen;

    this->imin = imin;
    this->imax = imax;
    this->jmin = jmin;
    this->jmax = jmax;

    this->_obstacles = obstacles;
    build(diagonal);
}

template <class T> minpath::Graph<T>::~Graph() {}

template <class T> inline void minpath::FieldGraph<T>::build(bool diagonal) {
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
    for (long i = 0; i < Ncols; i++) {
        for (long j = 0; j < Nrows; j++) {
            if (i > 0) {
                field[i][j].addNeighbour(field[i - 1][j]);
            }
            if (i < Ncols - 1) {
                field[i][j].addNeighbour(field[i + 1][j]);
            }
            if (j > 0) {
                field[i][j].addNeighbour(field[i][j - 1]);
            }
            if (j < Nrows - 1) {
                field[i][j].addNeighbour(field[i][j + 1]);
            }
            if (diagonal) {
                if (i > 0 && j < Nrows - 1) {
                    field[i][j].addNeighbour(field[i - 1][j + 1]);
                }
                if (i < Ncols - 1 && j < Nrows - 1) {
                    field[i][j].addNeighbour(field[i + 1][j + 1]);
                }
                if (i < Ncols - 1 && j > 0) {
                    field[i][j].addNeighbour(field[i + 1][j - 1]);
                }
                if (i > 0 && j > 0) {
                    field[i][j].addNeighbour(field[i - 1][j - 1]);
                }
            }
        }
    }

    // remove connections between nodes due to obstacles
    for (auto& xy : _obstacles.coords()) {
        auto i = xy[0] - imin;
        auto j = xy[1] - jmin;

        if (i < 0 || i >= Ncols || j < 0 || j >= Nrows) {
            this->_logger.warn(
                "Obstacle out of field bounds: {0}, {1}. Ignoring.", i, j);
            continue;
        }
        auto removedNodeId = field[i][j].getNodeId();
        // remove left-right connections
        if (i > 0) {
            field[i - 1][j].removeNeighbour(removedNodeId);
        }
        if (i < Ncols - 1) {
            field[i + 1][j].removeNeighbour(removedNodeId);
        }
        // remove top-down connections
        if (j > 0) {
            field[i][j - 1].removeNeighbour(removedNodeId);
        }
        if (j < Nrows - 1) {
            field[i][j + 1].removeNeighbour(removedNodeId);
        }
        if (diagonal) {
            // remove diagonal connections
            if (i > 0 && j > 0) {
                field[i - 1][j - 1].removeNeighbour(removedNodeId);
            }
            if (i < Ncols - 1 && j < Nrows - 1) {
                field[i + 1][j + 1].removeNeighbour(removedNodeId);
            }
            // remove diagonal connections
            if (i < Ncols - 1 && j > 0) {
                field[i + 1][j - 1].removeNeighbour(removedNodeId);
            }
            if (i > 0 && j < Nrows - 1) {
                field[i - 1][j + 1].removeNeighbour(removedNodeId);
            }
        }
        // remove connections for obstacle node
        field[i][j].neighbors().clear();
    }

    // add nodes to graph
    for (long i = 0; i < Ncols; i++) {
        for (long j = 0; j < Nrows; j++) {
            Graph<T>::addNode(field[i][j]);
        }
    }
}

template <class T>
void minpath::Graph<T>::addNode(const minpath::Node<T>& node) {
    this->_nodes[node.getNodeId()] = node;
}

template <class T>
void minpath::Graph<T>::addNodes(const std::vector<minpath::Node<T>>& nodes) {
    for (const auto& node : nodes) {
        addNode(node);
    }
}

template <class T>
inline std::list<minpath::Node<T>*>
minpath::FieldGraph<T>::get_linked_nodes(T i, T j) {
    std::list<minpath::Node<T>*> neigh;
    return neigh;
}

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

template <class T>
inline T minpath::Graph<T>::get_min_dist(std::unordered_set<size_t>& nodes) {
    auto n = get_min_dist_node(nodes);
    return n.getMinDistFromStart();
}

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
inline std::set<size_t> minpath::Graph<T>::getNodesAtDistance(T dist) {
    std::set<size_t> nodesAtRequestedDistance;
    for (auto [id, node] : getNodes()) {
        if (node.getMinDistFromStart() == dist) {
            nodesAtRequestedDistance.insert(id);
        }
    }
    return nodesAtRequestedDistance;
}

template <class T>
inline std::optional<T> minpath::Graph<T>::solveMinimalDistance() {

    initializeGraph();

    _logger.debug("initialized graph");

    auto startNodeIdx = getStartNodeId();
    auto curDist = node(startNodeIdx).getMinDistFromStart();
    minpath::Node<T>& endNode = node(getStopNodeId());
    std::set<size_t> nodesToVisit = {startNodeIdx};
    std::set<T> distances;

    while (!nodesToVisit.empty()) {

        for (auto curId : nodesToVisit) {
            auto& cur = node(curId);
            cur.setVisited(true);
            if (_logger.should_log(spdlog::level::level_enum::trace)) {
                std::cout << "Current node:\n " << cur << "\n";
            }

            for (auto& [nId, dist] : cur.neighbors()) {
                auto& nNode = node(nId);
                if (!nNode.wasVisited()) {
                    nNode.setMinDistFromStart(curDist + dist);
                    nNode.setPrevStepId(curId);
                    nNode.setVisited(true);
                    distances.insert(curDist + dist);
                } else {
                    if (curDist + dist < nNode.getMinDistFromStart()) {
                        nNode.setMinDistFromStart(curDist + dist);
                        nNode.setPrevStepId(curId);
                        distances.insert(curDist + dist);
                    }
                }
                if (_logger.should_log(spdlog::level::level_enum::trace)) {
                    std::cout << "Current neighbor:\n " << nNode << "\n";
                }
            }
        }
        nodesToVisit.clear();
        if (!distances.empty()) {
            if (_logger.should_log(spdlog::level::level_enum::trace)) {
                printSet<T>(distances, "distances");
            }

            curDist = *distances.begin();
            _logger.trace("Will process nodes at distance: {0}", curDist);

            // find nodes to visit
            nodesToVisit = getNodesAtDistance(curDist);

            if (_logger.should_log(spdlog::level::level_enum::trace)) {
                printSet<size_t>(nodesToVisit, "nodesToVisit");
            }
        }

        distances.erase(curDist);

        if (endNode.wasVisited() && curDist >= endNode.getMinDistFromStart()) {
            _logger.debug(
                "End node visited and distances from other nodes are larger "
                "than the path already found ({0}), stopping walk",
                curDist);
            break;
        }
    }

    if (endNode.wasVisited()) {
        return endNode.getMinDistFromStart();
    }
    return std::nullopt;
}

template <class T>
inline std::vector<size_t> minpath::Graph<T>::getShortestPathNodes() {
    std::vector<size_t> path;
    auto endNode = getStopNodeId();
    auto nodeId = node(endNode).getNodeId();

    while (nodeId != getStartNodeId()) {
        path.push_back(nodeId);
        nodeId = node(nodeId).getPrevStepId();
    }
    path.push_back(nodeId);
    return std::vector(path.rbegin(), path.rend());
}

template <class T>
inline void minpath::Graph<T>::dumpGraph(std::string dumpFile) {
    std::ofstream ofsN(dumpFile + ".nodes");
    std::ofstream ofsD(dumpFile + ".dist");
    assert(ofsN.is_open());
    assert(ofsD.is_open());
    for (auto [id, node] : nodes()) {
        ofsN << id << " ";
        ofsD << id << " ";
        for (auto [neigh, dist] : node.neighbors()) {
            ofsN << neigh << " ";
            ofsD << dist << " ";
        }
        ofsN << std::endl;
        ofsD << std::endl;
    }
    ofsN.close();
    ofsD.close();

    // dump coordinates
    std::ofstream ofs(dumpFile + ".coords");
    assert(ofs.is_open());
    for (auto [id, node] : nodes()) {
        ofs << id << " ";
        for (auto coord : node.coords()) {
            ofs << coord << " ";
        }
        ofs << std::endl;
    }
    ofs.close();
}

template <class T>
inline void minpath::Graph<T>::dumpShortestPath(std::string dumpFile) {
    std::ofstream ofsN(dumpFile + ".nodes");
    auto shortestPath = getShortestPathNodes();
    assert(ofsN.is_open());
    for (auto id : shortestPath) {
        ofsN << id << std::endl;
    }
    ofsN.close();

    // dump coordinates
    std::ofstream ofs(dumpFile + ".coords");
    assert(ofs.is_open());
    for (auto id : shortestPath) {
        for (auto coord : node(id).coords()) {
            ofs << coord << " ";
        }
        ofs << std::endl;
    }
    ofs.close();
}

#endif /* GRAPH_H_ */

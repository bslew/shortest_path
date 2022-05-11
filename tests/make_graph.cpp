#include "Graph.h"
#include <iostream>

/**
 * @brief Example program showing how to create graphs using nodes.
 *
 */
int main(int argc, char** argv) {

    minpath::Graph<int> G;

    // create node with id 1
    minpath::Node<int> n1(1);
    minpath::Node<int> n2(2);
    minpath::Node<int> n3(3);

    // Indicate that n1 has neighbour node 2 at distance 1
    // This does not automatically imply that node with id2 has neighbour with
    // id1
    n1.addNeighbour(2, 1);
    // indicate that n1 has neighbour node 3 at distance 2
    n1.addNeighbour(3, 2);
    std::cout << n1 << "\n";

    n3.addNeighbour(n2, 2);

    // Add nodes to graph
    G.addNode(n1);
    G.addNode(n2);
    G.addNode(n3);
    std::cout << "Graph\n" << G << "\n";

    return 0;
}

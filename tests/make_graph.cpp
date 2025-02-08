#include "Graph.h"
#include <iostream>

int main(int argc, char** argv) {

    minpath::Graph<int> G;

    minpath::Node<int> n1(1);
    minpath::Node<int> n2(2);
    minpath::Node<int> n3(3);

    n1.addNeighbour(2, 1);
    n1.addNeighbour(3, 2);
    std::cout << n1 << "\n";

    n3.addNeighbour(n2, 2);

    G.addNode(n1);
    G.addNode(n2);
    G.addNode(n3);
    std::cout << "Graph\n" << G << "\n";

    return 0;
}

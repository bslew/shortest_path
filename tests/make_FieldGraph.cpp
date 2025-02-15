#include "Graph.h"
#include <iostream>

/**
 * @brief Example program to create graph that can be interpreted as rectangular
 * grid of nodes having connections to closest neighboring nodes along cartesian
 * coordinates. The nodes can have assigned coordinates.
 */
int main(int argc, char** argv) {
    // Obstacles remove connections between graph nodes
    minpath::Obstacles<int> obstacles;
    // remove connections to and from the node at coordinates (1,1)
    obstacles.add(1, 1);

    // Create graph with nodes at rectangular grid spanning x,y coordinates
    // [0, 2]x[0, 3]. Indicate node at coordinates (0, 0) as initial node, and
    // node at coordinates (2,1) as the final node.
    bool diagonal = false; // do not create diagonal connections between nodes
    minpath::FieldGraph<int> F(0, 0, 2, 1, 0, 2, 0, 3, obstacles, diagonal);

    std::cout << "Graph\n" << F << "\n";

    return 0;
}

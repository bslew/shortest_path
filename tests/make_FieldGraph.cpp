#include "Graph.h"
#include <iostream>

int main(int argc, char** argv) {
    minpath::Obstacles<int> obst;
    minpath::FieldGraph<int> F(0, 0, 2, 2, 0, 2, 0, 3, obst);

    std::cout << "Graph\n" << F << "\n";

    return 0;
}

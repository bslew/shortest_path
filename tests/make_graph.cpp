#include <iostream>
#include "Graph.h"

int main(int argc, char** argv) {
	
	{
		minpath::Graph<int> G;
		
		minpath::Node<int> n1;
		minpath::Node<int> n2;
		minpath::Node<int> n3;
		
		minpath::Node<int>::neighbor_t n;
		
		n.neigh_node=&n2;
		n.dist=1;
		
		n1.addNeighbour(n);

		n.neigh_node=&n3;
		n.dist=2;
		n1.addNeighbour(n);
		std::cout << n1 << "\n";
//		std::cout << n1 << "\n";
		
		G.addNode(n1);
		std::cout << "Graph\n" << G << "\n";
	}
	

	return 0;
}

#include <iostream>
#include "Graph.h"

int main(int argc, char** argv) {
	
	{
		minpath::FieldGraph<int> F(0,0,2,2,0,2,0,3,0);
		
		std::cout << "Graph\n" << F << "\n";
	}
	

	return 0;
}

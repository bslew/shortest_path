/*
 * node.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef NODE_H_
#define NODE_H_

#include <list>
#include <iostream>
#include <fstream>

namespace minpath {

	template <class T> class Node;
	template <typename T> std::ostream& operator<< (std::ostream& out, Node<T> const& curr);

	/*
	 *
	 */
	template <class T>
	class Node {
		public:
			typedef enum {
				regular,start,end
			} node_t;

			typedef struct {
				Node<T>* neigh_node;
				T dist;
			} neighbor_t;
			
			typedef struct {
				std::list< neighbor_t > neigbours;
				node_t node_type;
				bool was_visited;
				T i,j; // node coordinates
				long id;
				T min_dist_from_start;
			} node_data_t;

			
			Node();
			virtual ~Node();
			void initialize();
			void setStart() { _node.node_type=start; }
			void setEnd() { _node.node_type=end; }

			void addNeighbour(neighbor_t& n);
			bool isStart() const { return _node.node_type==start; }
			bool isEnd() const { return _node.node_type==end; }
			bool isRegular() const  { return _node.node_type==regular; }
			void print_info() {
				
			}
			
			node_data_t& data() { return _node; }
			const node_data_t& getNodeData() const {
				return _node;
			}

			node_data_t _node;
	};

} /* namespace minpath */


template <class T>
inline minpath::Node<T>::Node() {
	initialize();
}

template <class T>
inline minpath::Node<T>::~Node() {
	// TODO Auto-generated destructor stub
}

template <class T>
void minpath::Node<T>::addNeighbour(Node<T>::neighbor_t& n) {
	this->_node.neigbours.push_back(n);
}

template <class T>
inline void minpath::Node<T>::initialize() {
	_node.id=0;
	_node.node_type=Node::regular;
	_node.was_visited=false;

}


template <typename T>
inline std::ostream& minpath::operator <<(std::ostream &out,
		const Node<T> &curr) {
	
	std::cout << "---------" << std::endl;
	std::cout << "Node> id: " << curr.getNodeData().id << std::endl;
	std::cout << "Node> coords: " << curr.getNodeData().i << "," << curr.getNodeData().j << std::endl;
	std::cout << "Node> was visited: " << curr.getNodeData().was_visited << std::endl;;
	std::cout << "Node> is start: " << curr.isStart() << std::endl;;
	std::cout << "Node> is end: " << curr.isEnd() << std::endl;;
	std::cout << "Node> has " << curr.getNodeData().neigbours.size() << " neighbors" << std::endl;
	auto neigh=curr.getNodeData().neigbours;
	std::cout << "Node> neighbors distances: ";
	for (auto& n : neigh) {
		std::cout << n.dist << ",";
	}
	std::cout << std::endl;
	std::cout << "---------" << std::endl;

	return out;
}

#endif /* NODE_H_ */

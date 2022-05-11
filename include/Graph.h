/*
 * Graph.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <assert.h>
#include "Node.h"
#include "Obstacles.h"

namespace minpath {

	template <class T> class Graph;
	template <typename T> std::ostream& operator<< (std::ostream& out, Graph<T> const& curr);

	/*
	 *
	 */
	template <class T>
	class Graph {
		public:
			Graph(long Nnodes=0);
			virtual ~Graph();
			
			void addNode(minpath::Node<T> node
//					std::list<minpath::Node*> linked_nodes=std::list<minpath::Node*>(),
//					std::list<T> link_lenghts=std::list<T>()
					);
//			void addNode(minpath::Node node);
			
			std::list< minpath::Node<T> >& nodes() { return this->node;}
			const std::list< minpath::Node<T> > getNodes() const { return this->node;}
			unsigned long getNodesNum() const { return this->node.size(); }
//			std::list<minpath::Node*>& neighbors(long i) { return this->linked_nodes[i]; }
//			std::list<T>& dist(unsigned long i) { return this->link_lengths[i]; }
			void build() {}
			void solve();
			
//			std::list< std::list<T> > link_lengths;
//			std::list< std::list<minpath::Node*> > linked_nodes;
			std::list< minpath::Node<T> > node;
	};

	template <class T>
	class FieldGraph : public Graph<T> {
		public:
			FieldGraph() :Graph<T>() {};
			FieldGraph(T ist, T jst, T ien, T jen,
					T imin, T imax, T jmin, T jmax, 
					Obstacles<T>* obst);
//			void build(T imin, T imax, T jmin, T jmax, Obstacles<T>& obst);
			void build();
			std::list<minpath::Node<T>*> get_linked_nodes(T i,T j);
			
			T imin, imax, jmin, jmax, ist, jst, ien, jen;
			Obstacles<T>* obst;
	};
	
} /* namespace minpath */

template <class T>
minpath::Graph<T>::Graph(long Nnodes) :
//	adj(Nnodes, std::list<T>() ),
	node(Nnodes, minpath::Node<T>() ) 
{
}

template<class T>
inline minpath::FieldGraph<T>::FieldGraph(T ist, T jst, T ien, T jen, 
		T imin, T imax, T jmin, T jmax, 
		Obstacles<T>* obst) {
		this->ist=ist;
		this->jst=jst;
		this->ien=ien;
		this->jen=jen;
		
		this->imin=imin;
		this->imax=imax;
		this->jmin=jmin;
		this->jmax=jmax;
		
		this->obst=obst;
		build();
}

template <class T>
minpath::Graph<T>::~Graph() {
	// TODO Auto-generated destructor stub
}

/*
template<class T>
inline void minpath::FieldGraph<T>::build(T imin, T imax, T jmin, T jmax,
		Obstacles<T> &obst) {

		
	
}
*/

template<class T>
inline void minpath::FieldGraph<T>::build() {

//		auto linked_nodes=get_linked_nodes(ist,jst);
//		Node n(ist,jst,minpath::Node::start);
//		this->addNode(n,linked_nodes);
		
		typename minpath::Node<T>::neighbor_t n;
		n.dist=1;
		
		// allocate memory for all graph nodes
		// i - iterates columns in the field
		// j - iterates rows in the field
		T Ncols=imax-imin+1;
		T Nrows=jmax-jmin+1;
		std::vector< std::vector<minpath::Node<T> > > field; // first coord iterates column
		std::vector< minpath::Node<T> > field_col(Nrows);
		for (long i=0;i<Ncols;i++) {
			field.push_back(field_col);
		}
		
		//
		// make connection and initialize distances of links
		//
		
		// lower left corner
		n.neigh_node=&field[0][1];
		field[0][0].addNeighbour(n);
		n.neigh_node=&field[1][0];
		field[0][0].addNeighbour(n);

		// lower right corner
		n.neigh_node=&field[Ncols-2][0];
		field[Ncols-1][0].addNeighbour(n);
		n.neigh_node=&field[Ncols-1][1];
		field[Ncols-1][0].addNeighbour(n);

		// upper left corner
		n.neigh_node=&field[1][Nrows-1];
		field[0][Nrows-1].addNeighbour(n);
		n.neigh_node=&field[0][Nrows-2];
		field[0][Nrows-1].addNeighbour(n);

		// upper right corner
		n.neigh_node=&field[Ncols-2][Nrows-1];
		field[Ncols-1][Nrows-1].addNeighbour(n);
		n.neigh_node=&field[Ncols-1][Nrows-2];
		field[Ncols-1][Nrows-1].addNeighbour(n);

		// bottom line
		for (long i=1;i<Ncols-1;i++) {
			n.neigh_node=&field[i-1][0];
			field[i][0].addNeighbour(n);
			n.neigh_node=&field[i+1][0];
			field[i][0].addNeighbour(n);
			n.neigh_node=&field[i][1];
			field[i][0].addNeighbour(n);			
		}
		
		// top line
		for (long i=1;i<Ncols-1;i++) {
			n.neigh_node=&field[i-1][Nrows-1];
			field[i][Nrows-1].addNeighbour(n);
			n.neigh_node=&field[i+1][Nrows-1];
			field[i][Nrows-1].addNeighbour(n);
			n.neigh_node=&field[i][Nrows-2];
			field[i][Nrows-1].addNeighbour(n);			
		}

		// left line
		for (long j=1;j<Nrows-1;j++) {
			n.neigh_node=&field[0][j-1];
			field[0][j].addNeighbour(n);
			n.neigh_node=&field[0][j+1];
			field[0][j].addNeighbour(n);
			n.neigh_node=&field[1][j];
			field[0][j].addNeighbour(n);
		}

		// right line
		for (long j=1;j<Nrows-1;j++) {
			n.neigh_node=&field[Ncols-1][j-1];
			field[Ncols-1][j].addNeighbour(n);
			n.neigh_node=&field[Ncols-1][j+1];
			field[Ncols-1][j].addNeighbour(n);
			n.neigh_node=&field[Ncols-2][j];
			field[Ncols-1][j].addNeighbour(n);
		}

		// inside nodes
		for (long i=1;i<Ncols-1;i++) {
			for (long j=1;j<Nrows-1;j++) {
				n.neigh_node=&field[i-1][j];
				field[i][j].addNeighbour(n);
				n.neigh_node=&field[i+1][j];
				field[i][j].addNeighbour(n);
				n.neigh_node=&field[i][j-1];
				field[i][j].addNeighbour(n);
				n.neigh_node=&field[i][j+1];
				field[i][j].addNeighbour(n);
			}
		}

		long int id=0;
		for (long i=0;i<Ncols;i++) {
			for (long j=0;j<Nrows;j++) {
				field[i][j].data().min_dist_from_start=-1; // this means infinity
				field[i][j].data().id=id; 
				field[i][j].data().i=i; 
				field[i][j].data().j=j; 
				if (i==ist and j==jst) field[i][j].setStart();
				if (i==ien and j==jen) field[i][j].setEnd();

				Graph<T>::addNode(field[i][j]);
				id++;
			}
		}

		if (this->obst!=0) {
			std::cout << "Warning: currently we do not support building graphs with obstacles (ignoring)" << std::endl;
		}
}
/* ******************************************************************************************** */
template<class T>
void minpath::Graph<T>::addNode(minpath::Node<T> node
//		std::list<minpath::Node*> linked_nodes,
//		std::list<T> link_lenghts
		) {
		
//		assert(linked_nodes.size()==link_lenghts.size());
		
		this->node.push_back(node);
//		this->linked_nodes.push_back(linked_nodes);
//		this->link_lengths.push_back(link_lenghts);
}
/* ******************************************************************************************** */
template<class T>
inline std::list<minpath::Node<T>*> minpath::FieldGraph<T>::get_linked_nodes(T i, T j) {
		std::list<minpath::Node<T>*> neigh;
		return neigh;
}
/* ******************************************************************************************** */
template<typename T>
inline std::ostream& minpath::operator <<(std::ostream &out,
		const Graph<T> &curr) {

	unsigned long Nnodes = curr.getNodesNum();
	std::cout << "Graph has " << Nnodes << " nodes." << std::endl;
	for (auto& node : curr.getNodes()) {
		std::cout << "NODE" << std::endl;
		std::cout << node;
		std::cout << std::endl;
	}
	return out;
}
/* ******************************************************************************************** */
template<class T>
inline void minpath::Graph<T>::solve() {
		
}

#endif /* GRAPH_H_ */

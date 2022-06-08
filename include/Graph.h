/*
 * Graph.h
 *
 *  Created on: Jan 3, 2022
 *      Author: blew
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <set>
#include <unordered_set>
#include <assert.h>
#include <limits>
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
			/*
			static bool cmp_nodes(Node<T>* a, Node<T>* b) { 
				if (a->getMinDistFromStart()!=b->getMinDistFromStart()) {
					return a->getMinDistFromStart()<b->getMinDistFromStart(); 
				}
				return a<b;
//				return a->getMinDistFromStart()<b->getMinDistFromStart(); 
			}
			 */
			
			Graph(long Nnodes=0);
			virtual ~Graph();
			
			void addNode(minpath::Node<T>* node
					//					std::list<minpath::Node*> linked_nodes=std::list<minpath::Node*>(),
					//					std::list<T> link_lenghts=std::list<T>()
			);
			//			void addNode(minpath::Node node);
			
			/*!
				\brief return set of 
				\details 
				@param
				@return
			
				\date Jun 2, 2022, 2:25:43 PM
			 */
			std::unordered_set< minpath::Node<T>* > getUnvisitedNodes();
			std::list< minpath::Node<T>* >& nodes() { return this->node;}
			const std::list< minpath::Node<T>* > getNodes() const { return this->node;}
			unsigned long getNodesNum() const { return this->node.size(); }
			//			std::list<minpath::Node*>& neighbors(long i) { return this->linked_nodes[i]; }
			//			std::list<T>& dist(unsigned long i) { return this->link_lengths[i]; }
			void build() {}
			/*!
				\brief set all nodes as unvisited and set their distances to initial node to infinity
				\details 
			
				\date May 12, 2022, 1:53:47 PM
			 */
			void initialize_graph();
			minpath::Node<T>* getStartNode(unsigned long* idx=0);
			minpath::Node<T>* getStopNode(unsigned long* idx=0);
			/*!
				\brief get a set of unvisited neighbors while updating distances to nodes in the graph
				\details 
				@param cur - pointer of the node for which we want to obtain a set of unvisited 
					neighbors
				@return set of unvisited neighbors.
			
				\date May 12, 2022, 1:50:20 PM
			 */
			
			
			std::unordered_set< minpath::Node<T>* > unvisitedNeighbors(minpath::Node<T>* cur);
			
			T get_min_dist(std::unordered_set<minpath::Node<T>* > nodes);
			minpath::Node<T>* get_min_dist_node(std::unordered_set<minpath::Node<T>* > nodes);
			
			T solve();
			
			//			std::list< std::list<T> > link_lengths;
			//			std::list< std::list<minpath::Node*> > linked_nodes;
			std::list< minpath::Node<T>* > node;
			static constexpr T max_dist=std::numeric_limits<T>::max();
			
	};
	
	
	
	
	/*!
		\brief class that initializes the underlying graph as rectangular field
		\details 
		The graph connections have unit distances and each rectangular field has
		horizontal and vertical connections to neighbors unless the cells are
		at the field boundary.
	
		\date May 12, 2022, 11:47:01 AM
	 */
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
			
			T imin, imax, jmin, jmax, ist, jst, ien, jen; // coordinate space
			Obstacles<T>* obst;
	};
	
} /* namespace minpath */


/* ******************************************************************************************** */
/* ******************************************************************************************** */
/* ******************************************************************************************** */
/* ******************************************************************************************** */
template <class T>
minpath::Graph<T>::Graph(long Nnodes) :
//	adj(Nnodes, std::list<T>() ),
//node(Nnodes, minpath::Node<T>() ) 
node(Nnodes, 0 ) 
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
		for (auto n : nodes()) {
			if (n!=0) delete n;
		}
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
		n.dist=1; // set distance to the neighbor (same for all grid cells)
		
		// allocate memory for all graph nodes
		// i - iterates columns in the field
		// j - iterates rows in the field
		T Ncols=imax-imin+1;
		T Nrows=jmax-jmin+1;
		std::vector< std::vector<minpath::Node<T>* > > field; // first coord iterates column
		std::vector< minpath::Node<T>* > field_col(Nrows);
		for (long i=0;i<Ncols;i++) {
			field.push_back(field_col);
			for (long j=0;j<Nrows;j++) {
				field[i][j]=new minpath::Node<T>();
			}
		}
		
		//
		// make connection and initialize distances of links
		//
		
		// lower left corner
		n.neigh_node=field[0][1]; // set address of the neighbour
		field[0][0]->addNeighbour(n);
		n.neigh_node=field[1][0];
		field[0][0]->addNeighbour(n);
		
		// lower right corner
		n.neigh_node=field[Ncols-2][0];
		field[Ncols-1][0]->addNeighbour(n);
		n.neigh_node=field[Ncols-1][1];
		field[Ncols-1][0]->addNeighbour(n);
		
		// upper left corner
		n.neigh_node=field[1][Nrows-1];
		field[0][Nrows-1]->addNeighbour(n);
		n.neigh_node=field[0][Nrows-2];
		field[0][Nrows-1]->addNeighbour(n);
		
		// upper right corner
		n.neigh_node=field[Ncols-2][Nrows-1];
		field[Ncols-1][Nrows-1]->addNeighbour(n);
		n.neigh_node=field[Ncols-1][Nrows-2];
		field[Ncols-1][Nrows-1]->addNeighbour(n);
		
		// bottom line
		for (long i=1;i<Ncols-1;i++) {
			n.neigh_node=field[i-1][0];
			field[i][0]->addNeighbour(n);
			n.neigh_node=field[i+1][0];
			field[i][0]->addNeighbour(n);
			n.neigh_node=field[i][1];
			field[i][0]->addNeighbour(n);			
		}
		
		// top line
		for (long i=1;i<Ncols-1;i++) {
			n.neigh_node=field[i-1][Nrows-1];
			field[i][Nrows-1]->addNeighbour(n);
			n.neigh_node=field[i+1][Nrows-1];
			field[i][Nrows-1]->addNeighbour(n);
			n.neigh_node=field[i][Nrows-2];
			field[i][Nrows-1]->addNeighbour(n);			
		}
		
		// left line
		for (long j=1;j<Nrows-1;j++) {
			n.neigh_node=field[0][j-1];
			field[0][j]->addNeighbour(n);
			n.neigh_node=field[0][j+1];
			field[0][j]->addNeighbour(n);
			n.neigh_node=field[1][j];
			field[0][j]->addNeighbour(n);
		}
		
		// right line
		for (long j=1;j<Nrows-1;j++) {
			n.neigh_node=field[Ncols-1][j-1];
			field[Ncols-1][j]->addNeighbour(n);
			n.neigh_node=field[Ncols-1][j+1];
			field[Ncols-1][j]->addNeighbour(n);
			n.neigh_node=field[Ncols-2][j];
			field[Ncols-1][j]->addNeighbour(n);
		}
		
		// inside nodes
		for (long i=1;i<Ncols-1;i++) {
			for (long j=1;j<Nrows-1;j++) {
				n.neigh_node=field[i-1][j];
				field[i][j]->addNeighbour(n);
				n.neigh_node=field[i+1][j];
				field[i][j]->addNeighbour(n);
				n.neigh_node=field[i][j-1];
				field[i][j]->addNeighbour(n);
				n.neigh_node=field[i][j+1];
				field[i][j]->addNeighbour(n);
			}
		}
		
		// initialize graph distances and other data
		long int id=0;
		for (long i=0;i<Ncols;i++) {
			for (long j=0;j<Nrows;j++) {
				field[i][j]->data().min_dist_from_start=minpath::Graph<T>::max_dist;
				field[i][j]->data().id=id; 
				field[i][j]->data().i=i+imin; 
				field[i][j]->data().j=j+jmin; 
				if (i+imin==ist and j+jmin==jst) field[i][j]->setStart();
				if (i+imin==ien and j+jmin==jen) field[i][j]->setEnd();
				
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
void minpath::Graph<T>::addNode(minpath::Node<T>* node
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

template<class T>
inline void minpath::Graph<T>::initialize_graph() {
		
		// initialize graph distances and other data
		long int id=0;
		for (auto& n : this->node) {
			
			if (n->isStart()) n->data().min_dist_from_start=0;
			else n->data().min_dist_from_start=max_dist;
			
			n->data().was_visited=false;
		}
		
}

template<class T>
inline minpath::Node<T>* minpath::Graph<T>::getStartNode(unsigned long* idx) {
		minpath::Node<T>* cur=0;
		// goto start node
		long i=0;
		/*
		for (i = 0; i < this->nodes().size(); i++) {
			if ((this->nodes())[i].isStart()) break;
		}
		 */
		for (auto& n : this->nodes()) {
			if (n->isStart()) { 
				cur=n;
				break; 
			}
			i++;
		}
		if (idx!=0) *idx=i;
		//		cur=&this->nodes()[i];
		return cur;
}

template<class T>
inline std::unordered_set< minpath::Node<T>* >  minpath::Graph<T>::unvisitedNeighbors(minpath::Node<T>* cur) {
		std::unordered_set< minpath::Node<T>* > uv;
		
		for (auto& n : cur->neighbors()) {
			if (not n.neigh_node->wasVisited()) {
				T tentative_dist=cur->getMinDistFromStart()+n.dist;
				if (tentative_dist<n.neigh_node->getMinDistFromStart())
					n.neigh_node->setMinDistFromStart(tentative_dist);
				uv.insert(n.neigh_node);
			}
			std::cout << "-----\n" << *n.neigh_node;
			std::cout << std::endl;
		}
		
		return uv;
}

template<class T>
inline std::unordered_set<minpath::Node<T>* > minpath::Graph<T>::getUnvisitedNodes() {
		std::unordered_set<minpath::Node<T>* > unvisited_nodes;
		
		
		//		typename std::list< minpath::Node<T> >::iterator it=this->nodes().begin();
		//		for (it=this->nodes().begin(); it!=this->nodes().end(); ++it) {
		//			if (not (*it).wasVisited()) { 
		//				unvisited_nodes.insert(&(*it));
		//				std::cout << unvisited_nodes.size() << "\n";
		//			}
		//			
		//		}
		
		for (auto& n : this->nodes()) {
			if (not n->wasVisited()) { 
				unvisited_nodes.insert(n);
				std::cout << unvisited_nodes.size() << "," << n << "\n";
			}
		}
		
		return unvisited_nodes;
}

template<class T>
inline minpath::Node<T>* minpath::Graph<T>::getStopNode(unsigned long *idx) {
		minpath::Node<T>* cur=0;
		long i=0;
		for (auto& n : this->nodes()) {
			if (n->isEnd()) { 
				cur=n;
				break; 
			}
			i++;
		}
		if (idx!=0) *idx=i;
		return cur;
}
/* ******************************************************************************************** */
template<class T>
inline T minpath::Graph<T>::get_min_dist(
		std::unordered_set<minpath::Node<T>*> nodes) {
		
//		if (nodes.size()==0) return max_dist;
		auto n=get_min_dist_node(nodes);
		if (n==0) return max_dist;
		return n->getMinDistFromStart();
}
/* ******************************************************************************************** */
template<class T>
inline minpath::Node<T>* minpath::Graph<T>::get_min_dist_node(
		std::unordered_set<minpath::Node<T>*> nodes) {
		
		if (nodes.size()==0) return 0;
		T minD=max_dist;
		minpath::Node<T>* minD_node=0;//=*nodes.begin();
		for (auto n : nodes) {
			if (minD_node==0) minD_node=n;
			if (n->getMinDistFromStart()<minD_node->getMinDistFromStart()) 
				minD_node=n;
		}
		return minD_node;
}

/* ******************************************************************************************** */
template<class T>
inline T minpath::Graph<T>::solve() {
		
		initialize_graph();
		
		std::cout << "initialized graph\n" << *this << "\n";
		std::cout << "initialized graph\n" << *this << "\n";
		
		auto unvisited_nodes=getUnvisitedNodes();
		unsigned long start_node_idx=-1;
		minpath::Node<T>* cur=getStartNode(&start_node_idx);
		minpath::Node<T>* endNode=getStopNode();
		assert(cur!=0);
		
		bool unvisited_min_dist_eq_inf=false;
		//
		while (not endNode->wasVisited() and not unvisited_min_dist_eq_inf) {
			
			// get set of unvisited neighbors 
			// and on the graph update their distances from the starting node
			auto unvisited_neigh=unvisitedNeighbors(cur);
			
			cur->setVisited(true);
			if (not cur->isEnd()) {
				unvisited_nodes.erase(cur);
				cur=get_min_dist_node(unvisited_neigh);
				unvisited_min_dist_eq_inf=get_min_dist(unvisited_nodes)==max_dist;
			}
			std::cout << "Current node:\n " << *cur << "\n"; // @suppress("Symbol is not resolved")
		}
		return cur->getMinDistFromStart();
}

#endif /* GRAPH_H_ */

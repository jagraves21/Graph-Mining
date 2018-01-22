#ifndef _GRAPH_HH_
#define _GRAPH_HH_

#include <ostream>
#include <sstream>

#include <set>
#include <utility>

#include <algorithm>

#include <stdexcept>

#include <cstddef>

#include "output_any.hh"

template <typename V>
class graph {
	public:
		typedef size_t size_type;

		typedef typename std::set<V>::value_type VERTEX;
		typedef typename std::set<std::pair<VERTEX,VERTEX> >::value_type EDGE;

		typedef typename std::set<VERTEX>::iterator vertex_iterator;
		typedef typename std::set<VERTEX>::const_iterator const_vertex_iterator;
		
		typedef typename std::set<EDGE>::iterator edge_iterator;
		typedef typename std::set<EDGE>::const_iterator const_edge_iterator;

		graph() {

		}

		graph(const graph &other) : vertices(other.vertices), edges(other.edges) {

		}

		graph & operator=(const graph &other) {
			vertices = other.vertices;
			edges = other.edges;

			return *this;
		}

		/*
		 * Iterators
		 */
		vertex_iterator begin_vertices() {
			return vertices.begin();
		}

		vertex_iterator end_vertices() {
			return vertices.end();
		}

		const_vertex_iterator begin_vertices() const {
			return vertices.begin();
		}
		
		const_vertex_iterator end_vertices() const {
			return vertices.end();
		}

		edge_iterator begin_edges() {
			return edges.begin();
		}

		edge_iterator end_edges() {
			return edges.end();
		}

		const_edge_iterator begin_edges() const {
			return edges.begin();
		}
		
		const_edge_iterator  end_edges() const {
			return edges.end();
		}

		/*
		 * Capacity
		 */
		size_type size_vertices() {
			return (size_type)vertices.size();
		}

		size_type size_edges() {
			return (size_type)edges.size();
		}

		/*
		 * Element Access
		 */

		/* 
		 * Modifiers
		 */
		std::pair<vertex_iterator,bool> insert(const VERTEX &vertex) {
			return vertices.insert(vertex);
		}
		
		std::pair<edge_iterator,bool> insert(const EDGE &edge) {
			if(find(edge.first) == end_vertices()) {
				std::ostringstream oss;
				oss << "unexpected vertex";

				throw std::domain_error(oss.str());
			}
			else if(find(edge.second) == end_vertices()) {
				std::ostringstream oss;
				oss << "unexpected vertex";

				throw std::domain_error(oss.str());
			}

			if(edge.first > edge.second) {
				return edges.insert( EDGE(edge.second, edge.first) );
			}
			else {
				return edges.insert(edge);
			}
		}
		
		std::pair<edge_iterator,bool> insert(const VERTEX &src, const VERTEX &dst) {
			if(src <= dst) {
				return insert( EDGE(src,dst) );
			}
			else {
				return insert( EDGE(dst,src) );
			}
		}

		size_type erase(const VERTEX &vertex) {
			vertex_iterator vertex_iter = find(vertex);
			if(vertex_iter != end_vertices()) {
				remove_incident_edges(vertex);		
				erase(vertex_iter);	
			}
			else {
				return 0;
			}
		}

		size_type erase(const EDGE &edge) {
			if(edge.first > edge.second) {
				return (size_type)edges.erase( EDGE(edge.second, edge.first) );
			}
			else {
				return (size_type)edges.erase(edge);
			}
		}
		
		void erase(vertex_iterator position) {
			vertices.erase(position);
		}

		void erase(edge_iterator position) {
			edges.erase(position);
		}

		void clear_vertices() {
			vertices.clear();
			clear_edges();
		}

		void clear_edges() {
			edges.clear();
		}

		void clear() {
			clear_vertices();
			clear_edges();
		}

		void remove_incident_edges(const VERTEX &vertex) {
			/*
			edge_iterator edge_iter = begin_edges();
			for(; edge_iter != end_edges(); ++edge_iter) {
				if(edge_iter->first == vertex || edge_iter->second == vertex) {
					edges.erase(edge_iter++);
				}
				else {
					++edge_iter;
				}
			}*/
			std::remove_if(edges.begin(), edges.end(), incident_test(vertex));
		}

		/*
		 * Operations
		 */
		vertex_iterator find(const VERTEX &vertex) {
			return vertices.find(vertex);
		}

		const_vertex_iterator find(const VERTEX &vertex) const {
			return vertices.find(vertex);
		}

		edge_iterator find(const EDGE &edge) {
			return edges.find(edge);
		}

		const_edge_iterator find(const EDGE &edge) const {
			return edges.find(edge);
		}
		
		vertex_iterator lower_bound(const VERTEX &vertex) {
			return vertices.lower_bound(vertex);
		}
		
		const_vertex_iterator lower_bound(const VERTEX &vertex) const {
			return vertices.lower_bound(vertex);
		}

		edge_iterator lower_bound(const EDGE &edge) {
			return edges.lower_bound(edge);
		}
		
		const_edge_iterator lower_bound(const EDGE &edge) const {
			return edges.lower_bound(edge);
		}

		vertex_iterator upper_bound(const VERTEX &vertex) {
			return vertices.upper_bound(vertex);
		}
		
		const_vertex_iterator upper_bound(const VERTEX &vertex) const {
			return vertices.upper_bound(vertex);
		}

		edge_iterator upper_bound(const EDGE &edge) {
			return edges.upper_bound(edge);
		}
		
		const_edge_iterator upper_bound(const EDGE &edge) const {
			return edges.upper_bound(edge);
		}

		/*
		 * Serialization
		 */

		virtual std::ostream & serialize(std::ostream &output) const {
			const_vertex_iterator vertex_iter = begin_vertices();
			const_edge_iterator edge_iter = begin_edges();
			for(; vertex_iter != end_vertices(); ++vertex_iter) {
				output << output_any(*vertex_iter) << std::endl;
				while(edge_iter != end_edges() && edge_iter->first == *vertex_iter) {
					output << "  " << output_any(edge_iter->second) << std::endl;
					++edge_iter;
				}
			}
			return output;
		}

		friend std::ostream & operator<<(std::ostream &output, const graph &graph) {
			return graph.serialize(output);
		}
	
	protected:
		std::set<VERTEX> vertices;
		std::set<EDGE> edges;
	
		struct incident_test {
			const VERTEX &vertex;
			incident_test(const VERTEX &vertex): vertex(vertex) {
				
			}

			bool operator()(const EDGE &edge) const {
				return edge.first == vertex || edge.second == vertex;
			}
		};	
};

#endif


#ifndef _GRAPH_HH_
#define _GRAPH_HH_

#include <ostream>
#include <sstream>

#include <map>
#include <utility>

#include <algorithm>

#include <stdexcept>

#include <cstddef>

#include "output_any.hh"

template <typename V, typename L>
class labeled_graph {
	public:
		typedef size_t size_type;

		typedef L label;
		typedef V vertex;
		
		typedef typename std::map<std::pair<vertex,vertex>,label>::key_type edge;

		typedef typename std::map<vertex,label>::iterator vertex_iterator;
		typedef typename std::map<vertex,label>::const_iterator const_vertex_iterator;
		
		typedef typename std::map<std::pair<vertex,vertex>,label>::iterator edge_iterator;
		typedef typename std::map<std::pair<vertex,vertex>,label>::const_iterator const_edge_iterator;

		labeled_graph() {

		}

		labeled_graph(const labeled_graph &other) : vertices(other.vertices), edges(other.edges)  {
			
		}

		labeled_graph & operator=(const labeled_graph &other) {
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
		
		const_edge_iterator end_edges() const {
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
		std::pair<vertex_iterator,bool> insert(const vertex &vrt, label &lbl) {
			vertex_iterator vertex_iter = vertices.lower_bound(vrt);
			if(vertex_iter != end_vertices() && vertex_iter->first == vrt) {
				return std::pair<vertex_iterator,bool>(vertex_iter,false);
			}
			
			return vertices.insert( typename std::map<vertex,label>::value_type(vrt,lbl) );
		}
		
		std::pair<edge_iterator,bool> insert(const edge &edg) {
			if(find(edg.first) == end_vertices()) {
				std::ostringstream oss;
				oss << "unexpected vertex";

				throw std::domain_error(oss.str());
			}
			else if(find(edg.second) == end_vertices()) {
				std::ostringstream oss;
				oss << "unexpected vertex";

				throw std::domain_error(oss.str());
			}

			return edges.insert(edg);
		}
		
		std::pair<edge_iterator,bool> insert(const vertex &src, const vertex &dst) {
			if(src <= dst) {
				return insert( edge(src,dst) );
			}
			else {
				return insert( edge(dst,src) );
			}
		}

		size_type erase(const vertex &vrt) {
			vertex_iterator vertex_iter = find(vrt);
			if(vertex_iter != end_vertices()) {
				remove_incident_edges(vrt);		
				erase(vertex_iter);	
			}
			else {
				return 0;
			}
		}

		size_type erase(const edge &edg) {
			return (size_type)edges.erase(edg);
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

		void remove_incident_edges(const vertex &vrt) {
			/*
			edge_iterator edge_iter = begin_edges();
			for(; edge_iter != end_edges(); ++edge_iter) {
				if(edge_iter->first == vrt || edge_iter->second == vrt) {
					edges.erase(edge_iter++);
				}
				else {
					++edge_iter;
				}
			}*/
			std::remove_if(edges.begin(), edges.end(), is_incident(vrt));
		}

		/*
		 * Operations
		 */
		vertex_iterator find(const vertex &vrt) {
			return vertices.find(vrt);
		}

		const_vertex_iterator find(const vertex &vrt) const {
			return vertices.find(vrt);
		}

		edge_iterator find(const edge &edg) {
			return edges.find(edg);
		}

		const_edge_iterator find(const edge &edg) const {
			return edges.find(edg);
		}
		
		vertex_iterator lower_bound(const vertex &vrt) {
			return vertices.lower_bound(vrt);
		}
		
		const_vertex_iterator lower_bound(const vertex &vrt) const {
			return vertices.lower_bound(vrt);
		}

		edge_iterator lower_bound(const edge &edg) {
			return edges.lower_bound(edg);
		}
		
		const_edge_iterator lower_bound(const edge &edg) const {
			return edges.lower_bound(edg);
		}

		vertex_iterator upper_bound(const vertex &vrt) {
			return vertices.upper_bound(vrt);
		}
		
		const_vertex_iterator upper_bound(const vertex &vrt) const {
			return vertices.upper_bound(vrt);
		}

		edge_iterator upper_bound(const edge &edg) {
			return edges.upper_bound(edg);
		}
		
		const_edge_iterator upper_bound(const edge &edg) const {
			return edges.upper_bound(edg);
		}

		/*
		 * Serialization
		 */

		virtual std::ostream & serialize(std::ostream &output) const {
			const_vertex_iterator vertex_iter = begin_vertices();
			const_edge_iterator edge_iter = begin_edges();
			for(; vertex_iter != end_vertices(); ++vertex_iter) {
				output << output_any(*vertex_iter) << std::endl;
				while(edge_iter != end_edges() && edge_iter->first.first == vertex_iter->first) {
					output << "  " << output_any(edge_iter->second) << std::endl;
					++edge_iter;
				}
			}
			return output;
		}

		friend std::ostream & operator<<(std::ostream &output, const labeled_graph &graph) {
			return graph.serialize(output);
		}
	
	protected:
		std::map<vertex,label> vertices;
		std::map<edge,label> edges;
	
		struct is_incident {
			const vertex &vrt;
			is_incident(const vertex &vrt): vrt(vrt) {
				
			}

			bool operator()(const edge &edg) const {
				return edg.first == vrt || edg.second == vrt;
			}
		};	
};

#endif


/*
 * Created by Maou Lim on 2019/1/20.
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "../common/defines.h"
#include "pair.h"
#include "sequence.h"

namespace tools {

	template <typename _EdgeIterator>
	struct _neighbor_iterator_base {

	};

	template <
	    typename _Key,
	    typename _Vertex,
	    typename _Edge,
	    typename _KeyEqual
	>
	struct digraph {

		typedef digraph<_Key, _Vertex, _Edge, _KeyEqual> self_type;

		typedef _Key    key_type;
		typedef _Vertex vertex_attr;
		typedef _Edge   edge_attr;
		typedef size_t  size_type;

		virtual ~digraph() = default;

		vertex_attr& operator[](const key_type& key) { return vertex(key); }
		const vertex_attr& operator[](const key_type& key) const { return vertex(key); }

		edge_attr& operator()(const key_type& from, const key_type& to) { return edge(from, to); }
		const edge_attr& operator()(const key_type& from, const key_type& to) const { return edge(from, to); }

		bool empty() const { return 0 == edges(); }
		bool zero() const { return 0 == vertices(); }

		virtual size_t vertices() const = 0;
		virtual size_t edges() const = 0;

		virtual const vertex_attr& vertex(const key_type&) const = 0;
		virtual const edge_attr& edge(const key_type& from, const key_type&) const = 0;

		vertex_attr& vertex(const key_type& key) {
			return const_cast<vertex_attr&>(((const self_type*) this)->vertex(key));
		}

		edge_attr& edge(const key_type& from, const key_type& to) {
			return const_cast<edge_attr&>(((const self_type*) this)->edge(from, to));
		}

		template <typename _VertexVisitor>
		void foreach_dfs(const key_type& origin, _VertexVisitor visitor) {

		}

		template <typename _VertexVisitor>
		void foreach_bfs(const key_type& origin, _VertexVisitor visitor) {

		}
	};

}

#endif //_GRAPH_H_

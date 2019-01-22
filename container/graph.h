/*
 * Created by Maou Lim on 2019/1/20.
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "../common/defines.h"
#include "pair.h"
#include "sequence.h"

namespace tools {

	struct _graph_node_base {
		typedef _graph_node_base          self_type;
		typedef self_type*                base_ptr;
		typedef tools::sequence<base_ptr> adjacency_type;

		virtual ~_graph_node_base() = default;

		adjacency_type adjacency;
	};

	template <typename _VertexAttr, typename _EdgeAttr>
	struct _graph_node : _graph_node_base {
	private:
		typedef _graph_node_base                    base_type;
		typedef _graph_node<_VertexAttr, _EdgeAttr> self_type;
		typedef _VertexAttr                         vertex_attr;
		typedef _EdgeAttr                           edge_attr;
		typedef self_type*                          link_type;
		typedef adjacency_type::size_type           size_type;

	public:
		vertex_attr v_attr;
		edge_attr   e_attr;

		_graph_node(const vertex_attr& _va, const edge_attr& _ea) :
			v_attr(_va), e_attr(_ea) { }

		link_type& operator[](size_type index) { (link_type&) adjacency[index]; }
	};

	template <typename _VertexAttr>
	struct _graph_node<_VertexAttr, void> : _graph_node_base {
	private:
		typedef _graph_node_base               base_type;
		typedef _graph_node<_VertexAttr, void> self_type;
		typedef _VertexAttr                    vertex_attr;
		typedef void                           edge_attr;
		typedef self_type*                     link_type;
		typedef adjacency_type::size_type      size_type;

	public:
		vertex_attr v_attr;

		_graph_node(const vertex_attr& _va) : v_attr(_va) { }

		link_type& operator[](size_type index) { (link_type&) adjacency[index]; }
	};

	template <
	    typename _VertexAttr, typename _EdgeAttr, typename _Alloc
	>
	class graph {
		typedef _graph_node_base*                     base_ptr;
		typedef _graph_node<_VertexAttr, _EdgeAttr>   node_type;
		typedef node_type*                            link_type;
		typedef graph<_VertexAttr, _EdgeAttr, _Alloc> self_type;
		typedef standard_alloc<node_type, _Alloc>     allocator_type;

	public:
		typedef size_t      size_type;
		typedef _VertexAttr vertex_attr;
		typedef _EdgeAttr   edge_attr;

		bool empty() const { return 0 == m_vertices; }

		size_type size()         const { return num_vertices(); }
		size_type num_vertices() const { return m_vertices; }
		size_type num_edges()    const { return m_edges; }

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { allocator_type::deallocate(p); }

		link_type create_node(const vertex_attr& _va, const edge_attr& _ea) {
			auto p = get_node();
			construct(p, _va, _ea);
			return p;
		}

		void destroy_node(link_type p) {
			destroy(p);
			put_node(p);
		}

	private:
		void _clear() {
			return;
		}


	private:
		size_type m_edges, m_vertices;
		base_ptr  m_base;
	};
}

#endif //_GRAPH_H_

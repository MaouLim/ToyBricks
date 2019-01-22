/*
 * Created by Maou Lim on 2018/1/23.
 */

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "../common/functor.h"
#include "iterator.h"
#include "memory.h"
#include "tree_base.h"
#include "pair.h"

namespace tools {

	struct _avl_tree_node_base : _bitree_node_base {

		typedef _avl_tree_node_base* base_ptr;
		typedef _bitree_node_base    base_type;
		typedef size_t               height_type;
		typedef ptrdiff_t            balance_factor_type;

		height_type height;

		explicit _avl_tree_node_base(base_ptr    p = nullptr,
		                             base_ptr    l = nullptr,
		                             base_ptr    r = nullptr,
		                             height_type h = 0) :
			base_type(p, l, r), height(h) { }

		balance_factor_type balance_factor() const { return left()->height - right()->height; }

		base_ptr& parent() const { return (base_ptr&) base_type::parent; }
		base_ptr& left() const { return (base_ptr&) base_type::left; }
		base_ptr& right() const { return (base_ptr&) base_type::right; }

	};

	template <typename _Val>
	struct _avl_tree_node : _avl_tree_node_base {
	protected:
		typedef _avl_tree_node_base  base_type;
		typedef _avl_tree_node<_Val> self_type;

	public:
		typedef base_type::base_ptr            base_ptr;
		typedef _Val                           value_type;
		typedef base_type::height_type         height;
		typedef base_type::balance_factor_type balance_factor_type;

	public:
		value_type value;

	public:
		explicit _avl_tree_node(const value_type& val) : value(val) { }
	};

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator = less<_Key>,
		typename _Allocator  = std::allocator<_avl_tree_node<_Val>>
	>
	class _avl_tree {
	protected:
		typedef _avl_tree_node<_Val>                    node_type;
		typedef node_type*                              link_type;
		typedef typename node_type::height_type         height_type;
		typedef typename node_type::balance_factor_type balance_factor_type;

		typedef _avl_tree<_Key, _Val, _KeyOf, _Comparator, _Allocator> self_type;

	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef _Key        key_type;
		typedef _Comparator comparator_type;
		typedef size_t      size_type;
		typedef self_type   bst_type;

		typedef standard_alloc<node_type, _Allocator> allocator_type;

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { return allocator_type::deallocate(p); }

		link_type create_node(const value_type& val) {
			link_type new_node = get_node();
			construct(new_node, val);
			return new_node;
		}

		link_type clone_node(link_type node) { return create_node(node->value); }
		void destroy_node(link_type node) { destroy(node); put_node(node); }

	private:
		link_type       m_header;
		size_type       m_count;
		comparator_type m_comp;

	protected:
		link_type& root() const { return (link_type&) m_header->parent; }
		link_type& leftmost() const { return (link_type&) m_header->left; }
		link_type& rightmost() const { return (link_type&) m_header->right; }

	private:
		void _initialize() {
			m_header = get_node();

			root()      = nullptr;
			leftmost()  = m_header;
			rightmost() = m_header;
		}

		void _clear() {
			// todo
		}

	public:
		explicit _avl_tree(const comparator_type& comp = _Comparator()) :
			m_count(0), m_comp(comp) { _initialize(); }

		template <typename _InputIterator>
		_avl_tree(_InputIterator         first,
		          _InputIterator         last ,
		          const comparator_type& comp = _Comparator()) :
			m_count(0), m_comp(comp) {
			_initialize();
			while (first != last) {
				this->insert_equal(*first);
				++first;
			}
		}

		~_avl_tree() { _clear(); put_node(m_header); }

	protected:
		typedef _bstree_iterator<node_type>       inner_iterator;
		typedef _const_bstree_iterator<node_type> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		typedef _reverse_iterator<iterator>       reverse_iterator;
		typedef _reverse_iterator<const_iterator> const_reverse_iterator;


	public:
		bool empty() const { return 0 == m_count; }
		size_type size() const { return m_count; }
		size_type max_size() const { return size_type(-1); }

		iterator begin() { return inner_iterator(root()); }
		const_iterator begin() const { return const_inner_iterator(root()); }

		iterator end() { return inner_iterator(m_header); }
		const_iterator end() const { return const_inner_iterator(m_header); }

		reverse_iterator rbegin() { return end(); }
		const_reverse_iterator rbegin() const { return end(); }

		reverse_iterator rend() { return begin(); }
		const_reverse_iterator rend() const { return begin(); }

		tools::pair<iterator, bool> insert_unique(const value_type& val) {

		}

		iterator insert_equal(const value_type& val) {

		}

		iterator find(const key_type& key) {

		}
	};
}

#endif //_AVL_TREE_H_

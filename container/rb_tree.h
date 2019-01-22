/*
 * Created by Maou Lim on 2018/1/11.
 */

#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <cassert>

#include "../common/functor.h"
#include "iterator.h"
#include "memory.h"
#include "tree_base.h"
#include "pair.h"

namespace tools {

	enum _rb_tree_color {
		_rb_tree_red, _rb_tree_black
	};

	struct _rb_tree_node_base : _bitree_node_base {
		typedef _bitree_node_base   base_type;
		typedef _rb_tree_node_base* base_ptr;
		typedef _rb_tree_color      color_type;

		color_type color;

		explicit _rb_tree_node_base(color_type color  = _rb_tree_red,
		                            base_ptr   parent = nullptr,
		                            base_ptr   left   = nullptr,
		                            base_ptr   right  = nullptr) :
			base_type(parent, left, right), color(color) { }

		base_ptr& parent() const { return (base_ptr&) base_type::parent; }
		base_ptr& left() const { return (base_ptr&) base_type::left; }
		base_ptr& right() const { return (base_ptr&) base_type::right; }
	};

	inline void _rb_tree_left_rotate(_bitree_node_base*  shaft,
	                                 _bitree_node_base*& root ) {
		_bitree_node_base* right = shaft->right;
		shaft->right = right->left;
		if (nullptr != right->left) {
			right->left->parent = shaft;
		}
		right->parent = shaft->parent;

		if (root == shaft) {
			root = right;
		}
		else if (shaft->parent->left == shaft) {
			shaft->parent->left = right;
		}
		else {
			shaft->parent->right = right;
		}

		right->left = shaft;
		shaft->parent = right;
	}

	inline void _rb_tree_right_rotate(_bitree_node_base*  shaft,
	                                  _bitree_node_base*& root ) {
		_bitree_node_base* left = shaft->left;
		shaft->left = left->right;
		if (nullptr != left->right) {
			left->right->parent = shaft;
		}
		left->parent = shaft->parent;

		if (root == shaft) {
			root = left;
		}
		else if (shaft->parent->right == shaft) {
			shaft->parent->right = left;
		}
		else {
			shaft->parent->left = left;
		}

		left->right = shaft;
		shaft->parent = left;
	}

	inline void _rb_tree_insert_rebalance(_rb_tree_node_base*  new_node,
	                                      _rb_tree_node_base*& root    ) {
		new_node->color = _rb_tree_red;
		while (root != new_node && _rb_tree_red == new_node->parent()->color) {
			if (new_node->parent() == new_node->parent()->parent()->left()) {
				_rb_tree_node_base* uncle = new_node->parent()->parent()->right();
				if (nullptr != uncle && _rb_tree_red == uncle->color) {
					new_node->parent()->color = _rb_tree_black;
					uncle->color = _rb_tree_black;
					new_node->parent()->parent()->color = _rb_tree_red;
					new_node = new_node->parent()->parent();
				}
				else {
					if (new_node->parent()->right() == new_node) {
						new_node = new_node->parent();
						_rb_tree_left_rotate(new_node, (_bitree_node_base*&) root);
					}
					new_node->parent()->color = _rb_tree_black;
					new_node->parent()->parent()->color = _rb_tree_red;
					_rb_tree_right_rotate(new_node->parent()->parent(), (_bitree_node_base*&) root);
				}
			}
			else {
				_rb_tree_node_base* uncle = new_node->parent()->parent()->left();
				if (nullptr != uncle && _rb_tree_red == uncle->color) {
					new_node->parent()->color = _rb_tree_black;
					uncle->color = _rb_tree_black;
					new_node->parent()->parent()->color = _rb_tree_red;
					new_node = new_node->parent()->parent();
				}
				else {
					if (new_node->parent()->left() == new_node) {
						new_node = new_node->parent();
						_rb_tree_right_rotate(new_node, (_bitree_node_base*&) root);
					}
					new_node->parent()->color = _rb_tree_black;
					new_node->parent()->parent()->color = _rb_tree_red;
					_rb_tree_left_rotate(new_node->parent()->parent(), (_bitree_node_base*&) root);
				}
			}
		}
		root->color = _rb_tree_black;
	}

	inline void _rb_tree_erase_rebalance() {
		// todo
	}

	template <typename _Val>
	struct _rb_tree_node : _rb_tree_node_base {

		typedef _rb_tree_node<_Val>* link_type;
		typedef _Val                 value_type;

		explicit _rb_tree_node(const value_type& value) : value(value) { }
		~_rb_tree_node() = default;

		value_type value;
	};

//	struct rb_tree_base_iterator {
//		typedef _rb_tree_node_base::base_ptr     base_ptr;
//		typedef std::bidirectional_iterator_tag iterator_category;
//		typedef ptrdiff_t                       difference_type;
//
//		base_ptr node;
//
//		rb_tree_base_iterator() = default;
//
//		explicit rb_tree_base_iterator(base_ptr p) : node(p) { }
//
//		void increment() {
//			if (nullptr != node->right) {
//				node = node->right;
//				while (nullptr != node->left) {
//					node = node->left;
//				}
//			}
//			else {
//				base_ptr p = node->parent;
//				while (p->right == node) {
//					node = p;
//					p = p->parent;
//				}
//				// todo
//				if (node->right != p) {
//					node = p;
//				}
//			}
//		}
//
//		void decrement() {
//			if (_rb_tree_red == node->color &&
//				node        == node->parent->parent) {
//				node = node->right;
//			}
//
//			else if (nullptr != node->left) {
//				base_ptr l = node->left;
//				while (nullptr != l->right) {
//					l = l->right;
//				}
//				node = l;
//			}
//
//			else {
//				base_ptr p = node->parent;
//				while (p->left == node) {
//					node = p;
//					p = p->parent;
//				}
//				node = p;
//			}
//		}
//	};
//
//	template <typename _Val>
//	struct rb_tree_const_iterator : rb_tree_base_iterator {
//	public:
//		typedef _Val        value_type;
//		typedef const _Val& reference;
//		typedef const _Val* pointer;
//
//	protected:
//		typedef rb_tree_const_iterator<_Val>           self_type;
//		typedef rb_tree_base_iterator                  base_type;
//		typedef typename _rb_tree_node<_Val>::link_type link_type;
//	public:
//		rb_tree_const_iterator() = default;
//		rb_tree_const_iterator(const self_type&) = default;
//		rb_tree_const_iterator(const base_type& other) : base_type(other.node) { }
//		explicit rb_tree_const_iterator(link_type p) : base_type(p) { }
//
//		reference operator*() const {
//			return link_type(node)->value;
//		}
//
//		pointer operator->() const {
//			return &(operator*());
//		}
//
//		self_type& operator++() {
//			increment();
//			return *this;
//		}
//
//		self_type& operator++(int) {
//			self_type tmp = *this;
//			increment();
//			return tmp;
//		}
//
//		self_type& operator--() {
//			decrement();
//			return *this;
//		}
//
//		self_type& operator--(int) {
//			self_type tmp = *this;
//			decrement();
//			return tmp;
//		}
//	};
//
//	template <typename _Val>
//	struct rb_tree_iterator : rb_tree_base_iterator {
//	public:
//		typedef _Val  value_type;
//		typedef _Val& reference;
//		typedef _Val* pointer;
//
//		typedef rb_tree_iterator<_Val> iterator;
//
//	protected:
//		typedef rb_tree_iterator<_Val>                 self_type;
//		typedef rb_tree_base_iterator                  base_type;
//		typedef typename _rb_tree_node<_Val>::link_type link_type;
//
//	public:
//		rb_tree_iterator() = default;
//		rb_tree_iterator(const self_type&) = default;
//		explicit rb_tree_iterator(link_type p) : base_type(p) { }
//
//		reference operator*() const {
//			return link_type(node)->value;
//		}
//
//		pointer operator->() const {
//			return &(operator*());
//		}
//
//		self_type& operator++() {
//			increment();
//			return *this;
//		}
//
//		self_type& operator++(int) {
//			self_type tmp = *this;
//			increment();
//			return tmp;
//		}
//
//		self_type& operator--() {
//			decrement();
//			return *this;
//		}
//
//		self_type& operator--(int) {
//			self_type tmp = *this;
//			decrement();
//			return tmp;
//		}
//	};
//
//	inline bool operator==(
//		const rb_tree_base_iterator& left,
//		const rb_tree_base_iterator& right
//	) {
//		return left.node == right.node;
//	}
//
//	inline bool operator!=(
//		const rb_tree_base_iterator& left,
//		const rb_tree_base_iterator& right
//	) {
//		return !operator==(left, right);
//	}

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator = less<_Key>,
		typename _Allocator  = std::allocator<_rb_tree_node<_Val>>
	>
	class _rb_tree {
	public:
		typedef _Key        key_type;
		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

		typedef _rb_tree_node<_Val>* link_type;
		typedef size_t               size_type;
		typedef ptrdiff_t            difference_type;

	protected:
		typedef _Comparator                           comparator_type;
		typedef _rb_tree_color                        color_type;
		typedef _rb_tree_node_base*                   base_ptr;
		typedef _rb_tree_node<_Val>                   node_type;
		typedef standard_alloc<node_type, _Allocator> allocator_type;

		typedef _rb_tree<_Key, _Val, _KeyOf, _Comparator, _Allocator> self_type;

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { return allocator_type::deallocate(p); }

		link_type create_node(const value_type& val) {
			link_type node = get_node();
			construct(node, val);
			return node;
		}

		link_type clone_node(link_type node) {
			link_type clone = create_node(node->value);
			clone->color = node->color;
			return clone;
		}

		void destroy_node(link_type node) {
			destroy(node);
			put_node(node);
		}

	private:
		size_type       m_count;
		link_type       m_header; /* virtual header */
		comparator_type m_comp;

	protected:
		link_type& root()      const { return (link_type&) m_header->parent(); }
		link_type& leftmost()  const { return (link_type&) m_header->left()  ; }
		link_type& rightmost() const { return (link_type&) m_header->right() ; }

	protected:
		typedef _bstree_iterator<node_type>       inner_iterator;
		typedef _const_bstree_iterator<node_type> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		typedef _reverse_iterator<iterator>       reverse_iterator;
		typedef _reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		void _initialize() {
			m_header = get_node();
			m_header->color = _rb_tree_red;

			root()      = nullptr;
			leftmost()  = m_header;
			rightmost() = m_header;
		}

		void _clear() {
			// todo

		}

		iterator _insert(link_type current, link_type parent, const value_type& val) {
			link_type new_node = nullptr;
			auto key_of = _KeyOf();

			if (
				m_header == parent  ||
				nullptr  != current || // ???
				m_comp(key_of(val), key_of(parent->value))
			) {
				new_node = create_node(val);
				parent->left() = new_node;
				if (m_header == parent) {
					root()     = new_node;
					leftmost() = new_node;
				}
				else if (leftmost() == parent) {
					leftmost() = new_node;
				}
			}
			else {
				new_node = create_node(val);
				parent->right() = new_node;
				if (rightmost() == parent) {
					rightmost() = new_node;
				}
			}
			new_node->parent() = parent;

			_rb_tree_insert_rebalance(
				new_node, reinterpret_cast<_rb_tree_node_base*&>(root())
			);
			++m_count;
			return inner_iterator(new_node);
		}

	public:
		explicit _rb_tree(const comparator_type& comp = _Comparator()) :
			m_count(0), m_comp(comp) { _initialize(); }

		~_rb_tree() {
			_clear();
			put_node(m_header);
		}

		self_type& operator=(const self_type& other) {
			// todo
			return *this;
		}

	public:
		const comparator_type& comparator() const { return m_comp; }

		iterator begin() { return inner_iterator(leftmost()); }
		const_iterator begin() const { return const_inner_iterator(leftmost()); }

		iterator end() { return inner_iterator(m_header); }
		const_iterator end() const { return const_inner_iterator(m_header); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		bool empty() const { return 0 == m_count; }
		size_type size() const { return m_count; }
		size_type max_size() const { return size_type(-1); }

		const_iterator minimum() const {
			if (empty()) {
				return end();
			}
			return const_inner_iterator(
				(link_type) _bstree_tool::minimum(root())
			);
		}

		const_iterator maximum() const {
			if (empty()) {
				return end();
			}

			return const_inner_iterator(
				(link_type) _bstree_tool::maximum(root())
			);
		}

	public:
		tools::pair<iterator, bool> insert_unique(const value_type& val) {
			link_type parent = m_header;
			link_type current = root();
			bool prior_to = true;
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				prior_to = m_comp(key_of(val), key_of(current->value));
				current = static_cast<link_type>(
					prior_to ? current->left() : current->right()
				);
			}

			assert(nullptr == current);

			inner_iterator iter(parent);
			if (prior_to) {
				if (leftmost() == parent) {
					return tools::pair<iterator, bool>(
						_insert(current, parent, val), true
					);
				}
				else {
					--iter;
				}
			}

			if (m_comp(key_of(*iter), key_of(val))) {
				return tools::pair<iterator, bool>(
					_insert(current, parent, val), true
				);
			}

			return tools::pair<iterator, bool>(iter, false);
		}

		iterator insert_equal(const value_type& val) {
			link_type parent = m_header;
			link_type current = root();
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				current = static_cast<link_type>(
					m_comp(key_of(val), key_of(current->value)) ? current->left() : current->right()
				);
			}

			assert(nullptr == current);

			return _insert(current, parent, val);
		}

		iterator find(const key_type& key) {
			link_type parent  = m_header;
			link_type current = root();

			auto key_of = _KeyOf();

			while (nullptr != current) {
				if (!m_comp(key_of(current->value), key)) {
					parent = current;
					current = static_cast<link_type>(current->left());
				}
				else {
					current = static_cast<link_type>(current->right());
				}
			}

			inner_iterator iter(parent);
			return (end() == iter || m_comp(key, key_of(*iter))) ? end() : iter;
		}
	};
}

#endif //_RED_BLACK_TREE_H_

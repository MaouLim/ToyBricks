/*
 * Created by Maou Lim on 2018/1/24.
 */

#ifndef _TREE_BASE_H_
#define _TREE_BASE_H_

#include <cassert>

#include "iterator.h"

namespace tools {

	struct _bitree_node_base {

		typedef _bitree_node_base* base_ptr;

		base_ptr parent;
		base_ptr left;
		base_ptr right;

		explicit _bitree_node_base(base_ptr p = nullptr,
		                           base_ptr l = nullptr,
		                           base_ptr r = nullptr) :
			parent(p), left(l), right(r) { }
	};

	template <typename _Val>
	struct _simple_bitree_node : _bitree_node_base {
	protected:

		typedef _bitree_node_base          base_type;
		typedef _simple_bitree_node<_Val>  self_type;

	public:
		typedef self_type* base_ptr;
		typedef _Val       value_type;

	public:
		value_type value;

	public:
		_simple_bitree_node() = default;
		_simple_bitree_node(const value_type& val) : value(val) { }
	};

	struct _bitree_iterator_base {
		typedef _bitree_node_base::base_ptr     base_ptr;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;

		base_ptr node;

		explicit _bitree_iterator_base(base_ptr p = nullptr) : node(p) { }
	};

	/* 默认顺序是L->R */
	namespace traversal {

		struct preorder  { };
		struct inorder   { };
		struct postorder { };

	}

	template <typename _Order>
	struct _bitree_iterator_impl;

	template <>
	struct _bitree_iterator_impl<
		traversal::preorder
	> : _bitree_iterator_base {

		typedef _bitree_iterator_base base_type;

		explicit _bitree_iterator_impl(base_ptr p = nullptr) :
			base_type(p) { }

		void increment() {
			if (nullptr != node->left) {
				node = node->left;
			}
			else if (nullptr != node->right) {
				node = node->right;
			}
			else {
				base_ptr p = node->parent;
				while ( nullptr == p->right ||
					   (node    == p->right &&
						node    != p->left)) {
					node = p;
					p = p->parent;
				}
				if (node == p->right) {
					node = p;
				}
				else {
					node = p->right;
				}
			}
		}

		void decrement() {
			/* 当树为空，而且 node == header 时引发错误 */
			base_ptr p = node->parent;
			if (node == p->right && node != p->left) {
				if (nullptr != p->left) {
					p = p->left;
					while (nullptr != p->right) {
						p = p->right;
					}
				}
			}

			if (p->parent != node || p->left == node) {
				node = p;
			}
			else {
				node = node->right;
			}
		}
	};

	template <>
	struct _bitree_iterator_impl<
		traversal::inorder
	> : _bitree_iterator_base {

		typedef _bitree_iterator_base base_type;

		explicit _bitree_iterator_impl(base_ptr p = nullptr) :
			base_type(p) { }

		void increment() {
			bool is_header = false;
			base_ptr p = node->parent;
			if (node    == p->parent &&
				nullptr != node->left &&
				nullptr != p->left) {
				is_header = (
					p->left == node->left &&
					node    != node->left->parent ||
					nullptr != p->left->left
				);
			}
			if (is_header) {
				node = node->left;
			}
			else if (nullptr != node->right) {
				node = node->right;
				while (nullptr != node->left) {
					node = node->left;
				}
			}
			else {
				base_ptr p = node->parent;
				while (p->right == node) {
					node = p;
					p = p->parent;
				}
				if (node->right != p) {
					node = p;
				}
			}
		}

		void decrement() {
			/* 当树为空，而且 node == header 时引发错误 */
			// 分开处理 header 和 root

			bool is_header = false;
			base_ptr p = node->parent;
			if (node    == p->parent &&
				nullptr != node->left &&
				nullptr != p->left) {
				is_header = (
					p->left == node->left &&
					node    != node->left->parent ||
					nullptr != p->left->left
				);
			}
			if (is_header) {
				node = node->right;
			}
			else if (nullptr != node->left) {
				base_ptr l = node->left;
				while (nullptr != l->right) {
					l = l->right;
				}
				node = l;
			}

			else {
				base_ptr p = node->parent;
				while (p->left == node) {
					node = p;
					p = p->parent;
				}
				if (p != node->left && p != node->right) {
					node = p;
				}
			}
		}
	};

	template <>
	struct _bitree_iterator_impl<
		traversal::postorder
	> : _bitree_iterator_base {

		typedef _bitree_iterator_base base_type;

		explicit _bitree_iterator_impl(base_ptr p = nullptr) : base_type(p) { }

		void increment() {
			/* 当树为空，而且 node == header 时引发错误 */
			base_ptr p = node->parent;
			if (node == p->left && node != p->right) {
				while (nullptr != p->right) {
					p = p->right;
					while (nullptr != p->left) {
						p = p->left;
					}
				}
			}

			if (p->parent != node || p->right == node) {
				node = p;
			}
			else {
				node = node->left;
			}
		}

		void decrement() {
			if (nullptr != node->right) {
				node = node->right;
			}
			else if (nullptr != node->left) {
				node = node->left;
			}
			else {
				base_ptr p = node->parent;
				while ( nullptr == p->left ||
					   (node    == p->left &&
					    node    != p->right)) {
					node = p;
					p = p->parent;
				}
				if (node != p->left) {
					node = p->left;
				}
				else {
					node = p;
				}
			}
		}
	};

	inline bool operator==(const _bitree_iterator_base& left,
	                       const _bitree_iterator_base& right) {
		return left.node == right.node;
	}

	inline bool operator!=(const _bitree_iterator_base& left,
	                       const _bitree_iterator_base& right) {
		return !(left == right);
	}

	template <typename _Node, typename _Order>
	struct _const_bitree_iterator :
		_bitree_iterator_impl<_Order> {
	protected:
		typedef _bitree_iterator_impl<_Order>         base_type;
		typedef _const_bitree_iterator<_Node, _Order> self_type;
		typedef _Node                                 node_type;
		typedef _Node*                                link_type;

	public:
		typedef typename node_type::value_type value_type;
		typedef const value_type&              reference;
		typedef const value_type*              pointer;

	public:
		_const_bitree_iterator() = default;
		_const_bitree_iterator(const base_type& other) : base_type(other) { }
		explicit _const_bitree_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(this->node)->value; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() { this->increment(); return *this; }
		self_type operator++(int) { self_type tmp = *this; this->increment(); return tmp; }

		self_type& operator--() { this->decrement(); return *this; }
		self_type operator--(int) { self_type tmp = *this; this->decrement(); return tmp; }
	};

	template <typename _Node, typename _Order>
	struct _bitree_iterator :
		_bitree_iterator_impl<_Order> {
	protected:
		typedef _bitree_iterator_impl<_Order>   base_type;
		typedef _bitree_iterator<_Node, _Order> self_type;
		typedef _Node                           node_type;
		typedef _Node*                          link_type;

	public:
		typedef typename node_type::value_type value_type;
		typedef value_type&                    reference;
		typedef value_type*                    pointer;

	public:
		_bitree_iterator() = default;
		_bitree_iterator(const self_type& other) : base_type(other) { }
		explicit _bitree_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(this->node)->value; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() { this->increment(); return *this; }
		self_type operator++(int) { self_type tmp = *this; this->increment(); return tmp; }

		self_type& operator--() { this->decrement(); return *this; }
		self_type operator--(int) { self_type tmp = *this; this->decrement(); return tmp; }
	};

	template <
		typename _Node,
		typename _Order     = traversal::inorder,
		typename _Allocator = std::allocator<_Node>
	>
	class _bitree_base {
	protected:
		typedef _Node      node_type;
		typedef _Order     order_type;
		typedef node_type* link_type;

		typedef _bitree_base<_Node, _Order,_Allocator> self_type;

	public:
		typedef typename node_type::value_type value_type;
		typedef value_type&                    reference;
		typedef const value_type&              const_reference;
		typedef value_type*                    pointer;
		typedef const value_type*              const_pointer;

		typedef size_t size_type;

		typedef standard_alloc<node_type, _Allocator> allocator_type;

	protected:
		link_type header;
		size_type count;

	protected:
		typedef _bitree_iterator<node_type, order_type>       inner_iterator;
		typedef _const_bitree_iterator<node_type, order_type> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		typedef _reverse_iterator<iterator>       reverse_iterator;
		typedef _reverse_iterator<const_iterator> const_reverse_iterator;

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

		link_type& root() const { return (link_type&) header->parent; }
		link_type& first() const { return (link_type&) header->left; }
		link_type& last() const { return (link_type&) header->right; }

	private:
		void _initialize() {
			header = get_node();

			root()  = nullptr;
			first() = header;
			last()  = header;
		}

		void _clear() {
			if (empty()) {
				return;
			}
			_erase(root());
		}

		bool _has_left(link_type p) const {
			return (nullptr != p->left && header != p->left);
		}

		bool _has_right(link_type p) const {
			return (nullptr != p->right && header != p->right);
		}

		bool _leaf(link_type p) const {
			return !_has_left(p) && !_has_right(p);
		}

		void _create_root_aux(traversal::preorder) { last()->left = header; }

		void _create_root_aux(traversal::inorder) { }

		void _create_root_aux(traversal::postorder) { first()->right = header; }

		void _insert_left_aux(link_type  parent  ,
		                      link_type  new_node,
		                      traversal::preorder) {
			if (parent == last()) {
				new_node->left = header;
				last() = new_node;
			}
		}

		void _insert_left_aux(link_type  parent  ,
		                      link_type  new_node,
		                      traversal::inorder ) {
			if (parent == first()) { first() = new_node; }
		}

		void _insert_left_aux(link_type  parent   ,
		                      link_type  new_node ,
		                      traversal::postorder) {
			if (parent == first()) {
				parent->right = nullptr;
				new_node->right = header;
				first() = new_node;
			}
			else {
				link_type p = first();
				while (header != p && parent != p) {
					p = (link_type) p->parent;
				}
				if (parent == p) {
					first()->right = nullptr;
					new_node->right = header;
					first() = new_node;
				}
			}
		}

		void _insert_right_aux(link_type  parent  ,
		                       link_type  new_node,
		                       traversal::preorder) {
			if (parent == last()) {
				parent->left = nullptr;
				new_node->left = header;
				last() = new_node;
			}
			else {
				link_type p = last();
				while (header != p && parent != p) {
					p = (link_type) p->parent;
				}
				if (parent == p) {
					last()->left = nullptr;
					new_node->left = header;
					last() = new_node;
				}
			}
		}

		void _insert_right_aux(link_type  parent  ,
		                       link_type  new_node,
		                       traversal::inorder ) {
			if (parent == last()) { last() = new_node; }
		}

		void _insert_right_aux(link_type  parent   ,
		                       link_type  new_node ,
		                       traversal::postorder) {
			if (parent == first()) {
				new_node->right = header;
				first() = new_node;
			}
		}

		void _locate_boundary() {
			if (nullptr == root()) {
				first() = header;
				last()  = header;
				return;
			}

			_locate_boundary_aux(order_type());
		}

		void _locate_boundary_aux(traversal::preorder) {
			link_type p = root();
			while (!_leaf(p)) {
				p = (link_type) (nullptr != p->right ? p->right : p->left);
			}
			last() = p;
			p->left = header;
		}

		void _locate_boundary_aux(traversal::inorder) {
			link_type p = root();
			link_type q = root();
			while (nullptr != p->left) {
				p = (link_type) p->left;
			}
			while (nullptr != q->right) {
				q = (link_type) q->right;
			}
			first() = p; last() = q;
		}

		void _locate_boundary_aux(traversal::postorder) {
			link_type p = root();
			while (!_leaf(p)) {
				p = (link_type) (nullptr != p->left ? p->left : p->right);
			}
			first() = p;
			p->right = header;
		}

		size_type _remove_tree(link_type root) {
			if (nullptr == root || header == root) { return 0; }

			size_type num_of_left  = _remove_tree((link_type) root->left );
			size_type num_of_right = _remove_tree((link_type) root->right);

			link_type parent = (link_type) root->parent;
			if (header == parent) {
				parent->parent = nullptr;
			}
			else if (parent->left == root) {
				parent->left = nullptr;
			}
			else {
				parent->right = nullptr;
			}

			destroy_node(root);
			return 1 + num_of_left + num_of_right;
		}

		/* root 非空 */
		link_type _erase(link_type root) {
			link_type parent = (link_type) root->parent;
			count -= _remove_tree(root);
			_locate_boundary();
			return parent;
		}

	public:
		_bitree_base() : count(0) { _initialize(); }
		~_bitree_base() { _clear(); put_node(header); }

	public:
		bool empty() const { return 0 == count; }
		size_type size() const { return count; }
		size_type max_size() const { return size_type(-1); }

		iterator begin() { return inner_iterator(first()); }
		const_iterator begin() const { return const_inner_iterator(first()); }

		iterator end() { return inner_iterator(header); }
		const_iterator end() const { return const_inner_iterator(header); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		bool leaf(const_iterator pos) const {
			return _leaf(pos.base().node);
		}

		std::pair<iterator, bool> create_root(const value_type& val) {
			bool no_root = empty();
			if (no_root) {
				link_type new_node = create_node(val);
				root() = new_node; first() = new_node; last() = new_node;
				new_node->parent = header;
				_create_root_aux(order_type());
				++count;
			}

			return std::pair<iterator, bool>(
				inner_iterator(root()), no_root
			);
		}

		std::pair<iterator, bool>
			insert_left(const_iterator    pos,
			            const value_type& val) {
			if (empty() || end() == pos) {
				return std::pair<iterator, bool>(end(), false);
			}

			link_type parent  = (link_type) pos.base().node;
			bool no_left =
				(nullptr == parent->left || header == parent->left);

			if (no_left) {
				link_type new_node = create_node(val);
				parent->left = new_node;
				new_node->parent = parent;
				_insert_left_aux(parent, new_node, order_type());
				++count;
			}

			return std::pair<iterator, bool>(
				inner_iterator((link_type) parent->left), no_left
			);
		}

		std::pair<iterator, bool>
			insert_right(const_iterator    pos,
			             const value_type& val) {
			if (empty() || end() == pos) {
				return std::pair<iterator, bool>(end(), false);
			}

			link_type parent = (link_type) pos.base().node;
			bool no_right =
				(nullptr == parent->right || header == parent->right);

			if (no_right) {
				link_type new_node = create_node(val);
				parent->right = new_node;
				new_node->parent = parent;
				_insert_right_aux(parent, new_node, order_type());
				++count;
			}

			return std::pair<iterator, bool>(
				inner_iterator((link_type) parent->right), no_right
			);
		}

		iterator erase_all(const_iterator pos) {
			if (empty() || end() == pos || const_iterator() == pos) {
				throw std::overflow_error("Invalid iterator or empty tree.");
			}

			link_type parent = _erase((link_type) pos.base().node);
			return inner_iterator(parent);
		}
	};

	template <typename _Node>
	using _bstree_iterator =
		_bitree_iterator<_Node, traversal::inorder>;

	template <typename _Node>
	using _const_bstree_iterator =
		_const_bitree_iterator<_Node, traversal::inorder>;

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator
	>
	struct _simple_bstree_node : _simple_bitree_node<_Val> {

		typedef _bitree_node_base base_type;
		typedef _Val              value_type;
		typedef _Key              key_type;
		typedef _Comparator       comparator_type;

		typedef _simple_bstree_node<_Key, _Val, _KeyOf, _Comparator> self_type;

		value_type      value;

		explicit _simple_bstree_node(const value_type& val) : value(val) { }
		virtual ~_simple_bstree_node() = default;

		key_type get_key() const { return _KeyOf()(value); }
	};

	template <
		typename _Node,
		typename _Key,
		typename _KeyOf,
		typename _Comparator = less<_Key>,
		typename _Allocator  = std::allocator<_Node>
	>
	class _bstree_base :
		public _bitree_base<_Node, traversal::inorder, _Allocator> {
	protected:
		typedef _bitree_base<_Node, traversal::inorder, _Allocator>        base_type;
		typedef _bstree_base<_Node, _Key, _KeyOf, _Comparator, _Allocator> self_type;
		typedef typename base_type::node_type                              node_type;
		typedef typename base_type::link_type                              link_type;
		typedef typename base_type::order_type                             order_type;
		typedef _Comparator                                                comparator_type;

	public:
		typedef _Key                           key_type;
		typedef typename node_type::value_type value_type;
		typedef value_type&                    reference;
		typedef const value_type&              const_reference;
		typedef value_type*                    pointer;
		typedef const value_type*              const_pointer;
		typedef typename base_type::size_type  size_type;

	protected:
		comparator_type comparator;

	protected:
		link_type& root() const { return base_type::root(); }
		link_type& first() const { return base_type::first(); }
		link_type& last() const { return base_type::last(); }

	public:
		explicit _bstree_base(const comparator_type& comp = _Comparator()) :
			comparator(comp) { }

	protected:
		typedef typename base_type::inner_iterator       inner_iterator;
		typedef typename base_type::const_inner_iterator const_inner_iterator;

	public:
		typedef typename base_type::iterator       iterator;
		typedef typename base_type::const_iterator const_iterator;

	private:

		link_type _insert(link_type         current,
		                  link_type         parent ,
		                  const value_type& val    ) {
			assert(nullptr == current);

			auto key_of = _KeyOf();
			link_type new_node = nullptr;

			if (comparator(key_of(val), key_of(parent->value))) {
				new_node = base_type::create_node(val);
				parent->left = new_node;

				if (parent == first()) {
					first() = new_node;
				}
			}
			else {
				new_node = base_type::create_node(val);
				parent->right = new_node;
				if (parent == last()) {
					last() = new_node;
				}
			}
			new_node->parent = parent;
			++base_type::count;

			return new_node;
		}

	private:
		std::pair<iterator, bool> create_root(const value_type& val) = delete;

		std::pair<iterator, bool>
			insert_left(const_iterator    pos,
		                const value_type& val) = delete;

		std::pair<iterator, bool>
			insert_right(const_iterator    pos,
		                 const value_type& val) = delete;

		iterator erase_all(const_iterator pos) = delete;

	public:
		iterator find(const key_type& key) {
			link_type parent  = base_type::header;
			link_type current = root();

			auto key_of = _KeyOf();

			while (nullptr != current) {
				if (!comparator(key_of(current->value), key)) {
					parent = current;
					current = (link_type) current->left;
				}
				else {
					current = (link_type) current->right;
				}
			}

			return (base_type::header == parent || comparator(key, key_of(parent->value))) ?
			            base_type::end() : iterator(inner_iterator(parent));
		}

		iterator insert_equal(const value_type& val) {
			if (this->empty()) {
				return base_type::create_root(val).first;
			}

			link_type parent = base_type::header;
			link_type current = root();
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				current = static_cast<link_type>(
					comparator(key_of(val), key_of(current->value)) ?
						current->left : current->right
				);
			}

			return inner_iterator(_insert(current, parent, val));
		}

		std::pair<iterator, bool> insert_unique(const value_type& val) {
			if (this->empty()) {
				return std::make_pair<iterator, bool>(
					base_type::create_root(val).first, true
				);
			}

			link_type parent = base_type::header;
			link_type current = root();
			bool prior_to = true;
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				prior_to = comparator(key_of(val), key_of(current->value));
				current = static_cast<link_type>(
					prior_to ? current->left : current->right
				);
			}

			assert(nullptr == current);

			inner_iterator iter(parent);
			if (prior_to) {
				if (first() == parent) {
					return std::pair<iterator, bool>(
						_insert(current, parent, val), true
					);
				}
				--iter;
			}

			if (comparator(key_of(parent->value), key_of(val))) {
				return std::pair<iterator, bool>(
					_insert(current, parent, val), true
				);
			}

			return std::pair<iterator, bool>(iterator(iter), false);
		}

		iterator erase(const_iterator pos) {
			// todo
		}
	};

	class _bstree_tool {
	public:
		typedef _bitree_node_base* base_ptr;

		static base_ptr minimum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->left) {
				root = root->left;
			}
			return root;
		}

		static base_ptr maximum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->right) {
				root = root->right;
			}
			return root;
		}
	};
}

#endif //_TREE_BASE_H_

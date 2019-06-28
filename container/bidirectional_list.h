/*
 * Created by Maou Lim on 2019/1/10.
 */

#ifndef _BIDIRECTIONAL_LIST_H_
#define _BIDIRECTIONAL_LIST_H_

#include <cassert>
#include <memory>
#include "memory.h"
#include "algorithm.h"

namespace tools {

	struct bilist_node_base {

		typedef bilist_node_base* base_ptr;

		base_ptr prev;
		base_ptr next;

		explicit bilist_node_base(
			base_ptr _prev = nullptr, base_ptr _next = nullptr
		) : prev(_prev), next(_next) { }

		static base_ptr last(base_ptr p, base_ptr terminator = nullptr) {
			if (nullptr == p) { return nullptr; }

			while (terminator != p->next) {
				p = p->next;
			}
			return p;
		}

		static base_ptr first(base_ptr p, base_ptr terminator = nullptr) {
			if (nullptr == p) { return nullptr; }

			while (terminator != p->prev) {
				p = p->prev;
			}
			return p;
		}
	};

	template <typename _Val>
	struct bilist_node : bilist_node_base {

		typedef bilist_node<_Val>* link_type;
		typedef _Val               value_type;

		value_type val;

		explicit bilist_node(const value_type& _val) : val(_val) { }

		link_type& get_next() const { return (link_type&) this->next; }
		link_type& get_prev() const { return (link_type&) this->prev; }
	};

	struct bilist_iterator_base {
		typedef bilist_node_base::base_ptr      base_ptr;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;

		base_ptr node;

		bilist_iterator_base() : node(nullptr) { }
		explicit bilist_iterator_base(base_ptr p) : node(p) { }

		void increment() { node = node->next; }
		void decrement() { node = node->prev; }
	};

	//todo iterators

	template <typename _Val>
	struct bilist_const_iterator : bilist_iterator_base {
	public:
		typedef _Val        value_type;
		typedef const _Val& reference;
		typedef const _Val* pointer;

	protected:
		typedef bilist_const_iterator<_Val>           self_type;
		typedef bilist_iterator_base                  base_type;
		typedef typename bilist_node<_Val>::link_type link_type;

	public:
		bilist_const_iterator() = default;
		bilist_const_iterator(const self_type&) = default;
		bilist_const_iterator(const base_type& other) : base_type(other.node) { }
		explicit bilist_const_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(node)->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			increment();
			return *this;
		}

		self_type operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}

		self_type& operator--() {
			decrement();
			return *this;
		}

		self_type operator--(int) {
			self_type tmp = *this;
			decrement();
			return tmp;
		}
	};

	template <typename _Val>
	struct bilist_iterator : bilist_iterator_base {
	public:
		typedef _Val  value_type;
		typedef _Val& reference;
		typedef _Val* pointer;

	protected:
		typedef bilist_iterator<_Val>                 self_type;
		typedef bilist_iterator_base                  base_type;
		typedef typename bilist_node<_Val>::link_type link_type;

	public:
		bilist_iterator() = default;
		bilist_iterator(const self_type&) = default;
		bilist_iterator(const base_type& other) : base_type(other.node) { }
		explicit bilist_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(node)->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			increment();
			return *this;
		}

		self_type operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}

		self_type& operator--() {
			decrement();
			return *this;
		}

		self_type operator--(int) {
			self_type tmp = *this;
			decrement();
			return tmp;
		}
	};

	inline bool operator==(const bilist_iterator_base& left,
	                       const bilist_iterator_base& right) {
		return left.node == right.node;
	}

	template <
	    typename _Val, typename _Allocator = std::allocator<bilist_node<_Val>>
	>
	class bidirectional_list {

	private:
		typedef bidirectional_list<_Val, _Allocator>  self_type;
		typedef bilist_node<_Val>                     node_type;
		typedef node_type*                            link_type;
		typedef standard_alloc<node_type, _Allocator> allocator_type;

	public:
		typedef size_t            size_type;
		typedef ptrdiff_t         difference_type;

		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

	private:
		link_type m_before_head;

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { allocator_type::deallocate(p); }

		template <typename... _Args>
		link_type create_node(_Args&&... args) {
			link_type p = get_node();
			construct(p, std::forward<_Args>(args)...);
			return p;
		}

		void destroy_node(link_type p) {
			destroy(p);
			put_node(p);
		}

		link_type& head() const { return m_before_head->get_next(); }
		link_type& tail() const { return m_before_head->get_prev(); }

	private:
		void _initialize() {
			m_before_head = get_node();
			if (nullptr == m_before_head) {
				throw std::bad_alloc();
			}
			head() = m_before_head;
			tail() = m_before_head;
		}

		void _clear() { while (!empty()) { _erase(head()); } }

		link_type _erase(link_type p) {
			link_type prior = p->get_prev();
			link_type next  = p->get_next();

			prior->next = next;
			next->prev  = prior;

			destroy_node(p);
			return next;
		}

		template <typename... _Args>
		link_type _emplace(link_type p, _Args&&... args) {
			link_type new_node = create_node(std::forward<_Args>(args)...);
			link_type prior = p->get_prev();

			prior->next = new_node;
			new_node->prev = prior;

			new_node->next = p;
			p->prev = new_node;

			return new_node;
		}

	protected:
		typedef bilist_iterator<_Val>       inner_iterator;
		typedef bilist_const_iterator<_Val> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;
		typedef _reverse_iterator<iterator>                        reverse_iterator;
		typedef _reverse_iterator<const_iterator>                  const_reverse_iterator;

	public:
		bidirectional_list() { _initialize(); }

		template <typename _InputIterator>
		bidirectional_list(_InputIterator first, _InputIterator last) {
			_initialize();
			while (first != last) {
				this->push_back(*first); ++first;
			}
		}

		~bidirectional_list() {
			_clear();
			put_node(m_before_head);
		}

		const_reference operator[](size_type index) const {
			size_type i = 0;
			link_type cursor = head();
			while (m_before_head != cursor && i < index) {
				cursor = cursor->get_next();
				++index;
			}

			if (m_before_head == cursor) {
				throw std::overflow_error("Index out of bound.");
			}

			return cursor->val;
		}

		reference operator[](size_type index) {
			return const_cast<reference>(((const self_type*) this)->operator[](index));
		}

		bool empty() const { return head() == m_before_head; }

		size_type size() const {
			size_type count = 0;

			link_type cursor = head();
			while (m_before_head != cursor) {
				++count;
				cursor = cursor->get_next();
			}

			return count;
		}

		size_type max_size() const { return size_type(-1); }

		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }
		const_reference front() const { return head()->val; }

		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }
		const_reference back() const { return tail()->val; }

		void clear() { _clear(); }

		void swap(self_type& other) { tools::swap(this->m_before_head, other.m_before_head); }

		void pop_front() {
			if (empty()) {
				throw std::overflow_error("Empty list.");
			}
			_erase(head());
		}

		void pop_back() {
			if (empty()) {
				throw std::overflow_error("Empty list.");
			}
			_erase(tail());
		}

		void push_front(const value_type& val) { _emplace(head(), val); }

		void push_back(const value_type& val) { _emplace(m_before_head, val); }

		iterator insert(const_iterator pos, const value_type& val) {
			return inner_iterator(_emplace(pos.base(), val));
		}

		iterator erase(const_iterator pos) {
			if (empty()) {
				throw std::overflow_error("Empty list.");
			}
			_erase(pos.base());
		}

		template <typename... _Args>
		iterator emplace(const_iterator pos, _Args&&... args) {
			return inner_iterator(_emplace(pos.base(), std::forward<_Args>(args)...));
		}

		template <typename... _Args>
		iterator emplace_front(_Args&&... args) { return emplace(begin(), std::forward<_Args>(args)...); }

		template <typename... _Args>
		iterator emplace_back(_Args&&... args) { return emplace(end(), std::forward<_Args>(args)...); }

		iterator begin() { return inner_iterator(head()); }
		const_iterator begin() const { return const_inner_iterator(head()); }

		iterator end() { return inner_iterator(m_before_head); }
		const_iterator end() const { return const_inner_iterator(m_before_head); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	};
}

#endif //_BIDIRECTIONAL_LIST_H_

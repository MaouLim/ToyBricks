/*
 * Created by Maou Lim on 2018/1/22.
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include "../common/functor.h"
#include "sequence.h"
#include "iterator.h"

namespace tools {

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _push_heap(_RandomAccessIterator first,
	                       _RandomAccessIterator last ,
	                       _Comparator           comp ) {
		typedef typename
			_iterator_traits<_RandomAccessIterator>::difference_type
		difference_type;

		_push_heap(first, last - first - 1, difference_type(0), *(last - 1), comp);
	}

	template <
		typename _RandomAccessIterator,
		typename _ValueType,
		typename _Difference,
		typename _Comparator
	>
	void _push_heap(_RandomAccessIterator base ,
	                _Difference           hole ,
	                _Difference           root ,
	                _ValueType            value,
	                _Comparator           comp ) {
		_Difference parent = (hole - 1) / 2;

		while (root < hole && comp(*(base + parent), value)) {
			*(base + hole) = *(base + parent);
			hole = parent;
			parent = (hole - 1) / 2;
		}

		*(base + hole) = value; /* 值类型允许拷贝 */
	}

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _pop_heap(_RandomAccessIterator first,
	                      _RandomAccessIterator last ,
	                      _Comparator           comp ) {
		_pop_heap(first, last - 1, last - 1, *(last - 1), comp);
	}

	template <
		typename _RandomAccessIterator,
		typename _ValueType,
		typename _Comparator
	>
	inline void _pop_heap(_RandomAccessIterator first ,
	                      _RandomAccessIterator last  ,
	                      _RandomAccessIterator result,
	                      _ValueType            value ,
	                      _Comparator           comp  ) {
		typedef typename
			_iterator_traits<_RandomAccessIterator>::difference_type
		difference_type;

		*result = *first;
		_adjust_heap(first, difference_type(0), last - first, value, comp);
	};

	template <
		typename _RandomAccessIterator,
		typename _ValueType,
		typename _Difference,
		typename _Comparator
	>
	inline void _adjust_heap(_RandomAccessIterator base  ,
	                         _Difference           hole  ,
	                         _Difference           length,
	                         _ValueType            value ,
							 _Comparator           comp  ) {
		_Difference child = hole * 2 + 2;

		while (child < length) {
			if (comp(*(base + child), *(base + child - 1))) {
				--child;
			}
			*(base + hole) = *(base + child);
			hole = child;
			child = child * 2 + 2;
		}

		if (child == length) {
			*(base + hole) = *(base + child - 1);
			hole = child - 1;
		}

		*(base + hole) = value;
	};

	template <typename _RandomAccessIterator, typename _Comparator>
	void _heap_sort(_RandomAccessIterator first,
	                _RandomAccessIterator last ,
	                _Comparator           comp ) {
		while (1 < last - first) {
			_pop_heap(first, last--, comp);
		}
	};

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _make_heap(_RandomAccessIterator first,
	                       _RandomAccessIterator last ,
						   _Comparator           comp ) {
		typedef typename
			_iterator_traits<_RandomAccessIterator>::value_type
		value_type;

		typedef typename
			_iterator_traits<_RandomAccessIterator>::difference_type
		difference_type;

		if (last - first < 2) {
			return;
		}

		difference_type length = last - first;
		difference_type root = (length - 2) / 2;

		while (0 <= root) {
			_adjust_heap(first, root, length, value_type(*(first + root)), comp);
			if (0 == root) { return; }
			--root;
		}
	}

	template <
		typename _Val,
		typename _Container  = tools::sequence<_Val>,
		typename _Comparator = tools::less<_Val>
	>
	class priority_queue {
	public:
		typedef typename _Container::value_type      value_type;
		typedef typename _Container::pointer         pointer;
		typedef typename _Container::const_pointer   const_pointer;
		typedef typename _Container::reference       reference;
		typedef typename _Container::const_reference const_reference;

		typedef typename _Container::size_type       size_type;

	protected:
		typedef _Container  container_type;
		typedef _Comparator comparator_type;

		typedef priority_queue<_Val, _Container, _Comparator> self_type;

	private:
		container_type  m_container;
		comparator_type m_comp;

	public:
		explicit priority_queue(const comparator_type& comp = _Comparator()) : m_comp(comp) { }

		template <typename _InputIterator>
		priority_queue(_InputIterator         first,
		               _InputIterator         last ,
		               const comparator_type& comp = _Comparator()) :
			m_container(first, last), m_comp(comp) {
			_make_heap(m_container.begin(), m_container.end(), m_comp);
		}

		const comparator_type& comparator() const { return m_comp; }

		bool empty() const { return m_container.empty(); }
		size_type size() const { return m_container.size(); }
		size_type max_size() const { return m_container.max_size(); }

		void clear() { m_container.clear(); }
		void swap(self_type& other) { m_container.swap(other.m_container); }

		const_reference top() const { return m_container.front(); }

		void push(const value_type& value) {
			m_container.push_back(value);
			_push_heap(m_container.begin(), m_container.end(), m_comp);
		}

		void pop() {
			_pop_heap(m_container.begin(), m_container.end(), m_comp);
			m_container.pop_back();
		}

		template <typename... _Args>
		void emplace(_Args&&... args) {
			m_container.emplace_back(std::forward<_Args>(args)...);
			_push_heap(m_container.begin(), m_container.end(), m_comp);
		}
	};

}

#endif //_HEAP_H_

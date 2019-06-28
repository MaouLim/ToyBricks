/*
 * Created by Maou Lim on 2019/1/20.
 */

#ifndef _ADJACENCY_LIST_H_
#define _ADJACENCY_LIST_H_

#include "bidirectional_list.h"
#include "pair.h"
#include "sequence.h"

namespace tools {

	template <
	    typename _Val, typename _Link = size_t
	>
	struct _link {
		typedef _Val  value_type;
		typedef _Link link_type;

		value_type val;
		link_type  link_to;

		_link(const value_type& _val, link_type _to) : val(_val), link_to(_to) { }
	};

	template <typename _Val, _Val _Unreachable>
	class adjacency_list;

	template <typename _Val, typename _LinkItr, _Val _Unreachable>
	struct _adjacency_list_itr_base {
	protected:
		typedef adjacency_list<_Val, _Unreachable> list_type;
		typedef typename list_type::iterator       vertex_iterator;
		typedef _LinkItr                           link_iterator;
	public:
		typedef _Val                               value_type;
		typedef std::forward_iterator_tag          iterator_category;
		typedef ptrdiff_t                          difference_type;
		typedef typename list_type::size_type      size_type;

		vertex_iterator        start_with;
		link_iterator          current;
		const list_type* const list;

		_adjacency_list_itr_base(
			vertex_iterator _vi, link_iterator _li, const list_type* const _list
		) : start_with(_vi), current(_li), list(_list) { }

		void increment() {
			++current;
			if (start_with->end() == current) {
				++start_with;
				while (
					list->end() != start_with && start_with->empty()
				) { ++start_with; }

				if (list->end() == start_with) {
					current = link_iterator(nullptr);
				}
				else { current = start_with->begin(); }
			}
		}

		bool is_end() const { return list->end() == start_with || start_with->end() == current; }
	};

	template <
	    typename _Val, typename _LinkItr, _Val _Unreachable
	>
	class _adjacency_list_itr :
		public _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable> {

		typedef _adjacency_list_itr<_Val, _LinkItr, _Unreachable>      self_type;
		typedef _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable> base_type;
		typedef typename base_type::link_iterator                      link_iterator;
		typedef typename base_type::vertex_iterator                    vertex_iterator;
		typedef typename base_type::list_type                          list_type;

	public:
		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::value_type        value_type;
		typedef value_type*                           pointer;
		typedef value_type&                           reference;
		typedef typename base_type::difference_type   difference_type;
		typedef typename base_type::size_type         size_type;

		_adjacency_list_itr(
			vertex_iterator _vi, link_iterator _li, const list_type* const _list
		) : base_type(_vi, _li, _list) { }

		reference operator*() const { return *base_type::current; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			base_type::increment();
			return *this;
		}

		self_type operator++(int) {
			auto tmp = *this;
			this->operator++();
			return tmp;
		}
	};

	template <typename _Val, typename _LinkItr, _Val _Unreachable>
	class _const_adjacency_list_itr :
		public _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable> {

		typedef _const_adjacency_list_itr<_Val, _LinkItr, _Unreachable> self_type;
		typedef _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable>  base_type;
		typedef typename base_type::link_iterator                       link_iterator;
		typedef typename base_type::vertex_iterator                     vertex_iterator;
		typedef typename base_type::list_type                           list_type;

	public:
		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::value_type        value_type;
		typedef const value_type*                     pointer;
		typedef const value_type&                     reference;
		typedef typename base_type::difference_type   difference_type;
		typedef typename base_type::size_type         size_type;

		_const_adjacency_list_itr(
			vertex_iterator _vi, link_iterator _li, const list_type* const _list
		) : base_type(_vi, _li, _list) { }

		reference operator*() const { return base_type::current->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			base_type::increment();
			return *this;
		}

		self_type operator++(int) {
			auto tmp = *this;
			this->operator++();
			return tmp;
		}
	};

	template <typename _Val, typename _LinkItr, _Val _Unreachable>
	bool operator==(
		const _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable>& left,
		const _adjacency_list_itr_base<_Val, _LinkItr, _Unreachable>& right
	) {
		if (left.is_end() && right.is_end()) { return true; }
		return left.current == right.current && left.start_with == right.start_with;
	}

	template <typename _Val, _Val _Unreachable = static_cast<_Val>(0)>
	class adjacency_list {
	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef size_t      size_type;

		static const value_type unreachable;

	private:
		typedef _link<_Val, size_type>               link_type;
		typedef tools::bidirectional_list<link_type> element_type;
		typedef tools::sequence<element_type>        rep_type;
		typedef adjacency_list<_Val, _Unreachable>   self_type;

		typedef _adjacency_list_itr<
		    _Val, typename element_type::iterator, _Unreachable
		> inner_iterator;

		typedef _const_adjacency_list_itr<
			_Val, typename element_type::iterator, _Unreachable
		> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		adjacency_list() : m_count_elements(0) { }

		reference operator()(size_type u, size_type v) {
			return const_cast<reference>(((const self_type*) this)->operator()(u, v));
		}

		const_reference operator()(size_type u, size_type v) const {
			assert(u < m_rep.size() && v < m_rep.size());

			const element_type& list = m_rep[u];
			for (const auto& each : list) {
				if (v == each.link_to) { return each.val; }
			}
			return unreachable;
		}

		bool empty() const { return 0 == m_count_elements; }
		bool zero() const { return m_rep.empty(); }

		size_type rank() const { return m_rep.size(); }
		size_type size() const { return m_count_elements; }

		iterator find_link(size_type u, size_type v) {
			assert(u < m_rep.size() && v < m_rep.size());

			auto start_with = m_rep.begin() + u;
			auto cursor = start_with->begin();
			while (start_with->end() != cursor) {
				if (v == cursor->link_to) { break; }
				++cursor;
			}

			return inner_iterator(start_with, cursor, this);
		}

		tools::pair<iterator, bool>
		    insert_or_replace_link(
		    	const_iterator hint, const value_type& val
		) {
			assert(m_rep.end() != hint.base().start_with);
			if (end() == hint) {
				auto& start_with = hint.base().start_with;
				size_type offset = start_with - m_rep.begin();
				auto& current = hint.base().current;
				auto new_link = start_with->emplace(current, val, offset);
				++m_count_elements;
				return tools::make_pair(iterator(inner_iterator(start_with, new_link, this)), true);
			}

			hint.base().current->val = val;
			return tools::make_pair(hint, false);
		}

		tools::pair<iterator, bool> erase_link(const_iterator pos) {
			if (end() == pos) { return tools::make_pair(end(), false); }
			auto& start_with = pos.base().start_with;
			auto next = start_with->erase(pos.base().current);
			if (start_with->empty()) {
				while (m_rep.end() != start_with && start_with->empty()) {
					++start_with;
				}

				next = m_rep.end() == start_with ? element_type::iterator(nullptr) : start_with->begin();
			}
			return tools::make_pair(iterator(inner_iterator(start_with, next, this)), true);
		}

		iterator begin() {
			auto itr = m_rep.begin();
			while (
				m_rep.end() != itr && itr->empty()
			) { ++itr; }

			return inner_iterator(
				itr, m_rep.end() == itr ? element_type::iterator(nullptr) : itr->begin(), this
			);
		}

		const_iterator begin() const {
			auto itr = m_rep.begin();
			while (
				m_rep.end() != itr && itr->empty()
			) { ++itr; }

			return const_inner_iterator(
				itr, m_rep.end() == itr ? element_type::iterator(nullptr) : itr->begin(), this
			);
		}

		iterator end() { return inner_iterator(m_rep.end(), element_type::iterator(nullptr), this); }
		const_iterator end() const { return const_inner_iterator(m_rep.end(), element_type::iterator(nullptr), this); }

	private:
		size_type m_count_elements;
		rep_type  m_rep;
	};

	template <typename _Val, _Val _Unreachable>
	const _Val adjacency_list<_Val, _Unreachable>::unreachable = _Unreachable;
}

#endif //_ADJACENCY_LIST_H_

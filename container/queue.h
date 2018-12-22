/*
 * Created by Maou Lim on 2018/1/22.
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "sequence.h"

namespace tools {

	template <typename _Val, typename _Container = tools::sequence<_Val>>
	class queue {
	public:
		typedef typename _Container::value_type      value_type;
		typedef typename _Container::pointer         pointer;
		typedef typename _Container::const_pointer   const_pointer;
		typedef typename _Container::reference       reference;
		typedef typename _Container::const_reference const_reference;

		typedef typename _Container::size_type       size_type;

	protected:
		typedef queue<_Val, _Container> self_type;
		typedef _Container              container_type;

	private:
		container_type m_container;

	public:
		bool empty() const { return m_container.empty(); }
		size_type size() const { return m_container.size(); }
		size_type max_size() const { return m_container.max_size(); }

		const_reference back() const { return m_container.back(); }
		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }

		const_reference front() const { return m_container.front(); }
		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }

		void pop() { m_container.pop_front(); }
		void push(const value_type& val) { return m_container.push_back(val); }

		template <typename... _Args>
		void emplace(_Args&&... args) { m_container.emplace_back(std::forward<_Args>(args)...); }
	};
}

#endif //_QUEUE_H_

/*
 * Created by Maou Lim on 2018/1/17.
 */

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <memory>
#include <cassert>

#include "iterator.h"
#include "memory.h"
#include "../common/type_base.h"
#include "algorithm.h"

namespace tools {

	template <
		typename _Val,
		typename _Allocator = std::allocator<_Val>
	>
	class sequence {
	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef ptrdiff_t difference_type;
		typedef size_t    size_type;

	protected:
		typedef sequence<_Val, _Allocator>       self_type;
		typedef standard_alloc<_Val, _Allocator> allocator_type;

		typedef pointer       inner_iterator;
		typedef const_pointer const_inner_iterator;

	private:
		inner_iterator m_base;
		inner_iterator m_finish;
		inner_iterator m_end_of_storage;

	protected:
		pointer get_space(size_type n) { return allocator_type::allocate(n); }
		void put_space(pointer p, size_type n) { return allocator_type::deallocate(p, n); }

	private:
		void _initialize_with_n(size_type n) throw (std::bad_alloc) {
			m_base = get_space(n);
			if (nullptr == m_base) {
				throw std::bad_alloc();
			}
			m_end_of_storage = m_base + n;
			m_finish = m_base;
		}

		void _fill(size_type n, const value_type& val) {
			construct(m_base, m_base + n, val);
			m_finish = m_base + n;
		}

		template <typename _ForwardIterator>
		void _fill(_ForwardIterator first, _ForwardIterator last) {
			while (first != last) {
				construct(m_finish++, *first);
				++first;
			}
		}

		void _fill(pointer first, pointer last) {
			size_type offset = last - first;
			memcpy(m_base, first, sizeof (value_type) * offset);
			m_finish = m_base + offset;
		}

		void _extend(size_type new_size) {
			inner_iterator old_base   = m_base;
			inner_iterator old_finish = m_finish;
			inner_iterator old_eos    = m_end_of_storage;

			try {
				_initialize_with_n(new_size);
			}
			catch (...) {
				m_base = old_base;
				m_finish  = old_finish;
				m_end_of_storage = old_eos;
				throw;
			}

			_fill(old_base, old_finish);
			put_space(old_base, old_eos - old_base);
		}

		void _extend_when_full(size_type min_capacity) {
			size_type default_capacity = (0 == capacity() ? 1 : capacity() * 2);
			_extend(default_capacity < min_capacity ? min_capacity : default_capacity);
		}

		void _resize(size_type new_size, const value_type& val) {
			if (capacity() == new_size) {
				return;
			}

			if (capacity() < new_size) {
				_extend(new_size);
			}
			else if (size() < new_size) {
				construct(m_finish, m_base + new_size, val);
				m_finish = m_base + new_size;
			}
			else {
				destroy(m_base + new_size, m_finish);
				m_finish = m_base + new_size;
			}
		}

		void _clear_aux(_true_type) { }

		void _clear_aux(_false_type) {
			inner_iterator cursor = m_base;
			while (m_finish != cursor) {
				destroy(cursor);
				++cursor;
			}
		}

		void _destroy() {
			clear();
			put_space(m_base, capacity());
		}

		bool _full() const { return m_finish == m_end_of_storage; }

		/* [start, end) */
		void _copy_backward(inner_iterator start    ,
		                    inner_iterator end      ,
		                    inner_iterator new_start) {
			difference_type offset = new_start - start;
			inner_iterator p = end + offset - 1;

			while (new_start <= p) {
				memcpy(p, p - offset, sizeof (value_type));
				--p;
			}
		}

		void _copy_forward(inner_iterator start, inner_iterator end, inner_iterator new_start) {
			while (start != end) {
				memcpy(new_start++, start++, sizeof (value_type));
			}
		}

		template <typename... _Args>
		inner_iterator _emplace(difference_type offset, _Args&&... args) {
			if (_full()) {
				_extend_when_full(1);
			}

			inner_iterator p = m_base + offset;
			_copy_backward(p, m_finish, p + 1);
			construct(p, std::forward<_Args>(args)...);
			++m_finish;

			return p;
		}

		template <typename... _Args>
		inner_iterator _emplace(difference_type offset,
		                        size_type       n,
		                        _Args&&...      args) {
			if (capacity() < size() + n) {
				_extend_when_full(size() + n);
			}

			inner_iterator p = m_base + offset;
			_copy_backward(p, m_finish, p + n);
			construct(p, p + n, std::forward<_Args>(args)...);
			m_finish += n;

			return p;
		}

	public:
		sequence() : m_base(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) { }

		explicit sequence(size_type size) { _initialize_with_n(size); }

		sequence(size_type size, const value_type& val) {
			_initialize_with_n(size);
			_fill(size, val);
		}

		sequence(const self_type& other) {
			_initialize_with_n(other.size());
			_fill(other.begin(), other.end());
		}

		template <typename _ForwardIterator>
		sequence(_ForwardIterator first, _ForwardIterator last) :
			m_base(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) {
			while (first != last) {
				this->push_back(*first);
				++first;
			}
		}

		~sequence() { _destroy(); }

	public:
		self_type& operator=(const self_type& other) {
			if (this == &other) {
				return *this;
			}

			_destroy();
			_initialize_with_n(other.size());
			_fill(other.m_base, other.m_finish);

			return *this;
		}

		const_reference operator[](size_type index) const {
			assert(index < size());
			return *(m_base + index);
		}

		reference operator[](size_type index) {
			return const_cast<reference>(
				((const self_type*) this)->operator[](index)
			);
		}

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		typedef _reverse_iterator<iterator>       reverse_iterator;
		typedef _reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		/* size and capacity */
		bool empty() const { return m_base == m_finish; }
		size_type size() const { return m_finish - m_base; }
		size_type max_size() const { return size_type(-1); }
		size_type capacity() const { return m_end_of_storage - m_base; }

		void resize(size_type new_size) { _resize(new_size, value_type()); }
		void resize(size_type new_size, const value_type& val) { _resize(new_size, val); }

		void reserve(size_type new_capacity) {
			if (new_capacity <= capacity()) {
				return;
			}

			_extend(new_capacity);
		}

		/* accessor */
		reference at(size_type index) { return operator[](index); }
		const_reference at(size_type index) const { return operator[](index); }

		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }
		const_pointer data() const { return m_base; }

		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }
		const_reference front() const { assert(!empty()); return *m_base; }

		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }
		const_reference back() const { assert(!empty()); return *(m_finish - 1); }

		/* modifier */
		void clear() {
			_clear_aux(
				typename _or<typename _is_scalar<value_type>::type,
					typename _is_trivially_destructible<value_type>::type
				>::type()
			);
			m_finish = m_base;
		}

		void swap(self_type& other) {
			inner_iterator base   = this->m_base;
			inner_iterator finish = this->m_finish;
			inner_iterator eos    = this->m_end_of_storage;

			this->m_base = other.m_base;
			this->m_finish = other.m_finish;
			this->m_end_of_storage = other.m_end_of_storage;

			other.m_base = base;
			other.m_finish = finish;
			other.m_end_of_storage = eos;
		}

		template <typename... _Args>
		iterator emplace(const_iterator pos, _Args&&... args) {
			return iterator(_emplace(pos - begin(), std::forward<_Args>(args)...));
		}

		template <typename... _Args>
		void emplace_back(_Args&&... args) {
			_emplace(end() - begin(), std::forward<_Args>(args)...);
		}

		void push_back(const value_type& val) { insert(end(), val); }

		void pop_back() { erase(--end()); }

		iterator insert(const_iterator pos, const value_type& val) {
			return iterator(_emplace(pos - begin(), val));
		}

		iterator insert(const_iterator pos, size_type n, const value_type& val) {
			return iterator(_emplace(pos - begin(), n, val));
		}

		template <typename _InputIterator>
		iterator insert(const_iterator pos, _InputIterator first, _InputIterator last) {
			size_type count = 0;
			auto _first = first;
			while (_first != last) { ++count; ++_first; }

			if (capacity() < size() + count) {
				_extend_when_full(size() + count);
			}

			difference_type offset = pos - begin();

			inner_iterator start = m_base + offset;
			_copy_backward(start, m_finish, start + count);

			while (first != last) {
				construct(start, *first);
				++first; ++start;
			}

			m_finish += count;

			return iterator(pos.base());
		}

		iterator erase(const_iterator pos) {
			if (empty() || end() == pos) {
				throw std::overflow_error("Invalid iterator or empty sequence.");
			}

			difference_type offset = pos - begin();
			inner_iterator to_erase = m_base + offset;

			destroy(to_erase);
			_copy_forward(to_erase + 1, m_finish, to_erase);

			--m_finish;
			return iterator(to_erase);
		}

		/* iterators */
		iterator begin() { return iterator(m_base); }
		const_iterator begin() const { return const_iterator(m_base); }

		iterator end() { return iterator(m_finish); }
		const_iterator end() const { return const_iterator(m_finish); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	};

	template <typename _Val, size_t _Size>
	struct fixed_seq {

		typedef fixed_seq<_Val, _Size> self_type;

	public:

		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef ptrdiff_t difference_type;
		typedef size_t    size_type;

		typedef _iterator_wrapper<pointer, self_type>       iterator;
		typedef _iterator_wrapper<const_pointer, self_type> const_iterator;
		typedef _reverse_iterator<iterator>                 reverse_iterator;
		typedef _reverse_iterator<const_iterator>           const_reverse_iterator;

	public:
		fixed_seq() = default;

		const_reference operator[](size_type index) const {
			assert(index < _Size);
			return elements[index];
		}

		reference operator[](size_type index) {
			return const_cast<reference>(((const self_type*) this)->at(index));
		}

		bool empty() const { return 0 == _Size; }
		size_type max_size() const { return _Size; }
		size_type size() const { return _Size; }

		const_reference at(size_type index) const {
			assert(index < _Size);
			return elements[index];
		}

		reference at(size_type index) {
			return const_cast<reference>(((const self_type*) this)->at(index));
		}

		const_reference front() const { assert(!empty()); return *elements; }
		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }

		const_reference back() const { assert(!empty()); return *(elements + _Size - 1); }
		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }

		const_pointer data() const { return elements; }
		pointer data() { return const_cast<reference>(((const self_type*) this)->data()); }

		void fill(const value_type& val) { tools::fill_n(begin(), _Size, val); }

		void swap(self_type& other) { tools::swap_ranges(begin(), end(), other.begin()); }

		iterator begin() { return iterator(elements); }
		iterator end() { return iterator(elements + _Size); }

		const_iterator begin() const { return const_iterator(elements); }
		const_iterator end() const { return const_iterator(elements + _Size); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		value_type elements[_Size];
	};
};

#endif //_SEQUENCE_H_

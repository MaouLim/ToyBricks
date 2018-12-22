/*
 * Created by Maou Lim on 2018/1/17.
 */

#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include <bits/stl_iterator_base_types.h>

#include "../common/defines.h"

namespace tools {

	template <typename _Iterator>
	struct _iterator_traits {
		typedef typename _Iterator::value_type        value_type;
		typedef typename _Iterator::reference         reference;
		typedef typename _Iterator::pointer           pointer;
		typedef typename _Iterator::iterator_category iterator_category;
		typedef typename _Iterator::difference_type   difference_type;
	};

	template <typename _Pointee>
	struct _iterator_traits<_Pointee*> {
		typedef _Pointee                        value_type;
		typedef _Pointee&                       reference;
		typedef _Pointee*                       pointer;
		typedef std::random_access_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;
	};

	template <typename _Pointee>
	struct _iterator_traits<const _Pointee*> {
		typedef _Pointee                        value_type;
		typedef const _Pointee&                 reference;
		typedef const _Pointee*                 pointer;
		typedef std::random_access_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;
	};

	template <typename _Iterator, typename _Container>
	struct _iterator_wrapper : _iterator_traits<_Iterator> {
	protected:
		typedef _iterator_wrapper<_Iterator, _Container> self_type;
		typedef _iterator_traits<_Iterator>              traits_type;

	public:
		typedef _Iterator                                iterator_type;
		typedef _Container                               container_type;

		typedef typename traits_type::reference            reference;
		typedef typename traits_type::pointer              pointer;
		typedef typename traits_type::difference_type      difference_type;
	protected:
		iterator_type current;

	public:
		_iterator_wrapper() : current(_Iterator()) { }
		_iterator_wrapper(const iterator_type& iter) : current(iter) { }

		template <typename _OtherIter = _Iterator>
		_iterator_wrapper(const _iterator_wrapper<_OtherIter, _Container>& other) :
			current(other.base()) { }

	public:
		const iterator_type& base() const { return current; }
		
		reference operator*() const { return *current; }
		pointer operator->() const { return &operator*(); }

		self_type& operator++() { ++current; return *this; }
		self_type operator++(int) { self_type tmp = *this; ++current; return tmp; }

		self_type& operator--() { --current; return *this; }
		self_type operator--(int) { self_type tmp = *this; --current; return tmp; }

		reference operator[](difference_type n) const { return current[n]; }

		self_type& operator+=(difference_type n) { current += n; return *this; }
		self_type operator+(difference_type n) const { return self_type(current + n); }

		self_type& operator-=(difference_type n) { current -= n; return *this; }
		self_type operator-(difference_type n) const { return self_type(current - n); }
	};
	
	template <typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool operator==(const _iterator_wrapper<_IteratorL, _Container>& left,
		                   const _iterator_wrapper<_IteratorR, _Container>& right) {
		return left.base() == right.base();
	};

	template <typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool operator!=(const _iterator_wrapper<_IteratorL, _Container>& left,
	                       const _iterator_wrapper<_IteratorR, _Container>& right) {
		return !operator==(left, right);
	};

	template <typename _RandomItrL, typename _RandomItrR, typename _Container>
	inline bool operator<(const _iterator_wrapper<_RandomItrL, _Container>& left,
	                      const _iterator_wrapper<_RandomItrR, _Container>& right) {
		return left.base() < right.base();
	}

	template <typename _RandomItrL, typename _RandomItrR, typename _Container>
	inline bool operator>(const _iterator_wrapper<_RandomItrL, _Container>& left,
	                      const _iterator_wrapper<_RandomItrR, _Container>& right) {
		return left.base() > right.base();
	}

	template <typename _RandomItrL, typename _RandomItrR, typename _Container>
	inline bool operator>=(const _iterator_wrapper<_RandomItrL, _Container>& left,
	                       const _iterator_wrapper<_RandomItrR, _Container>& right) {
		return !operator<(left, right);
	}

	template <typename _RandomItrL, typename _RandomItrR, typename _Container>
	inline bool operator<=(const _iterator_wrapper<_RandomItrL, _Container>& left,
	                       const _iterator_wrapper<_RandomItrR, _Container>& right) {
		return !operator>(left, right);
	}

	template <typename _RandomItrL, typename _RandomItrR, typename _Container>
	inline typename _Container::difference_type
		operator-(const _iterator_wrapper<_RandomItrL, _Container>& left,
		          const _iterator_wrapper<_RandomItrR, _Container>& right) {
		return left.base() - right.base();
	}

	template <typename _Iterator>
	struct _reverse_iterator :
		_iterator_traits<_Iterator> {
	protected:
		typedef _Iterator                    iterator_type;
		typedef _iterator_traits<_Iterator>  traits_type;
		typedef _reverse_iterator<_Iterator> self_type;

	public:
		typedef typename traits_type::reference       reference;
		typedef typename traits_type::pointer         pointer;
		typedef typename traits_type::difference_type difference_type;

	protected:
		iterator_type current;

	public:
		_reverse_iterator() = default;
		_reverse_iterator(const self_type& other) : current(other.current) { }
		explicit _reverse_iterator(const iterator_type& iter) : current(iter) { }

	public:
		const iterator_type& base() const { return current; }

		reference operator*() { auto tmp = current; return *(--tmp); }
		pointer operator->() { return &(operator*()); }

		self_type& operator++() { --current; return *this; }
		self_type operator++(int) { auto tmp = *this; --current; return tmp; }

		self_type& operator--() { ++current; return *this; }
		self_type operator--(int) { auto tmp = *this; ++current; return tmp; }

		reference operator[](difference_type n) const { return *(current - n - 1); }

		self_type& operator+=(difference_type n) { current -= n; return *this; }
		self_type operator+(difference_type n) const { return self_type(current - n); }

		self_type& operator-=(difference_type n) { current += n; return *this; }
		self_type operator-(difference_type n) const { return self_type(current + n); }
	};

	template <typename _IteratorL, typename _IteratorR>
	inline bool operator==(const _reverse_iterator<_IteratorL>& left ,
	                       const _reverse_iterator<_IteratorR>& right) {
		return left.base() == right.base();
	}

	template <typename _IteratorL, typename _IteratorR>
	inline bool operator!=(const _reverse_iterator<_IteratorL>& left ,
	                       const _reverse_iterator<_IteratorR>& right) {
		return !operator==(left, right);
	}
}

#endif //_ITERATOR_H_

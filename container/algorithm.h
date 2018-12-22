/*
 * Created by Maou Lim on 2018/2/4.
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

//#include <algorithm>
//#include <functional>

#include "../common/functor.h"
#include "iterator.h"

namespace tools {

	template <typename _Tp>
	_Tp min(const _Tp& left, const _Tp& right) {
		return left < right ? left : right;
	}

	template <typename _Tp>
	_Tp max(const _Tp& left, const _Tp& right) {
		return left < right ? right : left;
	}

	template <typename _Tp>
	void swap(_Tp& left, _Tp& right) {
		_Tp tmp(left);
		left = right;
		right = tmp;
	}

	template <typename _IteratorL, typename _IteratorR>
	void iter_swap(_IteratorL l, _IteratorR r) {
		swap(*l, *r);
	}

	template <typename _ForwardIterator1, typename _ForwardIterator2>
	void swap_ranges(_ForwardIterator1 first1,
					 _ForwardIterator1 last1,
					 _ForwardIterator2 first2) {
		while (first1 != last1) {
			iter_swap(first1, first2);
			++first1; ++first2;
		}
	}

	template <typename _OutputIterator, typename _Size, typename _Val>
	_OutputIterator fill_n(_OutputIterator first, _Size n, const _Val& v) {
		for (_Size i(0); i < n; ++i) {
			*(first++) = v;
		}
	}

	template <typename _InputIterator, typename _OutputIterator>
	_OutputIterator copy(_InputIterator  first,
		                 _InputIterator  last,
		                 _OutputIterator target) {
		while (first != last) {
			*target = *first;
			++target; ++first;
		}
		return target;
	}

	template <typename _InputIterator, typename _Val>
	_Val accumulate(_InputIterator first, _InputIterator last, _Val init) {
		return accumulate(
			first, last, init, plus<_Val, typename _iterator_traits<_InputIterator>::value_type>()
		);
	}

	template <typename _InputIterator, typename _Val, typename _BinaryOp>
	_Val accumulate(_InputIterator first,
	                _InputIterator last ,
	                _Val           init ,
	                _BinaryOp      op   ) {
		while (first != last) {
			init = op(init, *first);
			++first;
		}
		return init;
	}

	template <typename _InputIterator, typename _OutputIterator>
	_OutputIterator adjacent_difference(_InputIterator  first,
	                                    _InputIterator  last,
	                                    _OutputIterator result) {
		return adjacent_difference(
			first, last, result, minus<
				typename _iterator_traits<_InputIterator>::value_type,
				typename _iterator_traits<_OutputIterator>::value_type
			>()
		);
	}

	template <
		typename _InputIterator,
		typename _OutputIterator,
		typename _BinaryOp
	>
	_OutputIterator adjacent_difference(_InputIterator  first,
	                                    _InputIterator  last,
	                                    _OutputIterator result,
	                                    _BinaryOp       op) {
		typedef typename _iterator_traits<_InputIterator>::value_type value_type;

		if (first == last) { return result; }

		*result = *first;
		value_type val = *first;

		while (++first != last) {
			value_type tmp = *first;
			*(++result) = op(tmp, val);
			val = tmp;
		}

		return ++result;
	}

	template <typename _InputItr1, typename _InputItr2, typename _Val>
	_Val inner_product(_InputItr1 first1,
	                   _InputItr1 last1,
	                   _InputItr2 first2,
	                   _Val       init) {
		while (first1 != last1) {
			init += (*first1) * (*first2);
			++first1; ++first2;
		}

		return init;
//		typedef _iterator_traits<_InputItr1>::value_type vt1;
//		typedef _iterator_traits<_InputItr2>::value_type vt2;
//		auto func = multiply<vt1, vt2>().operator();
//		return inner_product(first1, last1, first2, plus<_Val, func::result_type>, multiply<vt1, vt2>());
	}

	template <
		typename _InputItr1,
		typename _InputItr2,
		typename _Val,
		typename _BinaryOp1,
		typename _BinaryOp2
	>
	_Val inner_product(_InputItr1 first1,
	                   _InputItr1 last1,
	                   _InputItr2 first2,
	                   _Val       init,
	                   _BinaryOp1 op1,
	                   _BinaryOp2 op2) {
		while (first1 != last1) {
			init = op1(init, op2(*first1, *first2));
			++first1; ++first2;
		}

		return init;
	}

	template <typename _InputIterator, typename _OutputIterator>
	_OutputIterator partial_sum(_InputIterator  first,
	                            _InputIterator  last,
	                            _OutputIterator result) {
		typedef typename _iterator_traits<_InputIterator>::value_type value_type;
		return _partial_sum(first, last, result, plus<value_type>(), (value_type*) nullptr);
	}

	template <typename _InputIterator, typename _OutputIterator, typename _BinaryOp>
	_OutputIterator partial_sum(_InputIterator  first,
	                            _InputIterator  last,
	                            _OutputIterator result,
	                            _BinaryOp       op) {
		typedef typename _iterator_traits<_InputIterator>::value_type value_type;
		_partial_sum(first, last, result, op, (value_type*) nullptr);
	}

	template <
		typename _InputIterator,
		typename _OutputIterator,
		typename _Val,
		typename _BinaryOp
	>
	_OutputIterator _partial_sum(_InputIterator  first ,
	                             _InputIterator  last  ,
	                             _OutputIterator result,
	                             _BinaryOp       op    ,
	                             _Val*    /* ignored */) {
		if (first == last) { return result; }
		*result = *first;

		_Val val = *first;
		while (++first != last) {
			val = op(val, *first);
			*(++result) = val;
		}
		return ++result;
	}



	template <
		typename _ForwardIterator,
		typename _ValueType
	>
	_ForwardIterator lower_bound(_ForwardIterator  first,
	                             _ForwardIterator  last,
	                             const _ValueType& value) {

	}

	template <
		typename _ForwardIterator,
		typename _ValueType,
		typename _Comparator
	>
	_ForwardIterator lower_bound(_ForwardIterator   first,
	                             _ForwardIterator   last ,
	                             const _ValueType&  value,
	                             const _Comparator& comp) {

	}
}

#endif //_ALGORITHM_H_

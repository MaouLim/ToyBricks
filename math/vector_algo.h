/*
 * Created by Maou Lim on 2019/6/24.
 */

#ifndef _VECTOR_ALGO_H_
#define _VECTOR_ALGO_H_

#include <limits>

#include "../common/functor.h"
#include "math_type.h"

namespace math {

	/* basic algorithms */

	template <
		size_t   _Length,
		typename _Vector,
		typename _VectorOut,
		typename _UnaryOp
	>
	struct _unary_op_impl;

	template <
		typename _Vector,
		typename _UnaryOp
	>
	_Vector& unary_op(_Vector& target, _UnaryOp op) {
		typedef typename _as_vector<_Vector>::vector_type vec_type;
		typedef vector_traits<vec_type> traits_type;
		auto vec_like = as_vector(target);
		_unary_op_impl<
			traits_type::shape_type::dim0, vec_type, vec_type, _UnaryOp
		>::element_apply(vec_like, vec_like, op);
		return target;
	}

	template <
		typename _Vector,
		typename _VectorOut,
		typename _UnaryOp
	>
	_VectorOut& unary_op(
		const _Vector& source,
		_VectorOut&    destination,
		_UnaryOp       op
	) {
		typedef typename _as_vector<const _Vector>::vector_type ivec_type;
		typedef typename _as_vector<_VectorOut>::vector_type    ovec_type;
		typedef vector_traits<ivec_type> itraits_type;
		typedef vector_traits<ovec_type> otraits_type;

		static_assert(
			_same_shape<
			    typename itraits_type::shape_type,
			    typename otraits_type::shape_type
			>::type::value,
			"Difference shape: _Vector, _VectorOut."
		);

		auto src = as_vector(source), dest = as_vector(destination);
		_unary_op_impl<
			itraits_type::shape_type::dim0, ivec_type, ovec_type, _UnaryOp
		>::element_apply(src, dest, op);
		return destination;
	}

	template <
		size_t   _Length,
		typename _Vector,
		typename _VectorOut,
		typename _UnaryOp
	>
	struct _unary_op_impl {

		static void element_apply(
			const _Vector& source,
			_VectorOut&    destination,
			_UnaryOp       op
		) {
			const auto index = _Length - 1;
			destination[index] = op(source[index]);
			_unary_op_impl<
				index, _Vector, _VectorOut, _UnaryOp
			>::element_apply(source, destination, op);
		}
	};

	template <
		typename _Vector,
		typename _VectorOut,
		typename _UnaryOp
	>
	struct _unary_op_impl<0, _Vector, _VectorOut, _UnaryOp> {

		static void element_apply(
			const _Vector&, _VectorOut&, _UnaryOp
		) { }
	};

	template <
		size_t   _Length,
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _binary_op_impl;

	template <
		typename _Vector,
		typename _BinaryOp
	>
	_Vector binary_op(
		const _Vector& left,
		const _Vector& right,
		_BinaryOp      op
	) {
		typedef typename _as_vector<const _Vector>::vector_type ivec_type;
		typedef typename _as_vector<_Vector>::vector_type       ovec_type;
		typedef vector_traits<ivec_type> traits_type;

		_Vector result;

		auto l = as_vector(left), r = as_vector(right), res = as_vector(result);

		_binary_op_impl<
			traits_type::shape_type::dim0, ivec_type, ivec_type, ovec_type, _BinaryOp
		>::element_apply(l, r, res, op);

		return result;
	}

	template <
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	_VectorOut& binary_op(
		const _VectorL& left,
		const _VectorR& right,
		_VectorOut&     result,
		_BinaryOp       op
	) {
		typedef typename _as_vector<const _VectorL>::vector_type lvec_type;
		typedef typename _as_vector<const _VectorR>::vector_type rvec_type;
		typedef typename _as_vector<_VectorOut>::vector_type     ovec_type;
		typedef vector_traits<lvec_type> ltraits_type;
		typedef vector_traits<rvec_type> rtraits_type;
		typedef vector_traits<ovec_type> otraits_type;

		static_assert(
			_same_shape<
				typename ltraits_type::shape_type,
				typename otraits_type::shape_type
			>::type::value &&
			_same_shape<
			    typename rtraits_type::shape_type,
			    typename otraits_type::shape_type
			>::type::value,
			"Difference shape: _VectorL, _VectorR, _VectorOut."
		);

		auto l = as_vector(left), r = as_vector(right), res = as_vector(result);

		_binary_op_impl<
			ltraits_type::shape_type::dim0, lvec_type, rvec_type, ovec_type, _BinaryOp
		>::element_apply(l, r, res, op);
		return result;
	}

	template <
		size_t   _Length,
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _binary_op_impl {

		typedef typename vector_traits<_VectorL>::shape_type   lshape;
		typedef typename vector_traits<_VectorR>::shape_type   rshape;
		typedef typename vector_traits<_VectorOut>::shape_type oshape;

		static_assert (
			_same_shape<lshape, rshape>::type::value &&
			_same_shape<lshape, oshape>::type::value,
			"Different shape: _VectorL, _VectorR, _VectorOut."
		);

		static void element_apply(
			const _VectorL& left,
			const _VectorR& right,
			_VectorOut&     result,
			_BinaryOp       op
		) {
			const size_t index = _Length - 1;
			result[index] = op(left[index], right[index]);
			_binary_op_impl<
				index, _VectorL, _VectorR, _VectorOut, _BinaryOp
			>::element_apply(left, right, result, op);
		}
	};

	template <
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _binary_op_impl<
		0, _VectorL, _VectorR, _VectorOut, _BinaryOp
	> {
		static void element_apply(
			const _VectorL&, const _VectorR&, _VectorOut&, _BinaryOp
		) { }
	};

	template <
		size_t   _Length,
		typename _Val,
		typename _Vector,
		typename _BinaryOp,
		typename _UnaryOp
	>
	struct _accumulate_impl;

	template <
		typename _Val,
		typename _Vector,
		typename _BinaryOp,
		typename _UnaryOp
	>
	constexpr _Val accumulate(
		const _Vector& vector,
		_BinaryOp      binary,
		_UnaryOp       unary ,
		_Val           init = static_cast<_Val>(0)
	) {
		typedef typename _as_vector<const _Vector>::vector_type vec_type;
		typedef vector_traits<vec_type> traits_type;
		auto vec = as_vector(vector);
		return _accumulate_impl<
			traits_type::shape_type::dim0, _Val, vec_type, _BinaryOp, _UnaryOp
		>::element_acc(vec, binary, unary, init);
	}

	template <
		size_t   _Length,
		typename _Val,
		typename _Vector,
		typename _BinaryOp,
		typename _UnaryOp
	>
	struct _accumulate_impl {

		static constexpr _Val element_acc(
			const _Vector& vector,
			_BinaryOp      binary,
			_UnaryOp       unary ,
			_Val           init
		) {
			const size_t index = _Length - 1;
			return binary(
				_accumulate_impl<
					index, _Val, _Vector, _BinaryOp, _UnaryOp
				>::element_acc(vector, binary, unary, init),
				unary(vector[index])
			);
		}
	};

	template <
		typename _Val,
		typename _Vector,
		typename _BinaryOp,
		typename _UnaryOp
	>
	struct _accumulate_impl<
		0, _Val, _Vector, _BinaryOp, _UnaryOp
	> {
		static _Val element_acc(
			const _Vector&, _BinaryOp, _UnaryOp, _Val init
		) { return init; }
	};

	template <
		size_t   _Length,
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl;

	template <
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	constexpr _Val product(
		const _VectorL& left ,
		const _VectorR& right,
		_Accumulator    acc  ,
		_BinaryOp       op   ,
		_Val            init
	) {
		typedef typename _as_vector<const _VectorL>::vector_type lvec_type;
		typedef typename _as_vector<const _VectorR>::vector_type rvec_type;

		typedef typename vector_traits<lvec_type>::shape_type lshape;
		typedef typename vector_traits<rvec_type>::shape_type rshape;

		static_assert (_same_shape<lshape, rshape>::type::value, "Difference shape: _VectorL, _VectorR.");

		auto l = as_vector(left), r = as_vector(right);

		return _product_impl<
			lshape::dim0, _Val, lvec_type, rvec_type, _Accumulator, _BinaryOp
		>::element_prod(l, r, acc, op, init);
	}

	template <
		size_t   _Length,
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl {

		static constexpr _Val element_prod(
			const _VectorL& left ,
			const _VectorR& right,
			_Accumulator    acc  ,
			_BinaryOp       op   ,
			_Val            init
		) {
			const size_t index = _Length - 1;
			return acc(
				_product_impl<
					index, _Val, _VectorL, _VectorR, _Accumulator, _BinaryOp
				>::element_prod(left, right, acc, op, init),
				op(left[index], right[index])
			);
		}
	};

	template <
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl<
		0, _Val, _VectorL, _VectorR, _Accumulator, _BinaryOp
	> {
		static constexpr _Val element_prod(
			const _VectorL& left ,
			const _VectorL& right,
			_Accumulator    acc  ,
			_BinaryOp       op   ,
			_Val            init
		) { return init; }
	};
}

#endif //_VECTOR_ALGO_H_

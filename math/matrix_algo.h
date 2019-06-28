/*
 * Created by Maou Lim on 2019/6/25.
 */

#ifndef _MATRIX_ALGO_H_
#define _MATRIX_ALGO_H_

#include "../common/defines.h"

namespace math {

	template <
	    tools::size_t _Rows,
		tools::size_t _Cols,
		typename      _Matrix,
		typename      _MatrixOut,
		typename      _UnaryOp
	>
	struct _mat_unary_op_impl;

	template <
		typename _Matrix,
		typename _UnaryOp
	>
	_Matrix& matrix_unary_op(_Matrix& target, _UnaryOp op) {

		typedef typename _as_matrix<_Matrix>::matrix_type mat_type;
		typedef matrix_traits<mat_type> traits_type;

		auto mat_like = as_matrix(target);
		_mat_unary_op_impl<
			traits_type::shape_type::dim0,
			traits_type::shape_type::dim1,
			mat_type, mat_type,
			_UnaryOp
		>::element_apply(mat_like, mat_like, op);

		return target;
	}

	template <
		typename _Matrix,
		typename _MatrixOut,
		typename _UnaryOp
	>
	_MatrixOut& matrix_unary_op(
		const _Matrix& source, _MatrixOut& destination, _UnaryOp op
	) {
		typedef typename _as_matrix<const _Matrix>::matrix_type imat_type;
		typedef typename _as_matrix<_MatrixOut>::matrix_type    omat_type;

		typedef matrix_traits<imat_type> itraits_type;
		typedef matrix_traits<omat_type> otraits_type;

		static_assert(
			_same_shape<
			    typename itraits_type::shape_type,
			    typename otraits_type::shape_type
			>::type::value,
			"Different shape: _Matrix, _MatrixOut."
		);

		auto src = as_matrix(source), dst = as_matrix(destination);

		_mat_unary_op_impl<
			itraits_type::shape_type::dim0,
			itraits_type::shape_type::dim1,
			imat_type, omat_type,
			_UnaryOp
		>::element_apply(src, dst, op);

		return destination;
	}

	template <
		tools::size_t _Rows,
	    tools::size_t _Cols,
		typename      _Matrix,
		typename      _MatrixOut,
		typename      _UnaryOp
	>
	struct _mat_unary_op_impl {

		static void element_apply(
			const _Matrix& source, _MatrixOut& dest, _UnaryOp op
		) {
			_mat_unary_op_impl<
				_Rows, _Cols - 1, _Matrix, _MatrixOut, _UnaryOp
			>::element_apply(source, dest, op);
			dest[_Rows - 1][_Cols - 1] = op(source[_Rows - 1][_Cols - 1]);
		}
	};

	template <
	    tools::size_t _Cols,
		typename _Matrix,
		typename _MatrixOut,
		typename _UnaryOp
	>
	struct _mat_unary_op_impl<0, _Cols, _Matrix, _MatrixOut, _UnaryOp> {

		static void element_apply(
			const _Matrix& source, _MatrixOut& dest, _UnaryOp op
		) { }
	};

	template <
		tools::size_t _Rows,
		typename      _Matrix,
		typename      _MatrixOut,
		typename      _UnaryOp
	>
	struct _mat_unary_op_impl<_Rows, 0, _Matrix, _MatrixOut, _UnaryOp> {

		static void element_apply(
			const _Matrix& source, _MatrixOut& dest, _UnaryOp op
		) {
			typedef matrix_traits<_Matrix> traits_type;
			_mat_unary_op_impl<
				_Rows - 1, traits_type::shape_type::dim1, _Matrix, _MatrixOut, _UnaryOp
			>::element_apply(source, dest, op);
		}
	};

	template <
		tools::size_t _Rows,
		tools::size_t _Cols,
	    typename      _MatrixL,
	    typename      _MatrixR,
	    typename      _MatrixOut,
	    typename      _BinaryOp
	>
	struct _mat_binary_op_impl;

	template <
		typename _Matrix,
		typename _BinaryOp
	>
	_Matrix matrix_binary_op(
		const _Matrix& left, const _Matrix& right, _BinaryOp op
	) {
		typedef typename _as_matrix<const _Matrix>::matrix_type imat_type;
		typedef typename _as_matrix<_Matrix>::matrix_type       omat_type;
		typedef matrix_traits<imat_type> traits_type;

		_Matrix result;

		auto l = as_matrix(left), r = as_matrix(right), res = as_matrix(result);

		_mat_binary_op_impl<
			traits_type::shape_type::dim0,
			traits_type::shape_type::dim1,
			imat_type, imat_type, omat_type,
			_BinaryOp
		>::element_apply(l, r, res, op);
		return result;
	}

	template <
		typename      _MatrixL,
		typename      _MatrixR,
		typename      _MatrixOut,
		typename      _BinaryOp
	>
	_MatrixOut& matrix_binary_op(
		const _MatrixL& left, const _MatrixR& right, _MatrixOut& result, _BinaryOp op
	) {
		typedef typename _as_matrix<const _MatrixL>::matrix_type lmat_type;
		typedef typename _as_matrix<const _MatrixR>::matrix_type rmat_type;
		typedef typename _as_matrix<_MatrixOut>::matrix_type     omat_type;

		typedef matrix_traits<lmat_type> ltraits_type;
		typedef matrix_traits<rmat_type> rtraits_type;
		typedef matrix_traits<omat_type> otraits_type;

		static_assert(
			_same_shape<
				typename ltraits_type::shape_type,
				typename otraits_type::shape_type
			>::type::value &&
			_same_shape<
				typename rtraits_type::shape_type,
				typename otraits_type::shape_type
			>::type::value,
			"Difference shape: _MatrixL, _MatrixR, _MatrixOut."
		);

		auto l = as_matrix(left), r = as_matrix(right), res = as_matrix(result);

		_mat_binary_op_impl<
			lmat_type::shape_type::dim0,
			lmat_type::shape_type::dim1,
			lmat_type, rmat_type, omat_type,
			_BinaryOp
		>::element_apply(l, r, res, op);
		return result;
	}

	template <
		tools::size_t _Rows,
		tools::size_t _Cols,
		typename      _MatrixL,
		typename      _MatrixR,
		typename      _MatrixOut,
		typename      _BinaryOp
	>
	struct _mat_binary_op_impl {

		static void element_apply(
			const _MatrixL& left,
			const _MatrixR& right,
			_MatrixOut&     result,
			_BinaryOp       op
		) {
			_mat_binary_op_impl<
				_Rows, _Cols - 1, _MatrixL, _MatrixR, _MatrixOut, _BinaryOp
			>::element_apply(left, right, result, op);
			result[_Rows - 1][_Cols - 1] =
				op(left[_Rows - 1][_Cols - 1], right[_Rows - 1][_Cols - 1]);
		}
	};

	template <
		tools::size_t _Rows,
		typename      _MatrixL,
		typename      _MatrixR,
		typename      _MatrixOut,
		typename      _BinaryOp
	>
	struct _mat_binary_op_impl<
		_Rows, 0, _MatrixL, _MatrixR, _MatrixOut, _BinaryOp
	> {
		static void element_apply(
			const _MatrixL& left,
			const _MatrixR& right,
			_MatrixOut&     result,
			_BinaryOp       op
		) {
			typedef matrix_traits<_MatrixL> traits_type;
			_mat_binary_op_impl<
			    _Rows - 1,
			    traits_type::shape_type::dim1,
			    _MatrixL, _MatrixR, _MatrixOut,
			    _BinaryOp
			>::element_apply(left, right, result, op);
		}
	};

	template <
	    tools::size_t _Cols,
		typename      _MatrixL,
		typename      _MatrixR,
		typename      _MatrixOut,
		typename      _BinaryOp
	>
	struct _mat_binary_op_impl<
	    0, _Cols, _MatrixL, _MatrixR, _MatrixOut, _BinaryOp
	> {
		static void element_apply(
			const _MatrixL& left,
			const _MatrixR& right,
			_MatrixOut&     result,
			_BinaryOp       op
		) { }
	};
}

#endif //_MATRIX_ALGO_H_

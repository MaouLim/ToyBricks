/*
 * Created by Maou Lim on 2019/6/24.
 */

#ifndef _MATH_MATRIX_H_
#define _MATH_MATRIX_H_

#include <cassert>

#include "vector.h"
#include "matrix_algo.h"

namespace math {

	template <
		typename _Tp, tools::size_t _Rows, tools::size_t _Cols
	>
	class matrix;

	template <typename _Tp, typename _Shape>
	struct matrix_of {
		typedef matrix<_Tp, 1, 1> type;
		typedef matrix<_Tp, 1, 1> transpose;
	};

	template <
	    typename _Tp, typename _Dim, _Dim _N
	>
	struct matrix_of<_Tp, _vector_shape<_Dim, _N>> {
		typedef matrix<_Tp, _N, 1> type;
		typedef matrix<_Tp, 1, _N> transpose;
	};

	template <
		typename _Tp, typename _Dim, _Dim _M, _Dim _N
	>
	struct matrix_of<_Tp, _matrix_shape<_Dim, _M, _N>> {
		typedef matrix<_Tp, _M, _N> type;
		typedef matrix<_Tp, _N, _M> transpose;
	};

	template <
	    typename _Tp, tools::size_t _Rows, tools::size_t _Cols
	>
	class matrix {
	protected:
		typedef matrix<_Tp, _Rows, _Cols> self_type;

		template <typename _U>
		using same_shape = matrix<_U, _Rows, _Cols>;

	public:
		const static tools::size_t rows = _Rows;
		const static tools::size_t cols = _Cols;

		typedef self_type                                  matrix_type;
		typedef matrix<_Tp, _Cols, _Rows>                  transpose;
		typedef vector<_Tp, _Rows>                         column_type;
		typedef vector<_Tp, _Cols>                         row_type;
		typedef _matrix_shape<tools::size_t, _Rows, _Cols> shape_type;

		typedef _Tp                       value_type;
		typedef _Tp*                      pointer;
		typedef const _Tp*                const_pointer;
		typedef _Tp&                      reference;
		typedef const _Tp&                const_reference;

		typedef tools::size_t             size_type;

		row_type row_vecs[_Rows];

		matrix() = default;

		matrix(const std::initializer_list<column_type>& vecs) {
			size_type count = 0;
			auto itr = vecs.begin();
			while (count < _Cols && itr != vecs.end()) {
				for (size_type i = 0u; i < _Rows; ++i) {
					row_vecs[i][count] = itr->operator[](i);
				}
				++count; ++itr;
			}
		}

		template <typename _U>
		matrix(const std::initializer_list<_U>& vals) {
			size_type row = 0, col = 0;
			auto itr = vals.begin();
			while (row < _Rows && itr != vals.end()) {
				row_vecs[row][col++] = *(itr++);
				if (_Cols == col) {
					col = 0; ++row;
				}
			}
		}

		row_type& operator[](size_type row) { return row_vecs[row]; }
		const row_type& operator[](size_type row) const { return row_vecs[row]; }

		reference operator()(size_type row, size_type col) {
			assert(row < rows && col < cols);
			return row_vecs[row][col];
		}

		const_reference operator()(size_type row, size_type col) const {
			assert(row < rows && col < cols);
			return row_vecs[row][col];
		}

		template <typename _U>
		self_type& operator=(const same_shape<_U>& other) {
			return math::matrix_unary_op(
				other, *this, tools::identity<value_type>()
			);
		}

		template <typename _U>
		self_type& operator+=(_U scalar) {
			return math::matrix_unary_op(
				*this, [scalar](_Tp t) { return t + static_cast<_Tp>(scalar); }
			);
		}

		template <typename _U>
		self_type& operator+=(const same_shape<_U>& other) {
			return math::matrix_binary_op(
				*this, other, *this, [](_Tp t, _U u) { return t + static_cast<_Tp>(u); }
			);
		}

		template <typename _U>
		self_type& operator-=(_U scalar) {
			return math::matrix_unary_op(
				*this, [scalar](_Tp t) { return t - static_cast<_Tp>(scalar); }
			);
		}

		template <typename _U>
		self_type& operator-=(const same_shape<_U>& other) {
			return math::matrix_binary_op(
				*this, other, *this, [](_Tp t, _U u) { return t - static_cast<_Tp>(u); }
			);
		}

		template <typename _U>
		self_type& operator*=(_U scalar) {
			return math::matrix_unary_op(
				*this, [scalar](_Tp t) { return t * static_cast<_Tp>(scalar); }
			);
		}

		template <typename _U>
		self_type& operator*=(const same_shape<_U>& other) {
			return math::matrix_binary_op(
				*this, other, *this, [](_Tp t, _U u) { return t * static_cast<_Tp>(u); }
			);
		}

		template <typename _U>
		self_type& operator/=(_U scalar) {
			return math::matrix_unary_op(
				*this, [scalar](_Tp t) { return t / static_cast<_Tp>(scalar); }
			);
		}

		template <typename _U>
		self_type& operator/=(const same_shape<_U>& other) {
			return math::matrix_binary_op(
				*this, other, *this, [](_Tp t, _U u) { return t / static_cast<_Tp>(u); }
			);
		}

		template <typename _U>
		self_type& operator%=(_U scalar) {
			return math::matrix_unary_op(
				*this, [scalar](_Tp t) { return t % static_cast<_Tp>(scalar); }
			);
		}

		template <typename _U>
		self_type& operator%=(const same_shape<_U>& other) {
			return math::matrix_binary_op(
				*this, other, *this, [](_Tp t, _U u) { return t % static_cast<_Tp>(u); }
			);
		}

		size_type size() const { return _Rows * _Cols; }

		// todo test the data()
		pointer data() { return row_vecs->data(); }
		const_pointer data() const { return row_vecs->data(); }

		row_type& row(size_type r) {
			assert(r < rows);
			return row_vecs[r];
		}

		const row_type& row(size_type r) const {
			assert(r < rows);
			return row_vecs[r];
		}

		column_type column(size_type c) const {
			assert(c < cols);
			column_type vec;
			return math::unary_op(row_vecs, vec, [c](const row_type& row) { return row[c]; });
		}
	};

	// multiply
	template <
	    typename      _Tp,
	    tools::size_t _L,
	    tools::size_t _M,
	    tools::size_t _N
	>
	matrix<_Tp, _L, _N> multiply(
		const matrix<_Tp, _L, _M>& left,
		const matrix<_Tp, _M, _N>& right
	) {
		
	}

	template <typename _Tp, tools::size_t _N>
	typename matrix_of<_Tp, typename math::vector<_Tp, _N>::shape_type>::type
		as_column(const math::vector<_Tp, _N>& vec)
	{
		typedef typename matrix_of<
			_Tp, typename math::vector<_Tp, _N>::shape_type
		>::type mat_t;

		return mat_t({ vec });
	}

	template <typename _Tp, tools::size_t _N>
	typename matrix_of<_Tp, typename math::vector<_Tp, _N>::shape_type>::transpose
		as_row(const math::vector<_Tp, _N>& vec)
	{
		typedef typename matrix_of<
		    _Tp, typename math::vector<_Tp, _N>::shape_type
		>::transpose mat_t;

		mat_t res;
		math::unary_op(vec, res[0], tools::identity<_Tp>());
		return res;
	}

	//todo convert to column(row) vector

	/* degenerative */
	template <typename _Tp, tools::size_t _Rows>
	struct matrix<_Tp, _Rows, 0> { /**/ matrix() = delete; };

	template <typename _Tp, tools::size_t _Cols>
	struct matrix<_Tp, 0, _Cols> { /**/ matrix() = delete; };

	template <typename _Tp>
	using matrix2x2 = matrix<_Tp, 2, 2>;

	template <typename _Tp>
	using matrix2x3 = matrix<_Tp, 2, 3>;

	template <typename _Tp>
	using matrix3x2 = matrix<_Tp, 3, 2>;

	template <typename _Tp>
	using matrix3x2 = matrix<_Tp, 3, 2>;

	template <typename _Tp>
	using matrix3x3 = matrix<_Tp, 3, 3>;

	template <typename _Tp>
	using matrix3x4 = matrix<_Tp, 3, 4>;

	template <typename _Tp>
	using matrix4x3 = matrix<_Tp, 4, 3>;

	template <typename _Tp>
	using matrix4x4 = matrix<_Tp, 4, 4>;
}

#endif //TOYBRICKS_MATRIX_H

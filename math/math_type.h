#ifndef _MATH_TYPE_H_
#define _MATH_TYPE_H_

#include "../common/type_base.h"

namespace math {

	template <
	    typename _DimType, _DimType _Dim0, _DimType _Dim1
	>
	struct _matrix_shape;

	template <
		typename _DimType, _DimType _Dim0
	>
	struct _vector_shape;

	template <typename _DimType>
	struct _scalar_shape {

		typedef _scalar_shape<_DimType> type;
		typedef _DimType                dim_type;

		typedef _scalar_shape<_DimType>       scalar_shape;
		typedef _vector_shape<_DimType, 1>    vector_shape;
		typedef _matrix_shape<_DimType, 1, 1> matrix_shape;

		static const tools::size_t dimension = 0;
	};

	template <
	    typename _DimType, _DimType _Dim0
	>
	struct _vector_shape {

		typedef _vector_shape<_DimType, _Dim0>    type;
		typedef _DimType                          dim_type;

		typedef _scalar_shape<_DimType>           scalar_shape;
		typedef _vector_shape<_DimType, _Dim0>    vector_shape;
		typedef _matrix_shape<_DimType, _Dim0, 1> matrix_shape;

		static const dim_type dim0 = _Dim0;
		static const tools::size_t dimension = 1;
	};

	template <
		typename _DimType, _DimType _Dim0, _DimType _Dim1
	>
	struct _matrix_shape {
		typedef _matrix_shape<_DimType, _Dim0, _Dim1> shape_type;
		typedef shape_type                            type;
		typedef _DimType                              dim_type;

		typedef _scalar_shape<_DimType>               scalar_shape;
		typedef _vector_shape<_DimType, _Dim0>        vector_shape;
		typedef _matrix_shape<_DimType, _Dim0, _Dim1> matrix_shape;

		typedef _vector_shape<_DimType, _Dim0>        column_shape;
		typedef _vector_shape<_DimType, _Dim1>        row_shape;

		static const dim_type dim0 = _Dim0;
		static const dim_type dim1 = _Dim1;
		static const tools::size_t dimension = 2;
	};

	template <typename _Tp>
	struct _shape_of {
		typedef _scalar_shape<tools::size_t> type;
	};

	template <typename _Tp>
	struct _shape_of<_Tp*> {
		typedef _vector_shape<tools::size_t, 1> type;
	};

	template <typename _Tp>
	struct _shape_of<_Tp**> {
		typedef _matrix_shape<tools::size_t, 1, 1> type;
	};

	template <typename _Tp, tools::size_t _N>
	struct _shape_of<_Tp[_N]> {
		typedef _vector_shape<tools::size_t, _N> type;
	};

	template <
	    typename _Tp, tools::size_t _M, tools::size_t _N
	>
	struct _shape_of<_Tp[_M][_N]> {
		typedef _matrix_shape<tools::size_t, _M, _N> type;
	};

	template <
	    typename      _Tp,
	    tools::size_t _N,
	    template <typename, tools::size_t> class _VecLike
	>
	struct _shape_of<_VecLike<_Tp, _N>> {
		typedef _vector_shape<tools::size_t, _N> type;
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <
			typename, tools::size_t, tools::size_t
		> class _MatLike
	>
	struct _shape_of<_MatLike<_Tp, _M, _N>> {
		typedef _matrix_shape<tools::size_t, _M, _N> type;
	};

	template <typename _ShapeL, typename _ShapeR>
	struct _same_shape :
		tools::_same_type<
		    typename _ShapeL::matrix_shape,
		    typename _ShapeR::matrix_shape
		> { };

	/**
	 * @note This class can make a constant pointer pose as a vector.
	 *       Users have to ensure the memory the pointer pointing
	 *       to is defined. Besides, users are unable to use the
	 *       wrapper to modify the memory.
	 */
	template <typename _Tp, tools::size_t _N>
	struct _const_vec_like {

		typedef _Tp                       value_type;
		typedef const _Tp&                reference;
		typedef const _Tp*                pointer;

		typedef tools::size_t             size_type;

		explicit _const_vec_like(pointer ptr) : base(ptr) { }

		reference operator[](size_type idx) const {
			assert(idx < dim0);
			return base[idx];
		}

		static const tools::size_t dim0 = _N;

		pointer base;
	};

	/**
	 * @note This class can make a pointer pose as a vector.
	 *       Users have to ensure the memory the pointer pointing
	 *       to is defined.
	 */
	template <typename _Tp, tools::size_t _N>
	struct _vec_like {

		typedef _Tp                 value_type;
		typedef _Tp&                reference;
		typedef const _Tp&          const_reference;
		typedef _Tp*                pointer;
		typedef const _Tp*          const_pointer;

		typedef tools::size_t       size_type;

		explicit _vec_like(pointer ptr) : base(ptr) { }

		reference operator[](size_type idx) {
			assert(idx < dim0);
			return base[idx];
		}

		const_reference operator[](size_type idx) const {
			assert(idx < dim0);
			return base[idx];
		}

		static const tools::size_t dim0 = _N;

		pointer base;
	};

	template <
		typename _Tp, tools::size_t _M, tools::size_t _N
	>
	struct _const_mat_like {

		typedef _Tp                 value_type;
		typedef const _Tp&          reference;
		typedef const _Tp*          pointer;

		typedef tools::size_t       size_type;

		typedef _const_mat_like<_Tp, _N, _M>          transpose;
		typedef pointer                               column_type;
		typedef pointer                               row_type;
		typedef _matrix_shape<tools::size_t, _M, _N>  shape_type;

		explicit _const_mat_like(pointer ptr) : base(ptr) { }

		row_type operator[](size_type idx) const {
			assert(idx < dim0);
			return base + idx * _N;
		}

		static const tools::size_t dim0 = _M;
		static const tools::size_t dim1 = _N;

		pointer base;
	};

	template <
		typename _Tp, tools::size_t _M, tools::size_t _N
	>
	struct _mat_like {

		typedef _Tp                 value_type;
		typedef _Tp&                reference;
		typedef const _Tp&          const_reference;
		typedef _Tp*                pointer;
		typedef const _Tp*          const_pointer;

		typedef tools::size_t       size_type;

		typedef _mat_like<_Tp, _N, _M>               transpose;
		typedef pointer                              column_type;
		typedef pointer                              row_type;
		typedef _matrix_shape<tools::size_t, _M, _N> shape_type;

		explicit _mat_like(pointer ptr) : base(ptr) { }

		row_type operator[](size_type idx) const {
			assert(idx < dim0);
			return base + idx * _N;
		}

		static const tools::size_t dim0 = _M;
		static const tools::size_t dim1 = _N;

		pointer base;
	};

	template <typename _Tp>
	struct _as_vector {

		typedef _Tp               truth_type;
		typedef _vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t); }
	};

	template <typename _Tp>
	struct _as_vector<const _Tp> {

		typedef const _Tp               truth_type;
		typedef _const_vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t); }
	};

	template <typename _Tp>
	struct _as_vector<_Tp*> {

		typedef _Tp*              truth_type;
		typedef _vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(t); }
	};

	template <typename _Tp>
	struct _as_vector<const _Tp*> {

		typedef const _Tp*              truth_type;
		typedef _const_vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(t); }
	};

	template <typename _Tp>
	struct _as_vector<_Tp* const> {

		typedef _Tp* const              truth_type;
		typedef _vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(const_cast<_Tp*>(t)); }
	};

	template <typename _Tp>
	struct _as_vector<const _Tp* const> {

		typedef const _Tp* const        truth_type;
		typedef _const_vec_like<_Tp, 1> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(const_cast<const _Tp*>(t)); }
	};

	template <
		typename      _Tp,
		tools::size_t _N
	>
	struct _as_vector<_Tp[_N]> {

		typedef _Tp                truth_type[_N];
		typedef _vec_like<_Tp, _N> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(t); }
	};

	template <
		typename      _Tp,
		tools::size_t _N
	>
	struct _as_vector<const _Tp[_N]> {

		typedef const _Tp                truth_type[_N];
		typedef _const_vec_like<_Tp, _N> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(t); }
	};

	template <
	    typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_vector<_VecLike<_Tp, _N>> {

		typedef _VecLike<_Tp, _N>  truth_type;
		typedef _vec_like<_Tp, _N> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t[0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_vector<const _VecLike<_Tp, _N>> {

		typedef const _VecLike<_Tp, _N>  truth_type;
		typedef _const_vec_like<_Tp, _N> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t[0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t, tools::size_t> class _MatLike
	>
	struct _as_vector<_MatLike<_Tp, _M, _N>> {

		typedef _MatLike<_Tp, _M, _N>                        truth_type;
		typedef _vec_like<typename truth_type::row_type, _M> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t[0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t, tools::size_t> class _MatLike
	>
	struct _as_vector<const _MatLike<_Tp, _M, _N>> {

		typedef const _MatLike<_Tp, _M, _N>                        truth_type;
		typedef _const_vec_like<typename truth_type::row_type, _M> vector_type;

		vector_type operator()(truth_type& t) const { return vector_type(&t[0]); }
	};

	template <typename _Tp>
	auto as_vector(_Tp& t) { return _as_vector<_Tp>()(t); }

	template <typename _Tp>
	struct _as_matrix {
		typedef _Tp                  truth_type;
		typedef _mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t); }
	};

	template <typename _Tp>
	struct _as_matrix<const _Tp> {
		typedef const _Tp                  truth_type;
		typedef _const_mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t); }
	};

	template <typename _Tp>
	struct _as_matrix<_Tp*> {
		typedef _Tp*                 truth_type;
		typedef _mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(t); }
	};

	template <typename _Tp>
	struct _as_matrix<const _Tp*> {
		typedef const _Tp*                 truth_type;
		typedef _const_mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(t); }
	};

	template <typename _Tp>
	struct _as_matrix<_Tp* const> {
		typedef _Tp* const           truth_type;
		typedef _mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(const_cast<_Tp*>(t)); }
	};

	template <typename _Tp>
	struct _as_matrix<const _Tp* const> {
		typedef const _Tp* const           truth_type;
		typedef _const_mat_like<_Tp, 1, 1> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(const_cast<const _Tp*>(t)); }
	};

	template <typename _Tp, tools::size_t _N>
	struct _as_matrix<_Tp[_N]> {
		typedef _Tp                    truth_type[_N];
		typedef _mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(t); }
	};

	template <typename _Tp, tools::size_t _N>
	struct _as_matrix<const _Tp[_N]> {
		typedef const _Tp                    truth_type[_N];
		typedef _const_mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(t); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<_VecLike<_Tp, _N>> {

		typedef _VecLike<_Tp, _N>      truth_type;
		typedef _mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t[0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<const _VecLike<_Tp, _N>> {

		typedef const _VecLike<_Tp, _N>      truth_type;
		typedef _const_mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t[0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<_VecLike<_Tp, _N>*> {

		typedef _VecLike<_Tp, _N>*     truth_type;
		typedef _mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&((*t)[0])); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<const _VecLike<_Tp, _N>*> {

		typedef const _VecLike<_Tp, _N>*     truth_type;
		typedef _const_mat_like<_Tp, _N, 1>  matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&((*t)[0])); }
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<_VecLike<_Tp, _N>* const> {

		typedef _VecLike<_Tp, _N>* const truth_type;
		typedef _mat_like<_Tp, _N, 1>    matrix_type;

		matrix_type operator()(truth_type& t) const {
			return matrix_type(&((*const_cast<_VecLike<_Tp, _N>*>(t))[0]));
		}
	};

	template <
		typename      _Tp,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<const _VecLike<_Tp, _N>* const> {

		typedef const _VecLike<_Tp, _N>* const truth_type;
		typedef _const_mat_like<_Tp, _N, 1>    matrix_type;

		matrix_type operator()(truth_type& t) const {
			return matrix_type(&((*const_cast<const _VecLike<_Tp, _N>*>(t))[0]));
		}
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<_VecLike<_Tp, _M>[_N]> {

		typedef _VecLike<_Tp, _M>      truth_type[_N];
		typedef _mat_like<_Tp, _M, _N> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&(t[0][0])); }
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t> class _VecLike
	>
	struct _as_matrix<const _VecLike<_Tp, _M>[_N]> {

		typedef const _VecLike<_Tp, _M>      truth_type[_N];
		typedef _const_mat_like<_Tp, _M, _N> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&(t[0][0])); }
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t, tools::size_t> class _MatLike
	>
	struct _as_matrix<_MatLike<_Tp, _M, _N>> {

		typedef _MatLike<_Tp, _M, _N>  truth_type;
		typedef _mat_like<_Tp, _M, _N> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t[0][0]); }
	};

	template <
		typename      _Tp,
		tools::size_t _M,
		tools::size_t _N,
		template <typename, tools::size_t, tools::size_t> class _MatLike
	>
	struct _as_matrix<const _MatLike<_Tp, _M, _N>> {

		typedef const _MatLike<_Tp, _M, _N>  truth_type;
		typedef _const_mat_like<_Tp, _M, _N> matrix_type;

		matrix_type operator()(truth_type& t) const { return matrix_type(&t[0][0]); }
	};

	template <typename _Tp>
	auto as_matrix(_Tp& t) { return _as_matrix<_Tp>()(t); }

	template <typename _Tp>
	struct vector_traits {

		static_assert(tools::_is_scalar<_Tp>::type::value, "Type not supported.");

		typedef _Tp*                         vector_type;
		typedef _Tp                          value_type;
		typedef _scalar_shape<tools::size_t> shape_type;
	};

	template <typename _Tp>
	struct vector_traits<_Tp*> {

		static_assert(tools::_is_scalar<_Tp>::type::value, "Type not supported.");

		typedef _Tp*                            vector_type;
		typedef _Tp                             value_type;
		typedef _vector_shape<tools::size_t, 1> shape_type;
	};

	template <typename _Tp, tools::size_t _N>
	struct vector_traits<_Tp[_N]> {

		typedef _Tp*                             vector_type;
		typedef _Tp                              value_type;
		typedef _vector_shape<tools::size_t, _N> shape_type;
	};

	template <
	    typename _Tp, tools::size_t _N,
	    template <typename, tools::size_t> class _VecLike
	>
	struct vector_traits<_VecLike<_Tp, _N>> {

		typedef _VecLike<_Tp, _N>                vector_type;
		typedef _Tp                              value_type;
		typedef _vector_shape<tools::size_t, _N> shape_type;
	};

	template <typename _Tp>
	struct matrix_traits {

		static_assert(tools::_is_scalar<_Tp>::type::value, "Type not supported.");

		typedef _Tp** matrix_type;
		typedef _Tp** transpose;
		typedef _Tp*  column_type;
		typedef _Tp*  row_type;

		typedef _Tp   value_type;

		typedef _matrix_shape<tools::size_t, 1, 1> shape_type;
	};

	template <
	    typename _Tp, tools::size_t _Rows, tools::size_t _Cols,
		template <
			typename, tools::size_t, tools::size_t
		> class _MatLike
	>
	struct matrix_traits<_MatLike<_Tp, _Rows, _Cols>> {

		typedef _MatLike<_Tp, _Rows, _Cols>       matrix_type;
		typedef typename matrix_type::transpose   transpose;
		typedef typename matrix_type::column_type column_type;
		typedef typename matrix_type::row_type    row_type;
		typedef typename matrix_type::value_type  value_type;
		typedef typename matrix_type::shape_type  shape_type;
	};
}

#endif // _MATH_TYPE_H_

/*
 * Created by Maou Lim on 2019/6/24.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cassert>

#include "../common/defines.h"
#include "../common/type_base.h"

#include "vector_algo.h"

namespace math {

	template <
	    typename _Tp, tools::size_t _N
	>
	struct vector;

	template <typename _Tp, typename _Shape>
	struct vector_of {
		typedef vector<_Tp, 1> type;
	};

	template <
	    typename _Tp, typename _Dim, _Dim _N
	>
	struct vector_of<_Tp, _vector_shape<_Dim, _N>> {
		typedef vector<_Tp, _N> type;
	};

	template <typename _Tp, tools::size_t _N>
	struct vector {
	protected:

		typedef vector<_Tp, _N> self_type;

		template <typename _U>
		using same_shape = vector<_U, _N>;

	public:
		typedef self_type                        vector_type;
		typedef _vector_shape<tools::size_t, _N> shape_type;

		typedef _Tp           value_type;
		typedef _Tp&          reference;
		typedef const _Tp&    const_reference;
		typedef _Tp*          pointer;
		typedef const _Tp*    const_pointer;

		typedef tools::size_t    size_type;
		typedef tools::float64_t length_type;

		value_type elements[_N];

		template <typename _U = _Tp, tools::size_t _M = _N>
		self_type& operator=(const vector<_U, _M>& other) {
			_assign_aux(other, tools::_bool_type<_M < _N>());
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator+=(_U scalar) {
			math::unary_op(*this, [scalar](_Tp t) { return t + static_cast<_Tp>(scalar); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator+=(const same_shape<_U>& other) {
			math::binary_op(*this, other, *this, [](_Tp t, _U u) { return t + static_cast<_Tp>(u); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(_U scalar) {
			math::unary_op(*this, [scalar](_Tp t) { return t - static_cast<_Tp>(scalar); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(const same_shape<_U>& other) {
			math::binary_op(*this, other, *this, [](_Tp t, _U u) { return t - static_cast<_Tp>(u); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(_U scalar) {
			math::unary_op(*this, [scalar](_Tp t) { return t * static_cast<_Tp>(scalar); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(const same_shape<_U>& other) {
			math::binary_op(*this, other, *this, [](_Tp t, _U u) { return t * static_cast<_Tp>(u); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(_U scalar) {
			math::unary_op(*this, [scalar](_Tp t) { return t / static_cast<_Tp>(scalar); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(const same_shape<_U>& other) {
			math::binary_op(*this, other, *this, [](_Tp t, _U u) { return t / static_cast<_Tp>(u); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(_U scalar) {
			math::unary_op(*this, [scalar](_Tp t) { return t % static_cast<_Tp>(scalar); });
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(const same_shape<_U>& other) {
			math::binary_op(*this, other, *this, [](_Tp t, _U u) { return t % static_cast<_Tp>(u); });
			return *this;
		}

		reference operator[](size_type idx) {
			assert(idx < shape_type::dim0);
			return elements[idx];
		}

		const_reference operator[](size_type idx) const {
			assert(idx < shape_type::dim0);
			return elements[idx];
		}

		size_type size() const { return shape_type::dim0; }
		length_type length() const { return static_cast<length_type>(sqrt(sum_of_squares(*this))); }

		reference at(size_type idx) {
			assert(idx < shape_type::dim0);
			return elements[idx];
		}

		const_reference at(size_type idx) const {
			assert(idx < shape_type::dim0);
			return elements[idx];
		}

		const_pointer data() const { return elements; }
		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }

	private:
		template <typename _U, tools::size_t _M>
		void _assign_aux(const vector<_U, _M>& other, tools::_true_type) {
			// _M < _N --> expand
			_vec_like<value_type, _M> same_shape(data());
			math::unary_op(other, same_shape, tools::identity<_U>());
			_vec_like<value_type, _N - _M> rest(data() + _M);
			math::unary_op(rest, [](value_type) { return static_cast<value_type>(0); });
		}

		template <typename _U, tools::size_t _M>
		void _assign_aux(const vector<_U, _M>& other, tools::_false_type) {
			// cut off
			_const_vec_like<_U, _N> wrapper(other.data());
			math::unary_op(wrapper, *this, tools::identity<_U>());
		}
	};

}

#endif //_VECTOR_H_

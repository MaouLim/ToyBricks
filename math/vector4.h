/*
 * Created by Maou Lim on 2018/12/22.
 */

#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <cassert>

#include "../common/defines.h"
#include "math_type.h"
#include "vector.h"

namespace math {

	template <typename _Tp>
	struct vector<_Tp, 4> {
	public:
		typedef _Tp                             value_type;
		typedef vector<_Tp, 4>                  vector_type;
		typedef _vector_shape<tools::size_t, 4> shape_type;

		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;

		typedef tools::size_t    size_type;
		typedef tools::float64_t length_type;

	protected:
		typedef vector_type self_type;

		template <typename _U>
		using same_shape = vector<_U, 4>;

	public:
		union {
			struct { _Tp x, y, z, w; };
			struct { _Tp r, b, g, a; };
			struct { _Tp s, t, p, q; };
		};

		vector() :
			x(static_cast<value_type>(0)),
			y(static_cast<value_type>(0)),
			z(static_cast<value_type>(0)),
			w(static_cast<value_type>(0)) { }

		template <typename _U>
		explicit vector(_U scalar) :
			x(static_cast<value_type>(scalar)),
			y(static_cast<value_type>(scalar)),
			z(static_cast<value_type>(scalar)),
			w(static_cast<value_type>(scalar)) { }

		template <typename _XTp, typename _YTp, typename _ZTp, typename _WTp>
		explicit vector(_XTp x, _YTp y, _ZTp z, _WTp w) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)),
			z(static_cast<value_type>(z)),
			w(static_cast<value_type>(w)) { }

		const_reference operator[](size_t index) const {
			assert(index < shape_type::dim0);
			return (&x)[index];
		}

		reference operator[](size_t index) {
			return const_cast<reference>(
				((const self_type*) this)->operator[](index)
			);
		}

		template <typename _U = _Tp, tools::size_t _M>
		self_type& operator=(const vector<_U, _M>& other) {
			_assign_aux(other, tools::_bool_type<_M < shape_type::dim0>());
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator+=(_U scalar) {
			this->x += static_cast<value_type>(scalar);
			this->y += static_cast<value_type>(scalar);
			this->z += static_cast<value_type>(scalar);
			this->w += static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator+=(const same_shape<_U>& other) {
			this->x += static_cast<value_type>(other.x);
			this->y += static_cast<value_type>(other.y);
			this->z += static_cast<value_type>(other.z);
			this->w += static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(_U scalar) {
			this->x -= static_cast<value_type>(scalar);
			this->y -= static_cast<value_type>(scalar);
			this->z -= static_cast<value_type>(scalar);
			this->w -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(const same_shape<_U>& other) {
			this->x -= static_cast<value_type>(other.x);
			this->y -= static_cast<value_type>(other.y);
			this->z -= static_cast<value_type>(other.z);
			this->w -= static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(_U scalar) {
			this->x *= static_cast<value_type>(scalar);
			this->y *= static_cast<value_type>(scalar);
			this->z *= static_cast<value_type>(scalar);
			this->w *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(const same_shape<_U>& other) {
			this->x *= static_cast<value_type>(other.x);
			this->y *= static_cast<value_type>(other.y);
			this->z *= static_cast<value_type>(other.z);
			this->w *= static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(_U scalar) {
			this->x /= static_cast<value_type>(scalar);
			this->y /= static_cast<value_type>(scalar);
			this->z /= static_cast<value_type>(scalar);
			this->w /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(const same_shape<_U>& other) {
			this->x /= static_cast<value_type>(other.x);
			this->y /= static_cast<value_type>(other.y);
			this->z /= static_cast<value_type>(other.z);
			this->w /= static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(_U scalar) {
			this->x %= static_cast<value_type>(scalar);
			this->y %= static_cast<value_type>(scalar);
			this->z %= static_cast<value_type>(scalar);
			this->w %= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(const same_shape<_U>& other) {
			this->x %= static_cast<value_type>(other.x);
			this->y %= static_cast<value_type>(other.y);
			this->z %= static_cast<value_type>(other.z);
			this->w %= static_cast<value_type>(other.w);
			return *this;
		}

		const_pointer data() const { return &x; }
		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }

		length_type length() const { return static_cast<length_type>(sqrt(sum_of_squares(*this))); }

	private:
		template <typename _U, tools::size_t _M>
		void _assign_aux(const vector<_U, _M>& other, tools::_true_type) {
			// _M < _N --> expand
			_vec_like<value_type, _M> same_shape(data());
			math::unary_op(other, same_shape, tools::identity<_U>());
			_vec_like<value_type, shape_type::dim0 - _M> rest(data() + _M);
			math::unary_op(rest, [](value_type) { return static_cast<value_type>(0); });
		}

		template <typename _U, tools::size_t _M>
		void _assign_aux(const vector<_U, _M>& other, tools::_false_type) {
			// cut off
			_const_vec_like<_U, shape_type::dim0> wrapper(other.data());
			math::unary_op(wrapper, *this, tools::identity<_U>());
		}
	};

	template <typename _Tp>
	using vector4 = vector<_Tp, 4>;

	using vector4u = vector4<tools::uint32_t>;
	using vector4i = vector4<tools::sint32_t>;
	using vector4f = vector4<tools::float32_t>;
	using vector4d = vector4<tools::float64_t>;
}

#endif //_VECTOR4_H_

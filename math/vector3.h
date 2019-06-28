#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cassert>

#include "../common/defines.h"
#include "vector.h"
#include "math_type.h"

namespace math {

	template <typename _Tp>
	struct vector<_Tp, 3> {
	public:
		typedef _Tp                             value_type;
		typedef vector<_Tp, 3>                  vector_type;
		typedef _vector_shape<tools::size_t, 3> shape_type;

		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;

		typedef tools::size_t    size_type;
		typedef tools::float64_t length_type;

	protected:
		typedef vector_type self_type;

		template <typename _U>
		using same_shape = vector<_U, 3>;

	public:
		union {
			struct { _Tp x, y, z; };
			struct { _Tp r, b, g; };
			struct { _Tp s, t, p; };
		};

		vector() :
			x(static_cast<value_type>(0)),
			y(static_cast<value_type>(0)),
			z(static_cast<value_type>(0)) { }

		template <typename _U>
		explicit vector(_U scalar) :
			x(static_cast<value_type>(scalar)),
			y(static_cast<value_type>(scalar)),
			z(static_cast<value_type>(scalar)) { }

		template <typename _XTp, typename _YTp, typename _ZTp>
		explicit vector(_XTp x, _YTp y, _ZTp z) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)),
			z(static_cast<value_type>(z)) { }

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
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator+=(const same_shape<_U>& other) {
			this->x += static_cast<value_type>(other.x);
			this->y += static_cast<value_type>(other.y);
			this->z += static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(_U scalar) {
			this->x -= static_cast<value_type>(scalar);
			this->y -= static_cast<value_type>(scalar);
			this->z -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator-=(const same_shape<_U>& other) {
			this->x -= static_cast<value_type>(other.x);
			this->y -= static_cast<value_type>(other.y);
			this->z -= static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(_U scalar) {
			this->x *= static_cast<value_type>(scalar);
			this->y *= static_cast<value_type>(scalar);
			this->z *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator*=(const same_shape<_U>& other) {
			this->x *= static_cast<value_type>(other.x);
			this->y *= static_cast<value_type>(other.y);
			this->z *= static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(_U scalar) {
			this->x /= static_cast<value_type>(scalar);
			this->y /= static_cast<value_type>(scalar);
			this->z /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator/=(const same_shape<_U>& other) {
			this->x /= static_cast<value_type>(other.x);
			this->y /= static_cast<value_type>(other.y);
			this->z /= static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(_U scalar) {
			this->x %= static_cast<value_type>(scalar);
			this->y %= static_cast<value_type>(scalar);
			this->z %= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _U = _Tp>
		self_type& operator%=(const same_shape<_U>& other) {
			this->x %= static_cast<value_type>(other.x);
			this->y %= static_cast<value_type>(other.y);
			this->z %= static_cast<value_type>(other.z);
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

#ifdef _VECTOR_USE_CONST_
		static const vector_type zero, x, y, z, xy, yz, zx, xyz;
#endif
	};

	template <typename _Tp>
	using vector3 = vector<_Tp, 3>;

	using vector3u = vector3<tools::uint32_t>;
	using vector3i = vector3<tools::sint32_t>;
	using vector3f = vector3<tools::float32_t>;
	using vector3d = vector3<tools::float64_t>;

#ifdef _VECTOR_USE_CONST_

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::zero(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::x(static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::y(static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::z(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(1));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::xy(static_cast<_Tp>(1), static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::yz(static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(1));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::zx(static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(1));

	template <typename _Tp>
	const vector3<_Tp> vector3<_Tp>::xyz(static_cast<_Tp>(1), static_cast<_Tp>(1), static_cast<_Tp>(1));
#endif
}

#endif
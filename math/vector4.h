/*
 * Created by Maou Lim on 2018/12/22.
 */

#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <cassert>

#include "../common/defines.h"
#include "math_base.h"

namespace tools {

	template <typename _Tp, typename _Precise>
	struct vector4 {
	public:
		typedef _Tp                     value_type;
		typedef vector4<_Tp, _Precise>  vec_type;
		typedef vec_type                type;
		typedef _Precise                precise_type;

		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;

		typedef size_t   size_type;
		typedef _Tp      length_type;

	protected:
		typedef vec_type self_type;

	public:
		union {
			struct { _Tp x, y, z, w; };
			struct { _Tp r, b, g, a; };
			struct { _Tp s, t, p, q; };
		};

		static const size_type dimension = 4;

		vector4(value_type scalar = static_cast<value_type>(0)) :
			x(scalar), y(scalar), z(scalar), w(scalar) { }

		vector4(value_type x, value_type y, value_type z, value_type w) : x(x), y(y), z(z), w(w) { }

		template <typename _OtherTp>
		explicit vector4(_OtherTp scalar) :
			x(static_cast<value_type>(scalar)),
			y(static_cast<value_type>(scalar)),
			z(static_cast<value_type>(scalar)),
			w(static_cast<value_type>(scalar)) { }

		template <typename _XTp, typename _YTp, typename _ZTp, typename _WTp>
		explicit vector4(_XTp x, _YTp y, _ZTp z, _WTp w) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)),
			z(static_cast<value_type>(z)),
			w(static_cast<value_type>(w)) { }

		const_reference operator[](size_t index) const {
			assert(index < dimension);
			return (&x)[index];
		}

		reference operator[](size_t index) {
			return const_cast<reference>(
				((const self_type*) this)->operator[](index)
			);
		}

		template <typename _OtherTp = _Tp>
		self_type& operator=(const vector4<_OtherTp, _Precise>& other) {
			this->x = static_cast<value_type>(other.x);
			this->y = static_cast<value_type>(other.y);
			this->z = static_cast<value_type>(other.z);
			this->w = static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(_OtherTp scalar) {
			this->x += static_cast<value_type>(scalar);
			this->y += static_cast<value_type>(scalar);
			this->z += static_cast<value_type>(scalar);
			this->w += static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(const vector4<_OtherTp, _Precise>& other) {
			this->x += static_cast<value_type>(other.x);
			this->y += static_cast<value_type>(other.y);
			this->z += static_cast<value_type>(other.z);
			this->w += static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(_OtherTp scalar) {
			this->x -= static_cast<value_type>(scalar);
			this->y -= static_cast<value_type>(scalar);
			this->z -= static_cast<value_type>(scalar);
			this->w -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(const vector4<_OtherTp, _Precise>& other) {
			this->x -= static_cast<value_type>(other.x);
			this->y -= static_cast<value_type>(other.y);
			this->z -= static_cast<value_type>(other.z);
			this->w -= static_cast<value_type>(other.w);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator*=(_OtherTp scalar) {
			this->x *= static_cast<value_type>(scalar);
			this->y *= static_cast<value_type>(scalar);
			this->z *= static_cast<value_type>(scalar);
			this->w *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator/=(_OtherTp scalar) {
			this->x /= static_cast<value_type>(scalar);
			this->y /= static_cast<value_type>(scalar);
			this->z /= static_cast<value_type>(scalar);
			this->w /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator%=(_OtherTp scalar) {
			this->x %= static_cast<value_type>(scalar);
			this->y %= static_cast<value_type>(scalar);
			this->z %= static_cast<value_type>(scalar);
			this->w %= static_cast<value_type>(scalar);
			return *this;
		}

		const_pointer data() const { return &x; }
		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }

		length_type length() const { return static_cast<length_type>(sqrt(sum_of_square(*this))); }
	};

#ifdef _VECTOR_USE_CONST_
	template <typename _Tp, typename _Precise>
	const vector4<_Tp, _Precise> zero(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector4<_Tp, _Precise> x(static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector4<_Tp, _Precise> y(static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector4<_Tp, _Precise> z(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector4<_Tp, _Precise> w(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(1));
#endif

	using vector4u       = vector4<uint32_t, precise::midp>;
	using vector4i       = vector4<sint32_t, precise::midp>;

	using vector4f       = vector4<float32_t, precise::midp>;
	using vector4f_highp = vector4<float64_t, precise::highp>;

	using vector4d       = vector4<float64_t, precise::midp>;
}

#endif //_VECTOR4_H_

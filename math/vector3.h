#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cassert>

#include "../common/defines.h"
#include "math_base.h"

namespace tools {

	template <typename _Tp, typename _Precise>
	struct vector3 {
	public:
		typedef _Tp                     value_type;
		typedef vector3<_Tp, _Precise>  vec_type;
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
			struct { _Tp x, y, z; };
			struct { _Tp r, b, g; };
			struct { _Tp s, t, q; };
		};

		static const size_type dimension = 3;

		vector3(value_type scalar) : x(scalar), y(scalar), z(scalar) { }
		vector3(value_type x, value_type y, value_type z) : x(x), y(y), z(z) { }

		template <typename _OtherTp>
		explicit vector3(_OtherTp scalar) :
			x(static_cast<value_type>(scalar)),
			y(static_cast<value_type>(scalar)),
		    z(static_cast<value_type>(scalar)) { }

		template <typename _XTp, typename _YTp, typename _ZTp>
		explicit vector3(_XTp x, _YTp y, _ZTp z) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)), 
			z(static_cast<value_type>(z)) { }

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
		self_type& operator=(const vector3<_OtherTp, _Precise>& other) {
			this->x = static_cast<value_type>(other.x);
			this->y = static_cast<value_type>(other.y);
			this->z = static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(_OtherTp scalar) {
			this->x += static_cast<value_type>(scalar);
			this->y += static_cast<value_type>(scalar);
			this->z += static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(const vector3<_OtherTp, _Precise>& other) {
			this->x += static_cast<value_type>(other.x);
			this->y += static_cast<value_type>(other.y);
			this->z += static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(_OtherTp scalar) {
			this->x -= static_cast<value_type>(scalar);
			this->y -= static_cast<value_type>(scalar);
			this->z -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(const vector3<_OtherTp, _Precise>& other) {
			this->x -= static_cast<value_type>(other.x);
			this->y -= static_cast<value_type>(other.y);
			this->z -= static_cast<value_type>(other.z);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator*=(_OtherTp scalar) {
			this->x *= static_cast<value_type>(scalar);
			this->y *= static_cast<value_type>(scalar);
			this->z *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator/=(_OtherTp scalar) {
			this->x /= static_cast<value_type>(scalar);
			this->y /= static_cast<value_type>(scalar);
			this->z /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator%=(_OtherTp scalar) {
			this->x %= static_cast<value_type>(scalar);
			this->y %= static_cast<value_type>(scalar);
			this->z %= static_cast<value_type>(scalar);
			return *this;
		}

		const_pointer data() const { return &x; }
		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }

		length_type length() const { return static_cast<length_type>(sqrt(sum_of_square(*this))); }
	};

#ifdef _VECTOR_USE_CONST_
	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> zero(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> x(static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> y(static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> z(static_cast<_Tp>(0), static_cast<_Tp>(0), static_cast<_Tp>(1));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> xy(static_cast<_Tp>(1), static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> yz(static_cast<_Tp>(0), static_cast<_Tp>(1), static_cast<_Tp>(1));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> zx(static_cast<_Tp>(1), static_cast<_Tp>(0), static_cast<_Tp>(1));

	template <typename _Tp, typename _Precise>
	const vector3<_Tp, _Precise> xyz(static_cast<_Tp>(1), static_cast<_Tp>(1), static_cast<_Tp>(1));
#endif

	using vector3u       = vector3<uint32_t, precise::midp>;
	using vector3i       = vector3<sint32_t, precise::midp>;

	using vector3f       = vector3<float32_t, precise::midp>;
	using vector3f_highp = vector3<float64_t, precise::highp>;

	using vector3d       = vector3<float64_t, precise::midp>;
}

#endif
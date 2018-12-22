#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cassert>

#include "../common/defines.h"
#include "math_base.h"
#include "vector_function.h"

namespace tools {

	template <typename _Tp, typename _Precise>
	struct vector2 {
	public:
		typedef _Tp                     value_type;
		typedef vector2<_Tp, _Precise>  vec_type;
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
			struct { _Tp x, y; };
			struct { _Tp r, b; };
			struct { _Tp s, t; };
		};

		static const size_type dimension = 2;

		vector2(value_type scalar = static_cast<value_type>(0)) : x(scalar), y(scalar) { }
		vector2(value_type x, value_type y) : x(x), y(y) { }

		template <typename _OtherTp>
		explicit vector2(_OtherTp scalar) : 
			x(static_cast<value_type>(scalar)), 
			y(static_cast<value_type>(scalar)) { }

		template <typename _XTp, typename _YTp>
		explicit vector2(_XTp x, _YTp y) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)) { }

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
		self_type& operator=(const vector2<_OtherTp, _Precise>& other) {
			this->x = static_cast<value_type>(other.x);
			this->y = static_cast<value_type>(other.y);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(_OtherTp scalar) {
			this->x += static_cast<value_type>(scalar);
			this->y += static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator+=(const vector2<_OtherTp, _Precise>& other) {
			this->x += static_cast<value_type>(other.x);
			this->y += static_cast<value_type>(other.y);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(_OtherTp scalar) {
			this->x -= static_cast<value_type>(scalar);
			this->y -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator-=(const vector2<_OtherTp, _Precise>& other) {
			this->x -= static_cast<value_type>(other.x);
			this->y -= static_cast<value_type>(other.y);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator*=(_OtherTp scalar) {
			this->x *= static_cast<value_type>(scalar);
			this->y *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator/=(_OtherTp scalar) {
			this->x /= static_cast<value_type>(scalar);
			this->y /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename _OtherTp = _Tp>
		self_type& operator%=(_OtherTp scalar) {
			this->x %= static_cast<value_type>(scalar);
			this->y %= static_cast<value_type>(scalar);
			return *this;
		}

		const_pointer data() const { return &x; }
		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }

		length_type length() const { return static_cast<length_type>(sqrt(sum_of_square(*this))); }
	};

#ifdef _VECTOR_USE_CONST_
	template <typename _Tp, typename _Precise>
	const vector2<_Tp, _Precise> zero(static_cast<_Tp>(0), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector2<_Tp, _Precise> x(static_cast<_Tp>(1), static_cast<_Tp>(0));

	template <typename _Tp, typename _Precise>
	const vector2<_Tp, _Precise> y(static_cast<_Tp>(0), static_cast<_Tp>(1));

	template <typename _Tp, typename _Precise>
	const vector2<_Tp, _Precise> xy(static_cast<_Tp>(1), static_cast<_Tp>(1));
#endif

	using vector2u       = vector2<uint32_t, precise::midp>;
	using vector2i       = vector2<sint32_t, precise::midp>;

	using vector2f       = vector2<float32_t, precise::midp>;
	using vector2f_highp = vector2<float64_t, precise::highp>;

	using vector2d       = vector2<float64_t, precise::midp>;
 }

#endif

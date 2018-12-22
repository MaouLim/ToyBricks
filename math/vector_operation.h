#ifndef _ITERATOR_H_ /* conflict with iterator.h */

#ifndef _VECTOR_OPERATION_H_
#define _VECTOR_OPERATION_H_

#include "../common/functor.h"
#include "vector_traits.h"
#include "vector_function.h"

namespace tools {

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr bool operator==(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return product(left, right, logic_and<bool>(), equal_to<_Tp>(), true);
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr bool operator!=(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) { return !(left == right); }

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	vec<_Tp, _Precise> operator+(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return apply(left, right, plus<_Tp>());
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	vec<_Tp, _Precise> operator-(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return apply(left, right, minus<_Tp>());
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec,
		typename _Scalar
	>
	vec<_Tp, _Precise> operator*(const vec<_Tp, _Precise>& v, _Scalar scalar) {
		vec<_Tp, _Precise> result;
		transform(v, result, [scalar](size_t, _Tp val) { return val * scalar; });
		return result;
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec,
		typename _Scalar
	>
	vec<_Tp, _Precise> operator*(_Scalar scalar, const vec<_Tp, _Precise>& v) {
		return operator*(v, scalar);
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	vec<_Tp, _Precise> operator*(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return apply(left, right, multiply<_Tp>());
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec,
		typename _Scalar
	>
	vec<_Tp, _Precise> operator/(const vec<_Tp, _Precise>& v, _Scalar scalar) {
		vec<_Tp, _Precise> result;
		transform(v, result, [scalar](size_t, _Tp val) { return val / scalar; });
		return result;
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	vec<_Tp, _Precise> operator/(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return apply(left, right, divide<_Tp>());
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	vec<_Tp, _Precise> operator%(
		const vec<_Tp, _Precise>& left,
		const vec<_Tp, _Precise>& right
	) {
		return apply(left, right, mod<_Tp>());
	}
}

#endif
#endif

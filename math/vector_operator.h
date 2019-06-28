#ifndef _ITERATOR_H_ /* conflict with iterator.h */

#ifndef _VECTOR_OPERATOR_H_
#define _VECTOR_OPERATOR_H_

#include "../common/defines.h"
#include "../common/functor.h"

#include "math_common.h"
#include "vector.h"

namespace math {

	template <
		typename      _Tp,
		tools::size_t _N
	>
	constexpr bool operator==(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return product(left, right, tools::logic_and<bool>(), tools::equal_to<_Tp>(), true);
	}

	template <
		typename      _Tp,
		tools::size_t _N
	>
	constexpr bool operator!=(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) { return !(left == right); }

	template <
		typename      _Tp,
		tools::size_t _N
	>
	vector<_Tp, _N> operator+(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return binary_op(left, right, tools::plus<_Tp>());
	}

	template <
		typename      _Tp,
		tools::size_t _N
	>
	vector<_Tp, _N> operator-(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return binary_op(left, right, tools::minus<_Tp>());
	}

	template <
		typename      _Tp,
		tools::size_t _N,
		typename      _Scalar
	>
	vector<_Tp, _N> operator*(const vector<_Tp, _N>& v, _Scalar scalar) {
		vector<_Tp, _N> result;
		unary_op(v, result, [scalar](_Tp t) { return t * scalar; });
		return result;
	}

	template <
		typename      _Tp,
		tools::size_t _N,
		typename      _Scalar
	>
	vector<_Tp, _N> operator*(_Scalar scalar, const vector<_Tp, _N>& v) {
		return operator*(v, scalar);
	}

	template <
		typename      _Tp,
		tools::size_t _N
	>
	vector<_Tp, _N> operator*(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return binary_op(left, right, tools::multiply<_Tp>());
	}

	template <
		typename      _Tp,
		tools::size_t _N,
		typename _Scalar
	>
	vector<_Tp, _N> operator/(const vector<_Tp, _N>& v, _Scalar scalar) {
		vector<_Tp, _N> result;
		unary_op(v, result, [scalar](_Tp t) { return t / scalar; });
		return result;
	}

	template <
		typename      _Tp,
		tools::size_t _N
	>
	vector<_Tp, _N> operator/(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return binary_op(left, right, tools::divide<_Tp>());
	}

	template <
		typename      _Tp,
		tools::size_t _N
	>
	vector<_Tp, _N> operator%(
		const vector<_Tp, _N>& left,
		const vector<_Tp, _N>& right
	) {
		return binary_op(left, right, tools::mod<_Tp>());
	}

	// basic operations on vector

	template <
		typename _Tp, tools::size_t _N
	>
	void clamp(math::vector<_Tp, _N>& target, _Tp lower, _Tp upper) {
		assert(lower <= upper);

		auto clamper = [lower, upper](_Tp val) {
			if (val < lower) { return lower; }
			else if (upper < val) { return upper; }
			else { return val; }
		};

		unary_op(target, clamper);
	}

	template <
		typename _Tp, tools::size_t _N
	>
	constexpr _Tp sum(const math::vector<_Tp, _N>& vec) {
		return accumulate(vec, tools::plus<_Tp>(), tools::identity<_Tp>(), _Tp(0));
	}

	template <
		typename _Tp, tools::size_t _N
	>
	constexpr _Tp mean(const math::vector<_Tp, _N>& vec) {
		typedef vector_traits<math::vector<_Tp, _N>> traits_type;
		return sum(vec) / traits_type::shape_type::dim0;
	}

	template <
		typename _Tp, tools::size_t _N
	>
	constexpr _Tp variance(const math::vector<_Tp, _N>& vec) {
		typedef vector_traits<math::vector<_Tp, _N>> traits_type;
		const _Tp m = mean(vec);
		return accumulate(
			vec,
			tools::plus<_Tp>(),
			[m](_Tp val) { return (val - m) * (val - m); }
		) / traits_type::dimension;
	}

	template <
		typename _Tp, tools::size_t _N, typename _Comparator
	>
	constexpr _Tp max_element(
		const math::vector<_Tp, _N>& vec,
		_Comparator                  comp
	) {
		return accumulate(
			vec, tools::max_filter<_Tp, _Comparator>(), tools::identity<_Tp>(), std::numeric_limits<_Tp>::min()
		);
	}

	template <
		typename _Tp, tools::size_t _N, typename _Comparator
	>
	constexpr _Tp min_element(
		const math::vector<_Tp, _N>& vec,
		_Comparator                  comp
	) {
		return accumulate(
			vec, tools::min_filter<_Tp, _Comparator>(), tools::identity<_Tp>(), std::numeric_limits<_Tp>::max()
		);
	}

	template <
		typename _Tp, tools::size_t _N
	>
	constexpr _Tp sum_of_squares(const math::vector<_Tp, _N>& vec) {
		return accumulate(vec, tools::plus<_Tp>(), tools::square<_Tp>(), _Tp(0));
	}

	template <
		typename _Tp, tools::size_t _N
	>
	void normalize(math::vector<_Tp, _N>& vec) {
		const _Tp length = sqrt(sum_of_squares(vec));
		unary_op(vec, [length](_Tp val) -> _Tp { return val / length; });
	}

	template <
		typename _Tp, tools::size_t _N
	>
	constexpr _Tp dot(
		const math::vector<_Tp, _N>& left,
		const math::vector<_Tp, _N>& right
	) {
		return product(
			left, right, tools::plus<_Tp>(), tools::multiply<_Tp>(), _Tp(0)
		);
	}
}

#endif
#endif

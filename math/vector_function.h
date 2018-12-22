#ifndef _VECTOR_FUNCTION_H_
#define _VECTOR_FUNCTION_H_

#include <cassert>
#include <cmath>
#include <limits>

#include "../common/defines.h"
#include "../common/functor.h"
#include "vector_traits.h"

namespace tools {

	/* basic algorithms */

	template <
		size_t   _Length,
		typename _Vector,
		typename _Consumer
	>
	struct _transform_impl;

	template <
		typename _Vector,
		typename _Consumer
	>
	void transform(_Vector& target, _Consumer consumer) {
		typedef vector_trait<_Vector> trait_type;
		_transform_impl<
			trait_type::dimension, _Vector, _Consumer
		>::element_transform_modified(target, consumer);
	}

	template <
		typename _Vector,
		typename _Consumer
	>
	void transform(
		const _Vector& source,
		_Vector&       destinate,
		_Consumer      consumer
	) {
		typedef vector_trait<_Vector> trait_type;
		_transform_impl<
			trait_type::dimension, _Vector, _Consumer
		>::element_transform_unmodified(source, destinate, consumer);
	}

	template <
		size_t   _Length,
		typename _Vector,
		typename _Consumer
	>
	struct _transform_impl {

		static void element_transform_modified(
			_Vector& target, _Consumer consumer
		) {
			const size_t index = _Length - 1;
			target[index] = consumer(index, target[index]);
			_transform_impl<
				index, _Vector, _Consumer
			>::element_transform_modified(target, consumer);
		}

		static void element_transform_unmodified(
			const _Vector& source,
			_Vector&       destinate,
			_Consumer      consumer
		) {
			const size_t index = _Length - 1;
			destinate[index] = consumer(index, source[index]);
			_transform_impl<
				index, _Vector, _Consumer
			>::element_transform_unmodified(source, destinate, consumer);
		}
	};

	template <
		typename _Vector,
		typename _Consumer
	>
		struct _transform_impl<0, _Vector, _Consumer> {

		static void element_transform_modified(
			_Vector&, _Consumer
		) { }

		static void element_transform_unmodified(
			const _Vector&, _Vector&, _Consumer
		) { }
	};

	template <
		size_t   _Length,
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _apply_impl;

	template <
		typename _Vector,
		typename _BinaryOp
	>
	_Vector apply(
		const _Vector& left,
		const _Vector& right,
		_BinaryOp      op
	) {
		typedef vector_trait<_Vector> trait_type;
		_Vector result(static_cast<typename trait_type::value_type>(0));

		_apply_impl<
			trait_type::dimension, _Vector, _Vector, _Vector, _BinaryOp
		>::element_apply(left, right, result, op);

		return result;
	}

	template <
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	void apply(
		const _VectorL& left,
		const _VectorR& right,
		_VectorOut&     result,
		_BinaryOp       op
	) {
		typedef vector_trait<_VectorL> trait_type; // todo!
		_apply_impl<
			trait_type::dimension, _VectorL, _VectorR, _VectorOut, _BinaryOp
		>::element_apply(left, right, result, op);
	}

	template <
		size_t   _Length,
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _apply_impl {

		typedef vector_trait<_VectorL>   ltrait_type;
		typedef vector_trait<_VectorR>   rtrait_type;
		typedef vector_trait<_VectorOut> otrait_type;

		static_assert (
			ltrait_type::dimension == rtrait_type::dimension &&
			rtrait_type::dimension == otrait_type::dimension
		);

		static void element_apply(
			const _VectorL& left,
			const _VectorR& right,
			_VectorOut&     result,
			_BinaryOp       op
		) {
			const size_t index = _Length - 1;
			result[index] = op(left[index], right[index]);
			_apply_impl<
				index, _VectorL, _VectorR, _VectorOut, _BinaryOp
			>::element_apply(left, right, result, op);
		}
	};

	template <
		typename _VectorL,
		typename _VectorR,
		typename _VectorOut,
		typename _BinaryOp
	>
	struct _apply_impl<
		0, _VectorL, _VectorR, _VectorOut, _BinaryOp
	> {
		static void element_apply(
			const _VectorL&, const _VectorR&, _VectorOut&, _BinaryOp
		) { }
	};

	template <
		size_t   _Length,
		typename _Val,
		typename _Vector,
		typename _Accumulator,
		typename _Consumer
	>
	struct _accumulate_impl;

	template <
		typename _Val,
		typename _Vector,
		typename _Accumulator,
		typename _Consumer
	>
	constexpr _Val accumulate(
		const _Vector& vector,
		_Accumulator   op,
		_Consumer      consumer, 
		_Val           init
	) {
		typedef vector_trait<_Vector> trait_type;
		return _accumulate_impl<
			trait_type::dimension, _Val, _Vector, _Accumulator, _Consumer
		>::element_acc(vector, op, consumer, init);
	}

	template <
		size_t   _Length,
		typename _Val,
		typename _Vector,
		typename _Accumulator,
		typename _Consumer
	>
	struct _accumulate_impl {

		static constexpr _Val element_acc(
			const _Vector& vector,
			_Accumulator   op,
			_Consumer      consumer,
			_Val           init
		) {
			const size_t index = _Length - 1;
			return op(
				_accumulate_impl<
					index, _Val, _Vector, _Accumulator, _Consumer
				>::element_acc(vector, op, consumer, init), 
				consumer(index, vector[index])
			);
		}
	};

	template <
		typename _Val,
		typename _Vector,
		typename _Accumulator,
		typename _Consumer
	>
	struct _accumulate_impl<
		0, _Val, _Vector, _Accumulator, _Consumer
	> {
		static _Val element_acc(
			const _Vector&, _Accumulator, _Consumer, _Val init
		) { return init; }
	};

	template <
		size_t   _Length,
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl;

	template <
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	constexpr _Val product(
		const _VectorL& left ,
		const _VectorR& right,
		_Accumulator    acc  ,
		_BinaryOp       op   ,
		_Val            init
	) {
		typedef vector_trait<_VectorL> ltrait_type;
		typedef vector_trait<_VectorR> rtrait_type;

		static_assert (ltrait_type::dimension == rtrait_type::dimension);

		return _product_impl<
			ltrait_type::dimension, _Val, _VectorL, _VectorR, _Accumulator, _BinaryOp
		>::element_prod(left, right, acc, op, init);
	}

	template <
		size_t   _Length,
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl {

		static constexpr _Val element_prod(
			const _VectorL& left ,
			const _VectorR& right,
			_Accumulator    acc  ,
			_BinaryOp       op   ,
			_Val            init
		) {
			const size_t index = _Length - 1;
			return acc(
				_product_impl<
					index, _Val, _VectorL, _VectorR, _Accumulator, _BinaryOp
				>::element_prod(left, right, acc, op, init),
				op(left[index], right[index])
			);
		}
	};

	template <
		typename _Val,
		typename _VectorL,
		typename _VectorR,
		typename _Accumulator,
		typename _BinaryOp
	>
	struct _product_impl<
		0, _Val, _VectorL, _VectorR, _Accumulator, _BinaryOp
	> {
		static constexpr _Val element_prod(
			const _VectorL& left ,
			const _VectorL& right,
			_Accumulator    acc  ,
			_BinaryOp       op   ,
			_Val            init
		) { return init; }
	};

	// apply basic algorithm to specific vector functions

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	void clamp(vec<_Tp, _Precise>& target, _Tp lower, _Tp upper) {
		assert(lower <= upper);

		auto clamper = [lower, upper](size_t, _Tp val) {
			if (val < lower) { return lower; }
			else if (upper < val) { return upper; }
			else { return val; }
		};

		transform(target, clamper);
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr _Tp sum(const vec<_Tp, _Precise>& vector) {
		return accumulate(vector, plus<_Tp>(), identity<_Tp>(), static_cast<_Tp>(0));
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr _Tp mean(const vec<_Tp, _Precise>& vector) {
		typedef vector_trait<vec<_Tp, _Precise>> trait_type;
		return sum(vector) / trait_type::dimension;
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr _Tp variance(const vec<_Tp, _Precise>& vector) {
		typedef vector_trait<vec<_Tp, _Precise>> trait_type;
		const _Tp m = mean(vector);
		return accumulate(
			vector, 
			plus<_Tp>(),
			[m](size_t, _Tp val) { return (val - m) * (val - m); }, 
			static_cast<_Tp>(0)
		) / trait_type::dimension;
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec, 
		typename _Comparator
	>
	constexpr _Tp max_element(
		const vec<_Tp, _Precise>& vector, 
		_Comparator               comparator
	) {
		return accumulate(
			vector, max_filter<_Tp, _Comparator>(), identity<_Tp>(), std::numeric_limits<_Tp>::min()
		);
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec,
		typename _Comparator
	>
	constexpr _Tp min_element(
		const vec<_Tp, _Precise>& vector, 
		_Comparator               comparator
	) {
		return accumulate(
			vector, min_filter<_Tp, _Comparator>(), identity<_Tp>(), std::numeric_limits<_Tp>::max()
		);
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr _Tp sum_of_square(const vec<_Tp, _Precise>& vector) {
		return accumulate(vector, plus<_Tp>(), [](size_t, const _Tp& val) { return val * val; }, static_cast<_Tp>(0));
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	void normalize(vec<_Tp, _Precise>& vector) {
		const _Tp length = std::sqrt(sum_of_square(vector));
		transform(vector, [length](size_t, _Tp val) -> _Tp { return val / length; });
	}

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	constexpr _Tp dot(
		const vec<_Tp, _Precise>& left, 
		const vec<_Tp, _Precise>& right
	) {
		return product(
			left, right, plus<_Tp>(), multiply<_Tp>(), static_cast<_Tp>(0)
		);
	}
}

#endif
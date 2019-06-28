/*
 * Created by Maou Lim on 2018/12/21.
 */

#ifndef _FUNCTOR_H_
#define _FUNCTOR_H_

namespace tools {

	/* relative operators */

	template <typename _TpL, typename  _TpR = _TpL>
	struct equal_to {
		bool operator()(const _TpL& left, const _TpR& right) const {
			return left == right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct less {
		bool operator()(const _TpL& left, const _TpR& right) const {
			return left < right;

		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct greater {
		bool operator()(const _TpL& left, const _TpR& right) const {
			return left > right;
		}
	};

	/* arithmetic operators */

	template <typename _TpL, typename _TpR = _TpL>
	struct plus {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left + right) {
			return left + right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct minus {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left - right) {
			return left - right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct multiply {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left * right) {
			return left * right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct divide {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left / right) {
			return left / right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct mod {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left % right)  {
			return left % right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct bit_and {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left & right)  {
			return left & right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct bit_or {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left | right)  {
			return left | right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct bit_xor {
		auto operator()(const _TpL& left,
		                const _TpR& right)
		const -> decltype(left ^ right)  {
			return left ^ right;
		}
	};

	template <typename _Tp, typename _Comparator = less<_Tp>>
	struct max_filter {
		constexpr _Tp operator()(const _Tp& a, const _Tp& b) {
			return _Comparator()(a, b) ? b : a;
		}
	};

	template <typename _Tp, typename _Comparator = less<_Tp>>
	struct min_filter {
		constexpr _Tp operator()(const _Tp& a, const _Tp& b) {
			return _Comparator()(a, b) ? a : b;
		}
	};

	template <typename _Tp>
	struct square {
		constexpr _Tp operator()(const _Tp& val) const { return val * val; }
	};

	template <typename _Tp>
	struct increase {
		constexpr _Tp operator()(const _Tp& val) const {
			return static_cast<_Tp>(val + 1);
		}
	};

	template <typename _Tp>
	struct decrease {
		constexpr _Tp operator()(const _Tp& val) const {
			return static_cast<_Tp>(val - 1);
		}
	};

	/* logic operators */

	template <typename _Boolean = bool>
	struct logic_and {

		_Boolean operator()(const _Boolean& left,
		                    const _Boolean& right) const {
			return static_cast<_Boolean>(left && right);
		}
	};

	template <typename _Boolean = bool>
	struct logic_or {

		_Boolean operator()(const _Boolean& left,
		                    const _Boolean& right) const {
			return static_cast<_Boolean>(left || right);
		}
	};

	/* mappings */

	template <typename _Tp>
	struct identity {
		const _Tp& operator()(const _Tp& val) const { return val; }
	};

	template <typename _Tp>
	struct zero {
		_Tp operator()(_Tp) const { return static_cast<_Tp>(0); }
		_Tp operator()() const { return static_cast<_Tp>(0); }
	};
}

#endif //_FUNCTOR_H_

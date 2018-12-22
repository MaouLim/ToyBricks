/*
 * Created by Maou Lim on 2018/1/19.
 */

#ifndef _TYPE_BASE_H_
#define _TYPE_BASE_H_

#include <type_traits>

namespace tools {

	/* _true or _false */

	template <bool _Constant>
	struct _bool_type {
		typedef bool value_type;
		static const bool value = _Constant;
	};

	typedef _bool_type<true>  _true_type;
	typedef _bool_type<false> _false_type;

	template <typename _BoolCover>
	struct _convert;

	template <>
	struct _convert<std::true_type> {
		typedef _true_type type;
	};

	template <>
	struct _convert<std::false_type> {
		typedef _false_type type;
	};

	/* _identity */

	template <typename _Tp>
	struct _identity {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp*> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp*> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp* const> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp* const> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp&&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp&&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _is_identity : _true_type { };

	/* scalar */

	template <typename _Tp>
	struct _is_scalar :
		_convert<typename std::is_scalar<_Tp>::type> { };

	template <typename _Tp>
	struct _is_trivially_destructible :
		_convert<typename std::is_trivially_destructible<_Tp>::type> { };

	/* or */
	template <typename _B1, typename _B2>
	struct _or {
		typedef _true_type type;
	};

	template <>
	struct _or<_false_type, _false_type>  {
		typedef _false_type type;
	};

	/* and */
	template <typename _B1, typename _B2>
	struct _and {
		typedef _false_type type;
	};

	template <>
	struct _and<_true_type, _true_type> {
		typedef _true_type type;
	};
}

#endif //_TYPE_BASE_H_

#ifndef _VECTOR_TRAITS_H_
#define _VECTOR_TRAITS_H_

namespace tools {

	template <typename _Vector>
	struct vector_trait {
		typedef typename _Vector::value_type   value_type;
		typedef typename _Vector::precise_type precise_type;
		typedef typename _Vector::size_type    size_type;
		typedef typename _Vector::length_type  length_type;

		static const size_type dimension = _Vector::dimension;
	};
}

#endif //_VECTOR_TRAITS_H_

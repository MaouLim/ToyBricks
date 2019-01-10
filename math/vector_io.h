#ifndef _VECTOR_IO_H_
#define _VECTOR_IO_H_

#include <ostream>
#include <string>
#include <sstream>

#include "vector_traits.h"

namespace tools {

	template <		
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	void from_tsv(
		const std::string&  csv_str, 
		vec<_Tp, _Precise>& dst
	) {
		// todo
	}

	template <
		size_t   _Length,
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	struct _to_tsv_impl;

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	std::string to_tsv(const vec<_Tp, _Precise>& src) {
		typedef vector_trait<vec<_Tp, _Precise>> trait_type;
		std::stringstream stream;
		return _to_tsv_impl<
			trait_type::dimension, _Tp, _Precise, vec
		>::element_sprint(stream, src).str();
	}

	template <
		size_t   _Length,
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	struct _to_tsv_impl {

		static std::stringstream& element_sprint(
			std::stringstream&        stream, 
			const vec<_Tp, _Precise>& src
		) {
			const size_t index = _Length - 1;
			_to_tsv_impl<
				index, _Tp, _Precise, vec
			>::element_sprint(stream, src) << src[index] << tab;
			return stream;
		}
	};

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
		struct _to_tsv_impl<0, _Tp, _Precise, vec> {

		static std::stringstream& element_sprint(
			std::stringstream& stream, const vec<_Tp, _Precise>&
		) { return stream; }
	};
}

namespace std {

	template <
		typename _Tp,
		typename _Precise,
		template <typename, typename> class vec
	>
	std::ostream& operator<<(
		std::ostream& stream, const vec<_Tp, _Precise>& v
	) {
		return stream << tools::to_tsv(v);
	}

}


#endif
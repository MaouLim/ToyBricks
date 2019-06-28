#ifndef _MATH_IO_H_
#define _MATH_IO_H_

#include <ostream>
#include <string>
#include <sstream>

#include "../common/defines.h"

#include "matrix.h"

namespace math {

	template <
		typename _Tp, tools::size_t _N
	>
	void from_tsv(
		const std::string& csv_str,
		vector<_Tp, _N>&   vec
	) {
		// todo
	}

	template <
		tools::size_t _Length,
		typename      _Tp,
		tools::size_t _N
	>
	struct _vec_to_tsv_impl;

	template <
		typename      _Tp,
		tools::size_t _N
	>
	std::string to_tsv(const vector<_Tp, _N>& src) {
		std::stringstream stream;
		return _vec_to_tsv_impl<
			_N, _Tp, _N
		>::element_sprint(stream, src).str();
	}

	template <
		tools::size_t _Length,
		typename      _Tp,
		tools::size_t _N
	>
	struct _vec_to_tsv_impl {

		static std::stringstream& element_sprint(
			std::stringstream&     stream,
			const vector<_Tp, _N>& src
		) {
			const tools::size_t index = _Length - 1;
			_vec_to_tsv_impl<
				index, _Tp, _N
			>::element_sprint(stream, src) << src[index] << tools::tab;
			return stream;
		}
	};

	template <
		typename      _Tp,
		tools::size_t _N
	>
	struct _vec_to_tsv_impl<0, _Tp, _N> {

		static std::stringstream& element_sprint(
			std::stringstream& stream, const vector<_Tp, _N>&
		) { return stream; }
	};

	template <
		tools::size_t _Length,
		typename      _Tp,
		tools::size_t _Rows,
		tools::size_t _Cols
	>
	struct _mat_to_tsv_impl;

	template <
		typename      _Tp,
		tools::size_t _Rows,
		tools::size_t _Cols
	>
	std::string to_tsv(const matrix<_Tp, _Rows, _Cols>& mat) {
		std::stringstream stream;
		return _mat_to_tsv_impl<
			_Rows * _Cols, _Tp, _Rows, _Cols
		>::element_sprint(stream, mat).str();
	}

	template <
		tools::size_t _Length,
		typename      _Tp,
		tools::size_t _Rows,
		tools::size_t _Cols
	>
	struct _mat_to_tsv_impl {

		static std::stringstream& element_sprint(
			std::stringstream&               stream,
			const matrix<_Tp, _Rows, _Cols>& src
		) {
			const tools::size_t rid = (_Length - 1) / _Cols;
			const tools::size_t cid = (_Length - 1) % _Cols;

			_mat_to_tsv_impl<
				_Length - 1, _Tp, _Rows, _Cols
			>::element_sprint(stream, src)
				<< src(rid, cid)
			    << (cid + 1 == _Cols ? tools::ret : tools::tab);
			return stream;
		}
	};

	template <
		typename      _Tp,
		tools::size_t _Rows,
		tools::size_t _Cols
	>
	struct _mat_to_tsv_impl<0, _Tp, _Rows, _Cols> {

		static std::stringstream& element_sprint(
			std::stringstream&               stream,
			const matrix<_Tp, _Rows, _Cols>& src
		) { return stream; }
	};
}

namespace math {

	template <
		typename _Tp, tools::size_t _N
	>
	std::ostream& operator<<(
		std::ostream& stream, const math::vector<_Tp, _N>& vec
	) {
		return stream << math::to_tsv(vec);
	}

	template <typename _Tp, tools::size_t _Rows, tools::size_t _Cols>
	std::ostream& operator<<(
		std::ostream& stream, const math::matrix<_Tp, _Rows, _Cols>& mat
	) {
		return stream << math::to_tsv(mat);
	}

}

#endif
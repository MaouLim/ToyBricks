/*
 * Created by Maou Lim on 2018/1/17.
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <utility>

#include "../common/defines.h"

namespace tools {

	template <typename _T>
	inline void construct(_T* ptr) {
		new (ptr) _T();
	}

	template <typename _T, typename... _Args>
	inline void construct(_T* ptr, _Args&&... args) {
		new (ptr) _T(std::forward<_Args>(args)...);
	};

	template <typename _InputIterator, typename... _Args>
	inline void construct(_InputIterator first,
	                      _InputIterator last,
	                      _Args&&...     args) {
		while (first != last) {
			construct(&(*first), std::forward<_Args>(args)...);
			++first;
		}
	};

	template <typename _T>
	inline void destroy(_T* ptr) {
		ptr->~_T();
	}

	template <typename _InputIterator>
	inline void destroy(_InputIterator first,
	                    _InputIterator last) {
		while (first != last) {
			destroy(&(*first));
			++first;
		}
	}

	template <typename _T, typename _Alloc>
	class standard_alloc {
	public:
		static _T* allocate(size_t n) {
			return 0 == n ? nullptr : (_T*) alloc.allocate(n * sizeof (_T));
		}

		static _T* allocate() {
			return (_T*) alloc.allocate(sizeof (_T));
		}

		static void deallocate(_T* p, size_t n) {
			if (0 == n) {
				return;
			}
			alloc.deallocate(p, n * sizeof (_T));
		}

		static void deallocate(_T* p) {
			alloc.deallocate( p, sizeof (_T));
		}

	private:
		static _Alloc alloc;
	};

	template <typename _T, typename _Alloc>
	_Alloc standard_alloc<_T, _Alloc>::alloc = _Alloc();
}

#endif //_MEMORY_H_

/*
 * Created by Maou Lim on 2019/1/10.
 */

#ifndef _PAIR_H_
#define _PAIR_H_

#include "algorithm.h"

namespace tools {

	template <typename _First, typename _Second>
	class pair {

		typedef _First                first_type;
		typedef _Second               second_type;
		typedef pair<_First, _Second> self_type;

	public:
		first_type  first;
		second_type second;

		pair() : first(), second() { }

		pair(const self_type& other) : first(other.first), second(other.second) { }

		template <typename _OtherFirst = _First, typename _OtherSecond = _Second>
		pair(const _OtherFirst& _first, const _OtherSecond& _second) :
			first(_first), second(_second) { }

		template <typename _OtherFirst, typename _OtherSecond>
		explicit pair(const pair<_OtherFirst, _OtherSecond>& other) :
			first(other.first),
			second(other.second) { }

		bool operator==(const self_type& other) const {
			return first == other.first && second == other.second;
		}

		bool operator!=(const self_type& other) const {
			return !this->operator==(other);
		}

		void swap(self_type& other)	{
			tools::swap(this->first, other.first);
			tools::swap(this->second, other.second);
		}
	};

	template <typename _First, typename _Second>
	pair<typename std::decay<_First>::type, typename std::decay<_Second>::type>
	    make_pair(
	    	_First&& first, _Second&& second
	) {
		typedef typename std::decay<_First>::type  first_type;
		typedef typename std::decay<_Second>::type second_type;

		return pair<first_type, second_type>(
			std::forward<_First>(first),
			std::forward<_Second>(second)
		);
	}
}

#endif //_PAIR_H_

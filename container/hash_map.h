/*
 * Created by Maou Lim on 2018/3/5.
 */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "../common/functor.h"
#include "hashtable.h"

namespace tools {

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _EqualKey,
		typename _Alloc
	>
	class hash_map {
	protected:
		//typedef _hashtable<_Key, std::pair<const _Key, _Val>, _Hash, >
	};

}

#endif //_HASH_MAP_H_

/*
 * Created by Maou Lim on 2018/2/12.
 */

#ifndef _HASH_SET_H_
#define _HASH_SET_H_

#include "../common/functor.h"
#include "hashtable.h"

namespace tools {

	template <
		typename _Val,
		typename _Hash,
		typename _EqualKey,
		typename _Alloc
	>
	class hash_set {
	protected:
		typedef _hashtable<
			_Val, _Val, _Hash, identity<_Val>, _EqualKey, _Alloc
		> rep_type;

	public:
		typedef typename rep_type::key_type   key_type;
		typedef typename rep_type::value_type value_type;
		typedef typename rep_type::hash_func  hash_func;
		typedef typename rep_type::key_equal  key_equal;

		typedef typename rep_type::size_type size_type;

		typedef typename rep_type::const_pointer   pointer;
		typedef typename rep_type::const_pointer   const_pointer;
		typedef typename rep_type::const_reference reference;
		typedef typename rep_type::const_reference const_reference;

		typedef typename rep_type::const_iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;

	protected:
		rep_type rep;

	public:
		explicit hash_set(size_type n = 100) : rep(n, hash_func(), key_equal()) { }
		//hash_set()

	public:
		/* size and capacity */
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_size(); }

		iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }

		/* modifier */
		void swap(hash_set& other) { rep.swap(other.rep); }
		void clear() { rep.clear(); }
		void resize(size_type hint) { rep.resize(hint); }

		/* iterators */
		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }
	};
}

#endif //_HASH_SET_H_

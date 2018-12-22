/*
 * Created by Maou Lim on 2018/2/2.
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "algorithm.h"
#include "single_list.h"
#include "sequence.h"

namespace tools {

	template <typename _Val>
	using _bucket_node = slist_node<_Val>;

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	class _hashtable;

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	struct _hashtable_iterator_base {
	public:
		typedef _Val value_type;

	protected:
		typedef _hashtable<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> hashtable_type;

		typedef _bucket_node<_Val> node_type;
		typedef node_type*         link_type;

	protected:
		link_type                   current;
		const hashtable_type* const hashtable;

	protected:
		static link_type& next(link_type p) { return (link_type&) p->next; }

	public:
		explicit _hashtable_iterator_base(node_type* p, const hashtable_type* const table) :
			current(p), hashtable(table) { }
	};

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	struct _const_hashtable_iterator :
		public _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> {
	protected:
		typedef _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> base_type;

		typedef _const_hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> self_type;

		typedef typename base_type::hashtable_type hashtable_type;
		typedef typename base_type::node_type      node_type;
		typedef typename base_type::link_type      link_type;

	public:
		typedef std::forward_iterator_tag      iterator_category;
		typedef typename base_type::value_type value_type;
		typedef const value_type*              pointer;
		typedef const value_type&              reference;
		typedef ptrdiff_t                      difference_type;

		typedef typename hashtable_type::size_type size_type;

	public:
		explicit _const_hashtable_iterator(node_type* p, const hashtable_type* const table) :
			base_type(p, table) { }

		explicit _const_hashtable_iterator(const base_type& other) : base_type(other) { }

		reference operator*() const { return current->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			const link_type prev = current;
			current = next(current);
			if (nullptr == current) {
				size_type index = hashtable->bucket_index(prev->val);
				while (nullptr == current && ++index < hashtable->buckets.size()) {
					current = hashtable->buckets[index];
				}
			}

			return *this;
		}

		self_type operator++(int) {
			auto tmp = *this;
			this->operator++();
			return tmp;
		}
	};

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	struct _hashtable_iterator :
		public _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> {
	protected:
		typedef _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> base_type;

		typedef _hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> self_type;

		typedef typename base_type::hashtable_type hashtable_type;
		typedef typename base_type::node_type      node_type;
		typedef typename base_type::link_type      link_type;

	public:
		typedef std::forward_iterator_tag      iterator_category;
		typedef typename base_type::value_type value_type;
		typedef value_type*                    pointer;
		typedef value_type&                    reference;
		typedef ptrdiff_t                      difference_type;

		typedef typename hashtable_type::size_type size_type;

	public:
		explicit _hashtable_iterator(node_type* p, const hashtable_type* const table) :
			base_type(p, table) { }

		reference operator*() const { return current->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			const link_type prev = current;
			current = next(current);
			if (nullptr == current) {
				size_type index = hashtable->bucket_index(prev->val);
				while (nullptr == current && ++index < hashtable->buckets.size()) {
					current = hashtable->buckets[index];
				}
			}

			return *this;
		}

		self_type operator++(int) {
			auto tmp = *this;
			this->operator++();
			return tmp;
		}
	};

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	inline bool
	operator==(
		const _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		>& left,
	    const _hashtable_iterator_base<
		    _Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
	    >& right
	) {
		return left.current == right.current;
	}

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	inline bool
	operator!=(
		const _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		>& left,
		const _hashtable_iterator_base<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		>& right
	) {
		return !(left == right);
	}

	static const int _primes_amount = 28;

	static const unsigned long _prime_list[_primes_amount] = {
		53,         97,           193,         389,       769,
		1543,       3079,         6151,        12289,     24593,
		49157,      98317,        196613,      393241,    786433,
		1572869,    3145739,      6291469,     12582917,  25165843,
		50331653,   100663319,    201326611,   402653189, 805306457,
		1610612741, 3221225473ul, 4294967291ul
	};

	inline static unsigned long _next_prime(unsigned long n) {
		const unsigned long* first = _prime_list;
		const unsigned long* last  = _prime_list + _primes_amount;
		const unsigned long* pos   = lower_bound(first, last, n);

		return last == pos ? *(last - 1) : *pos;
	}

	template <
		typename _Key,
		typename _Val,
		typename _Hash,
		typename _ExtractKey,
		typename _EqualKey,
		typename _Alloc
	>
	class _hashtable {
	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef _Key        key_type;
		typedef _Hash       hash_func;
		typedef _EqualKey   key_equal;
		typedef _ExtractKey key_extract;

		typedef size_t      size_type;

	protected:
		typedef _bucket_node<_Val>                node_type;
		typedef node_type*                        link_type;
		typedef sequence<link_type, _Alloc>       buckets_type;
		typedef standard_alloc<node_type, _Alloc> allocator_type;

		typedef _hashtable<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> self_type;

		friend struct _hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		>;

		friend struct _const_hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		>;

	protected:
		hash_func   hash;
		key_equal   equal;
		key_extract key_of;

		buckets_type buckets;
		size_type    count_elements;

	public:
		/* size and capacity */
		size_type count_buckets() const { return buckets.size(); }
		size_type max_buckets_amount() const { return _prime_list[_primes_amount - 1]; }

		bool empty() const { return 0 == count_elements; }
		size_type size() const { return count_elements; }
		size_type max_size() const { return size_type(-1); }

	protected:
		link_type create_node(const value_type& val) {
			link_type p = allocator_type::allocate();
			construct(p, val);
			return p;
		}

		void destroy_node(link_type p) {
			destroy(p);
			allocator_type::deallocate(p);
		}

	protected:
		typedef _hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> inner_iterator;

		typedef _const_hashtable_iterator<
			_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc
		> const_inner_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

	private:
		static size_type _next_size(size_type n) { return _next_prime(n); }

		void _initialize_buckets(size_type n) {
			const size_type size = _next_size(n);
			buckets.reserve(size);
			buckets.insert(buckets.end(), size, nullptr);
			count_elements = 0;
		}

		void _copy_from(const self_type& other) {
			buckets.clear();

			const size_type size = other.buckets.size();
			buckets.reserve(size);
			buckets.insert(buckets.end(), size, nullptr);

			for (size_type i = 0; i < size; ++i) {
				if (nullptr != other.buckets[i]) {
					buckets[i] =
						create_node(((link_type) other.buckets[i])->val);

					link_type p =  (link_type) ((link_type) other.buckets[i])->next;
					link_type q = buckets[i];

					while (nullptr != p) {
						q->next = create_node(p->val);
						p = (link_type) p->next;
						q = (link_type) q->next;
					}
				}
			}

			count_elements = other.count_elements;
		}

		size_type _bkt_index_key(const key_type& key, size_type n) const {
			return hash(key) % n;
		}

		size_type _bkt_index_key(const key_type& key) const {
			return _bkt_index_key(key, buckets.size());
		}

		std::pair<iterator, bool> _insert_unique_noresize(const value_type& val) {
			const size_type index = bucket_index(val);
			link_type head = buckets[index];

			for (link_type current = head; nullptr != current;) {
				if (equal(key_of(current->val), key_of(val))) {
					return std::pair<iterator, bool>(inner_iterator(current, this), false);
				}
				current = current->get_next();
			}

			link_type new_node = create_node(val);
			new_node->next = head;
			buckets[index] = new_node;
			++count_elements;
			return std::pair<iterator, bool>(inner_iterator(new_node, this), true);
		}

	protected:
		size_type bucket_index(const value_type& val, size_type n) {
			return _bkt_index_key(key_of(val), n);
		}

		size_type bucket_index(const value_type& val) const {
			return _bkt_index_key(key_of(val));
		}

	public:
		_hashtable(size_type n, const hash_func& h, const key_equal& eq) :
			hash(h), equal(eq), count_elements(0) { _initialize_buckets(n); }

		_hashtable(const self_type& other) { _copy_from(other); }

		~_hashtable() { clear(); }

	public:
		self_type& operator=(const self_type& other) {
			if (&other == this) {
				return *this;
			}
			_copy_from(other);
		}

	public:
		void clear() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				link_type p = buckets[i];
				while (nullptr != p) {
					link_type tmp = p;
					p = p->get_next();
					destroy_node(tmp);
				}
				buckets[i] = nullptr;
			}
			count_elements = 0;
		}

		void swap(self_type& other) {
			buckets.swap(other.buckets);
			size_type tmp = count_elements;
			count_elements = other.count_elements;
			other.count_elements = tmp;
		}

		size_type count(const key_type& key) const {
			const size_type index = _bkt_index_key(key);
			size_type res = 0;
			for (link_type p = buckets[index]; nullptr != p;) {
				if (equal(key_of(p->val), key)) {
					++res;
				}
				p = p->get_next();
			}
			return res;
		}

		iterator find(const key_type& key) const {
			const size_type index = _bkt_index_key(key);
			link_type p = buckets[index];
			while (nullptr != p && !equal(key_of(p->val), key)) {
				p = p->get_next();
			}
			return inner_iterator(p, this);
		}

		reference find_or_insert(const value_type& val) {
			resize(count_elements + 1); // resize的时机可以优化。

			const size_type index = bucket_index(val);
			link_type head = buckets[index];

			for (link_type p = head; nullptr != p;) {
				if (equal(key_of(p->val), key_of(val))) {
					return p->val;
				}
				p = p->get_next();
			}

			link_type new_node = create_node(val);
			new_node->next = head;
			buckets[index] = new_node;
			++count_elements;
			return new_node->val;
		}

		void resize(size_type hint) {
			const size_type count_bkt = buckets.size();
			if (count_bkt < hint) {
				const size_type new_size = _next_size(hint);
				if (count_bkt < new_size) {
					buckets_type tmp(new_size, nullptr);

					for (size_type index = 0; index < count_bkt; ++index) {
						link_type p = buckets[index];

						while (nullptr != p) {
							size_type new_index = bucket_index(p->val, new_size);
							buckets[new_index] = p->next;
							p->next = tmp[new_index];
							tmp[new_index] = p;
							p = buckets[new_index];
						}
					}
					buckets.swap(tmp);
				}
			}
		}

		std::pair<iterator, bool> insert_unique(const value_type& val) {
			resize(count_elements + 1);
			return _insert_unique_noresize(val);
		}

		iterator insert_equal(const value_type& val) {
			const size_type index = bucket_index(val);
			link_type head = buckets[index];

			for (link_type current = head; nullptr != current;) {
				if (equal(key_of(current->val), key_of(val))) {
					link_type new_node = create_node(val);
					new_node->next = current->next;
					current->next = new_node;
					++count_elements;
					return inner_iterator(new_node, this);
				}
				current = current->get_next();
			}

			link_type new_node = create_node(val);
			new_node->next = head;
			buckets[index] = new_node;
			++count_elements;
			return inner_iterator(new_node, this);
		}

		size_type erase(const key_type& key) {
			const size_type index = _bkt_index_key(key);
			link_type head = buckets[index];

			if (nullptr != head) {
				link_type current = head;
				link_type next    = current->get_next();
				while (nullptr != next) {
					if (equal(key_of(next->val), key)) {
						// todo
					}
				}
			}
		}

		void erase(const_iterator first, const_iterator last) {
			// todo
		}

		/* iterators */
		iterator begin() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				if (nullptr != buckets[i]) {
					return inner_iterator(buckets[i], this);
				}
			}

			return inner_iterator(nullptr, this);
		}

		const_iterator begin() const {
			for (size_type i = 0; i < buckets.size(); ++i) {
				if (nullptr != buckets[i]) {
					return const_inner_iterator(buckets[i], this);
				}
			}

			return const_inner_iterator(nullptr, this);
		}

		iterator end() { return inner_iterator(nullptr, this); }
		const_iterator end() const { return const_inner_iterator(nullptr, this); }
	};
}

#endif //_HASHTABLE_H_

















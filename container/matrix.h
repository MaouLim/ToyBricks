/*
 * Created by Maou Lim on 2018/11/4.
 */

#ifndef _MATRIX_H_
#define _MATRIX_H_


#include "../common/defines.h"
#include "sequence.h"

namespace tools {

	enum class matrix_init { zero, identity, random };

	template <typename _Val>
	struct _col_iterator_base {
		typedef std::random_access_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;
		typedef _Val                            value_type;
		typedef _Val*                           base_ptr;

		base_ptr current;

		_col_iterator_base(base_ptr p) : current(p) { }
	};

	template <
		typename _Val,
		size_t   _Rows,
		size_t   _Cols
	>
	struct _col_iterator : _col_iterator_base<_Val> {
	protected:
		typedef _col_iterator<_Val, _Rows, _Cols> self_type;
		typedef _col_iterator_base<_Val>          base_type;

	public:
		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::ptrdiff_t         difference_type;
		typedef typename base_type::value_type        value_type;
		typedef value_type*                           pointer;
		typedef value_type&                           reference;

		_col_iterator(pointer ptr) : base_type(ptr) { }

		reference operator*() { return *base_type::current; }
		pointer operator->() { return &(operator*()); }

		self_type& operator++() { base_type::current += _Cols; return *this; }
		self_type operator++(int) { self_type tmp = *this; base_type::current += _Cols; return tmp; }

		self_type& operator--() { base_type::current -= _Cols; return *this; }
		self_type operator--(int) { self_type tmp = *this; base_type::current -= _Cols; return tmp; }

		reference operator[](difference_type n) const { return base_type::current[n * _Cols]; }

		self_type& operator+=(difference_type n) { base_type::current += n * _Cols; return *this; }
		self_type operator+(difference_type n) const { return self_type(base_type::current + n * _Cols); }

		self_type& operator-=(difference_type n) { base_type::current -= n * _Cols; return *this; }
		self_type operator-(difference_type n) const { return self_type(base_type::current - n * _Cols); }
	};

	template <
		typename _Val,
		size_t   _Rows,
		size_t   _Cols
	>
	struct _const_col_iterator : _col_iterator_base<_Val> {
	protected:
		typedef _const_col_iterator<_Val, _Rows, _Cols> self_type;
		typedef _col_iterator_base<_Val>                base_type;

	public:
		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::ptrdiff_t         difference_type;
		typedef typename base_type::value_type        value_type;
		typedef const value_type*                     pointer;
		typedef const value_type&                     reference;

		_const_col_iterator(pointer ptr) : base_type(ptr) { }

		reference operator*() { return *base_type::current; }
		pointer operator->() { return &(operator*()); }

		self_type& operator++() { base_type::current += _Cols; return *this; }
		self_type operator++(int) { self_type tmp = *this; base_type::current += _Cols; return tmp; }

		self_type& operator--() { base_type::current -= _Cols; return *this; }
		self_type operator--(int) { self_type tmp = *this; base_type::current -= _Cols; return tmp; }

		reference operator[](difference_type n) const { return base_type::current[n * _Cols]; }

		self_type& operator+=(difference_type n) { base_type::current += n * _Cols; return *this; }
		self_type operator+(difference_type n) const { return self_type(base_type::current + n * _Cols); }

		self_type& operator-=(difference_type n) { base_type::current -= n * _Cols; return *this; }
		self_type operator-(difference_type n) const { return self_type(base_type::current - n * _Cols); }
	};

	template <typename _Val>
	bool operator==(const _col_iterator_base<_Val>& left,
		            const _col_iterator_base<_Val>& right) {
		return left.current == right.current;
	}

	template <typename _Val, size_t _Rows, size_t _Cols>
	struct fixed_matrix_attrs {

		typedef size_t size_type;
		typedef _Val   value_type;

		static const size_t rows      = _Rows;
		static const size_t cols      = _Cols;
		static const size_t elements  = _Rows * _Cols;
	};

	template <
		typename _Val,
		size_t   _Rows,
		size_t   _Cols
	>
	class fixed_matrix {

		template <typename _FixedMatrix, size_t _Row, size_t _Col>
		friend typename _FixedMatrix::value_type& get(_FixedMatrix& mat);

		/* type traits */
	protected:
		typedef fixed_matrix<_Val, _Rows, _Cols>       self_type;
		typedef size_t                                 size_type;
		typedef tools::fixed_seq<_Val, _Rows * _Cols>  buff_type;

	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef fixed_matrix_attrs<_Val, _Rows, _Cols> attributes;

		typedef typename buff_type::iterator       iterator;
		typedef typename buff_type::const_iterator const_iterator;

		typedef typename buff_type::reverse_iterator       reverse_iterator;
		typedef typename buff_type::const_reverse_iterator const_reverse_iterator;

		typedef iterator       row_iterator;
		typedef const_iterator const_row_iterator;

		typedef _reverse_iterator<row_iterator>       reverse_row_iterator;
		typedef _reverse_iterator<const_row_iterator> const_reverse_row_iterator;

		typedef _iterator_wrapper<_col_iterator<_Val, _Rows, _Cols>, self_type>       col_iterator;
		typedef _iterator_wrapper<_const_col_iterator<_Val, _Rows, _Cols>, self_type> const_col_iterator;

		typedef _reverse_iterator<col_iterator>       reverse_col_iterator;
		typedef _reverse_iterator<const_col_iterator> const_reverse_col_iterator;

	public:
		/* constructors */
		fixed_matrix() { m_buff.fill(static_cast<value_type>(0)); }

		template <typename _InputIterator>
		fixed_matrix(_InputIterator first, _InputIterator last) {
			tools::copy(first, last, m_buff.begin());
		}

		fixed_matrix(const std::initializer_list<value_type>& init) {
			assert(init.size() <= attributes::elements);
			tools::copy(init.begin(), init.end(), m_buff.begin());
		}

		fixed_matrix(const self_type& other) {
			tools::copy(other.begin(), other.end(), m_buff.begin());
		}

		/* operators */
		explicit operator buff_type() const { return m_buff; }

		const_row_iterator operator[](size_type row) const { return row_begin(row); }
		row_iterator operator[](size_type row) { return row_begin(row); }

		const_reference operator()(size_type row, size_type col) const {
			assert(row < _Rows && col < _Cols);
			return m_buff[_Cols * row + col];
		}

		reference operator()(size_type row, size_type col) {
			return const_cast<value_type&>(
				(*const_cast<const self_type*>(this))(row, col)
			);
		}

		self_type& operator=(const self_type& other) {
			if (this == &other) { return *this; }
			tools::copy(
				other.begin(),
				other.end(),
				m_buff.begin()
			);
			return *this;
		}

		size_type rows() const { return _Rows; }
		size_type cols() const { return _Cols; }
		size_type size() const { return attributes::elements; }

		void fill(const value_type& val) { m_buff.fill(val); }
		void swap(self_type& other) { m_buff.swap(other.m_buff); }

		const_pointer data() const { return m_buff.data(); }
		pointer data() { return m_buff.data(); }

		iterator begin() { return m_buff.begin(); }
		iterator end() { return m_buff.end(); }

		const_iterator begin() const { return m_buff.begin(); }
		const_iterator end() const { return m_buff.end(); }

		reverse_iterator rbegin() { return m_buff.rbegin(); }
		reverse_iterator rend() { return m_buff.rend(); }

		const_reverse_iterator rbegin() const { return m_buff.rbegin(); }
		const_reverse_iterator rend() const { return m_buff.rend(); }

		row_iterator row_begin(size_type row) { return m_buff.begin() + row * _Cols; }
		row_iterator row_end(size_type row) { return m_buff.begin() + (row + 1) * _Cols; }

		const_row_iterator row_begin(size_type row) const { return m_buff.begin() + row * _Cols; }
		const_row_iterator row_end(size_type row) const { return m_buff.begin() + (row + 1) * _Cols; }

		reverse_row_iterator row_rbegin(size_type row) { return reverse_row_iterator(row_end(row)); }
		reverse_row_iterator row_rend(size_type row) { return reverse_row_iterator(row_begin(row)); }

		const_reverse_row_iterator row_rbegin(size_type row) const { return const_reverse_row_iterator(row_end(row)); }
		const_reverse_row_iterator row_rend(size_type row) const { return const_reverse_row_iterator(row_begin(row)); }

		col_iterator col_begin(size_type col) { return col_iterator(m_buff.data() + col); }
		col_iterator col_end(size_type col) { return col_iterator(m_buff.data() + attributes::elements + col); }

		const_col_iterator col_begin(size_type col) const { return const_col_iterator(m_buff.data() + col); }
		const_col_iterator col_end(size_type col) const { return const_col_iterator(m_buff.data() + attributes::elements + col); }

		reverse_col_iterator col_rbegin(size_type col) { return reverse_col_iterator(col_end(col)); }
		reverse_col_iterator col_rend(size_type col) { return reverse_col_iterator(col_begin(col)); }

		const_reverse_col_iterator col_rbegin(size_type col) const { return const_reverse_col_iterator(col_end(col)); }
		const_reverse_col_iterator col_rend(size_type col) const { return const_reverse_col_iterator(col_begin(col)); }

	private:
		buff_type m_buff;
	};

	template <typename _Val, size_t _Rank>
	using fixed_square_matrix = fixed_matrix<_Val, _Rank, _Rank>;

	template <typename _FixedMatrix>
	struct is_square_matrix :
		_bool_type<
		    _FixedMatrix::attributes::rows == _FixedMatrix::attributes::cols
		> { };

	template <typename _FixedMatrix, size_t _Row, size_t _Col>
	typename _FixedMatrix::value_type& get(_FixedMatrix& mat) {
		typedef typename _FixedMatrix::attributes attrs_type;
		const auto rows = attrs_type::rows;
		const auto cols = attrs_type::cols;

		static_assert(_Row < rows && _Col < cols, "index out of range.");

		return mat.m_buff[_Row * rows + _Col];
	}

	template <typename _Val, typename _Container = tools::sequence<_Val>>
	class matrix {
		typedef matrix<_Val, _Container> self_type;
		typedef _Container               container_type;
		typedef size_t                   size_type;

	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef typename container_type::iterator       iterator;
		typedef typename container_type::const_iterator const_iterator;

		typedef iterator       row_iterator;
		typedef const_iterator const_row_iterator;

		matrix(size_type rows, size_type cols, matrix_init mode = matrix_init::zero) :
			m_rows(rows), m_cols(cols)
		{
			switch (mode) {
				case matrix_init::zero : {
					m_buff.resize(m_rows * m_cols, value_type(0));
					break;
				}
				case matrix_init::identity : { break; } //todo
				case matrix_init::random : { break; } //todo
				default : { break; }
			}
		}

		template <typename _InputIterator>
		matrix(
			size_type rows, size_type cols, _InputIterator first, _InputIterator last
		) : m_rows(rows), m_cols(cols)
		{
			const size_type buff_size = rows * cols;
			m_buff.reserve(buff_size);

			size_type count = 0;
			while (first != last && count < buff_size) {
				m_buff.emplace_back(*first);
				++first; ++count;
			}
		}

		matrix(const self_type& other) :
			m_rows(other.m_rows), m_cols(other.m_cols), m_buff(other.m_buff) { }

		matrix(self_type&& other) :
			m_rows(other.m_rows), m_cols(other.m_cols), m_buff(std::move(other.m_buff)) { }

		size_type rows() const { return m_rows; }
		size_type cols() const { return m_cols; }
		size_type size() const { return m_rows * m_cols; }

		bool squared()  const { return m_rows == m_cols; }
		bool degraded() const { return 0 == m_rows || 0 == m_cols; }

		self_type& operator=(const self_type& other) {
			if (this == &other) { return *this; }

			m_rows = other.m_rows;
			m_cols = other.m_cols;
			m_buff = other.m_buff;
			return *this;
		}

		self_type& operator=(self_type&& other) {
			if (this == &other) { return *this; }

			m_rows = other.m_rows;
			m_cols = other.m_cols;
			m_buff = std::move(other.m_buff);
			return *this;
		}

		row_iterator operator[](size_type row) { return row_begin(row); }
		const_row_iterator operator[](size_type row) const { return row_begin(row); }

		reference operator()(size_type row, size_type col) {
			assert(row < m_rows && col < m_cols);
			return m_buff[m_cols * row + col];
		}

		const_reference operator()(size_type row, size_type col) const {
			assert(row < m_rows && col < m_cols);
			return m_buff[m_cols * row + col];
		}

		self_type& reshape(size_type new_rows, size_type new_cols) {
			self_type tmp(new_rows, new_cols);

			size_type less_rows = tools::min(new_rows, m_rows);
			size_type less_cols = tools::min(new_cols, m_cols);

			for (size_type r = 0; r < less_rows; ++r) {
				for (size_type c = 0; c < less_cols; ++c) {
					tmp(r, c) = this->operator()(r, c);
				}
			}

			this->swap(tmp);
			return *this;
		}

		template <typename _Condition>
		self_type select(_Condition cond) const {
			self_type result(0, this->m_cols);

			for (size_type r = 0; r < m_rows; ++r) {
				if (cond(r, row_begin(r), row_end(r))) {
					result.push_bottom(row_begin(r), row_end(r));
				}
			}

			return result;
		}

		template <typename _InputIterator>
		void push_bottom(_InputIterator first, _InputIterator last) {

			m_buff.reserve((m_rows + 1) * m_cols);

			size_type i = 0;
			while (first != last && i < m_cols) {
				m_buff.emplace_back(*first);
				++first; ++i;
			}

			while (i < m_cols) {
				m_buff.emplace_back(value_type(0));
				++i;
			}

			++m_rows;
		}

		template <typename _InputIterator>
		void push_top(_InputIterator first, _InputIterator last) {
			container_type tmp;
			tmp.reverse((m_rows + 1) * m_cols);

			size_type i = 0;
			while (first != last && i < m_cols) {
				tmp.emplace_back(*first);
				++first; ++i;
			}

			while (i < m_cols) {
				tmp.emplace_back(value_type(0));
				++i;
			}

			for_each(
				m_buff.begin(), m_buff.end(),
				[&tmp](const value_type& item) { tmp.emplace_back(item); }
			);

			m_buff.swap(tmp); ++m_rows;
		}

		template <typename _InputIterator>
		void push_left(_InputIterator first, _InputIterator last) {
			const size_type new_size = m_rows * (m_cols + 1);

			container_type tmp;
			tmp.reverse(new_size);

			auto itr = m_buff.begin();

			for (size_type i = 0; i < new_size; ++i) {
				if (0 == i % (m_cols + 1)) {
					if (first != last) {
						tmp.emplace_back(*first); ++first;
					}
					else { tmp.emplace_back(value_type(0)); }
				}
				else { tmp.emplace_back(*itr); ++itr; }
			}

			m_buff.swap(tmp); ++m_cols;
		}

		template <typename _InputIterator>
		void push_right(_InputIterator first, _InputIterator last) {
			const size_type new_size = m_rows * (m_cols + 1);

			container_type tmp;
			tmp.reverse(new_size);

			auto itr = m_buff.begin();

			for (size_type i = 0; i < new_size; ++i) {
				if (0 == (i + 1) % (m_cols + 1)) {
					if (first != last) {
						tmp.emplace_back(*first); ++first;
					}
					else { tmp.emplace_back(value_type(0)); }
				}
				else { tmp.emplace_back(*itr); ++itr; }
			}

			m_buff.swap(tmp); ++m_cols;
		}

		void fill(const value_type& v) {
			tools::fill_n(begin(), m_rows * m_cols, v);
		}

		void swap(self_type& other) {
			tools::swap(other.m_rows, m_rows);
			tools::swap(other.m_cols, m_cols);
			m_buff.swap(other.m_buff);
		}

		const_pointer data() const { return m_buff.data(); }
		pointer data() { return m_buff.data(); }

		iterator begin() { return m_buff.begin(); }
		iterator end() { return m_buff.end(); }

		const_iterator begin() const { return m_buff.begin(); }
		const_iterator end() const { return m_buff.end(); }

		row_iterator row_begin(size_type row) { return m_buff.begin() + row * m_cols; }
		row_iterator row_end(size_type row) { return m_buff.begin() + (row + 1) * m_cols; }

		const_row_iterator row_begin(size_type row) const { return m_buff.begin() + row * m_cols; }
		const_row_iterator row_end(size_type row) const { return m_buff.begin() + (row + 1) * m_cols; }

	private:
		size_type m_rows, m_cols;
		container_type m_buff;
	};

	template <
	    typename _Val,
	    typename _Index             = size_t,
	    typename _ContainerForVal   = tools::sequence<_Val>,
	    typename _ContainerForIndex = tools::sequence<_Index>
	>
	class spare_matrix {
	public:
		typedef _Index     index_type;
		typedef index_type size_type;

		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

	protected:
		typedef spare_matrix<
		    _Val, _Index, _ContainerForVal, _ContainerForIndex
		> self_type;

		typedef _ContainerForIndex index_vec;
		typedef _ContainerForVal   value_vec;

//		spare_matrix(dim_type dim, mat_type type) : m_dim(dim) {
//			if (mat_type::ZERO == type) {
//				m_row_index.resize(m_dim + 1, index_type(0));
//				return;
//			}
//
//			m_row_index.reserve(dim + 1);
//			m_col_index.reserve(dim);
//			m_values.reserve(dim);
//
//			for (index_type i = 0; i < m_dim; ++i) {
//				m_row_index.push_back(i);
//				m_col_index.push_back(i);
//				m_values.push_back(one);
//			}
//
//			m_row_index.push_back(dim);
//		}
		// todo
	};
};

#endif //_MATRIX_H_

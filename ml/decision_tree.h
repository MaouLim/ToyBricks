/*
 * Created by Maou Lim on 2018/12/17.
 */

#ifndef _DECISION_TREE_H_
#define _DECISION_TREE_H_


#include <cassert>
#include <cmath>

#include <ostream>
#include <limits>

#include <map>
#include <vector>
#include <set>

namespace ml {

	template <typename _Attr, typename _Val>
	struct _dt_node {
	public:
		typedef _Attr  attr_type;
		typedef _Val   value_type;

	private:
		typedef _dt_node<_Attr, _Val> node_type;
		typedef node_type*              link_type;

	public:
		static const attr_type  invalid_attr;
		static const value_type invalid_value;

		std::vector<link_type> children;
		attr_type              attribute;
		value_type             choice;
		value_type             value;

		_dt_node() : attribute(invalid_attr), choice(invalid_value), value(invalid_value) { }
	};

	template <typename _Attr, typename _Val>
	const _Attr _dt_node<_Attr, _Val>::invalid_attr(-1); // no-numeric _Attr will throw compiling error

	template <typename _Attr, typename _Val>
	const _Val _dt_node<_Attr, _Val>::invalid_value(-1); // no-numeric _Label will throw compiling error

	template <typename _Attr, typename _Val>
	class decision_tree {
	public:
		typedef _Attr  attr_type;
		typedef _Val   value_type;
		typedef _Val   label_type;

	private:
		typedef _dt_node<_Attr, _Val> node_type;
		typedef node_type*            link_type;

	public:
		decision_tree() : m_root(nullptr) { }

		template <typename _Matrix2D>
		explicit decision_tree(const _Matrix2D& matrix) : m_root(nullptr) {
			_build(matrix, m_root);
		}

		~decision_tree() { _destroy(m_root); }

		template <typename _Matrix2D>
		void build(const _Matrix2D& matrix) {
			assert(0 < matrix.rows() && 0 < matrix.cols());

			// to clear the old decision tree
			_destroy(m_root);

			std::vector<bool> blacklist(matrix.cols(), false);
			blacklist.back() = true;
			size_t count_list = 1;

			_build(matrix, m_root, node_type::invalid_value, blacklist, count_list);
		}

		void print(std::ostream& stream) const { _print(stream, m_root, 0); }

	private:
		static link_type _create_node(
			const attr_type&  attr,
			const value_type& choice,
			const value_type& label
		) {
			link_type p = new node_type();
			p->attribute = attr;
			p->choice = choice;
			p->value = label;
			return p;
		}

		template <typename _Matrix2D>
		static double _info_gain(const _Matrix2D& matrix, const attr_type& attr) {

			double gain = 0.0;
			std::map<label_type, size_t> count_label_vals;

			for (auto r = 0; r < matrix.rows(); ++r) {
				label_type label = matrix[r][matrix.cols() - 1];
				auto lab_itr = count_label_vals.find(label);
				if (count_label_vals.end() == lab_itr) {
					count_label_vals.insert(
						lab_itr, std::make_pair((label_type) label, (size_t) 1)
					);
				}
				else { ++lab_itr->second; }
			}

			for (const auto& label_count : count_label_vals) {
				double p = (double) label_count.second / matrix.rows();
				gain -= p * std::log2(p);
			}

			count_label_vals.clear();
			std::map<value_type, size_t> count_attr_vals;

			for (auto r = 0; r < matrix.rows(); ++r) {
				value_type val = matrix[r][attr];
				auto attr_itr = count_attr_vals.find(val);
				if (count_attr_vals.end() == attr_itr) {
					count_attr_vals.insert(
						attr_itr, std::make_pair((value_type) val, (size_t) 1)
					);
				}
				else { ++attr_itr->second; }
			}

			for (const auto& val_count : count_attr_vals) {
				int total = 0;
				double entropy = 0.0;
				count_label_vals.clear();

				for (auto r = 0; r < matrix.rows(); ++r) {
					if (val_count.first == matrix[r][attr]) {
						label_type label = matrix[r][matrix.cols() - 1];
						auto lab_itr = count_label_vals.find(label);
						if (count_label_vals.end() == lab_itr) {
							count_label_vals.insert(
								lab_itr, std::make_pair((label_type) label, (size_t) 1)
							);
						}
						else { ++lab_itr->second; }
						++total;
					}
				}

				for (const auto& label_count : count_label_vals) {
					double p = (double) label_count.second / total;
					entropy -= p * std::log2(p);
				}

				gain -= (double) total / matrix.rows() * entropy;
			}

			return gain;
		}

		template <typename _Matrix2D>
		static attr_type _max_gain_attr(const _Matrix2D& matrix, const std::vector<bool>& blacklist) {
			assert(matrix.cols() == blacklist.size());

			attr_type max_attr = node_type::invalid_attr;
			double max_gain = -std::numeric_limits<double>::max();

			for (attr_type i = 0; i < matrix.cols(); ++i) {
				if (blacklist[i]) { continue; }

				double gain = _info_gain(matrix, i);
				if (max_gain < gain) { max_gain = gain; max_attr = i; }
			}

			return max_attr;
		}

		template <typename _Matrix2D>
		static label_type _most_label(const _Matrix2D& matrix) {
			std::map<label_type, size_t> count_label_vals;

			label_type most_label = node_type::invalid_value;
			size_t     most_count = 0;

			for (auto r = 0; r < matrix.rows(); ++r) {
				label_type label = matrix[r][matrix.cols() - 1];
				auto itr = count_label_vals.find(label);
				if (count_label_vals.end() == itr) {
					count_label_vals.insert(
						itr, std::make_pair((label_type) label, (size_t) 1));
				}
				else { ++itr->second; }

				if (most_count < itr->second) {
					most_count = itr->second;
					most_label = label;
				}
			}

			return most_label;
		}

		template <typename _Matrix2D>
		static bool _same_label(const _Matrix2D& matrix) {
			for (size_t r = 1; r < matrix.rows(); ++r) {
				if (matrix[  r  ][matrix.cols() - 1] !=
				    matrix[r - 1][matrix.cols() - 1]) {
					return false;
				}
			}

			return true;
		}

		template <typename _Matrix2D>
		static void _build(
			const _Matrix2D&   matrix,
			link_type&         root,
			value_type         choice,
			std::vector<bool>  blacklist,
			size_t             count_list
		) {
			if (_same_label(matrix)) {
				root = _create_node(
					node_type::invalid_attr,
					choice,
					matrix[0][matrix.cols() - 1]
				);
				return;
			}

			if (blacklist.size() == count_list) {
				root = _create_node(
					node_type::invalid_attr,
				    choice,
				    _most_label(matrix)
				);
				return;
			}

			attr_type id = _max_gain_attr(matrix, blacklist);
			assert(node_type::invalid_attr != id);
			blacklist[id] = true; ++count_list;

			root = _create_node(id, choice, node_type::invalid_value);

			std::set<attr_type> value_set;

			for (auto r = 0; r < matrix.rows(); ++r) {
				auto val = matrix[r][id];
				auto itr = value_set.find(val);
				if (value_set.end() == itr) {
					value_set.insert(itr, val);

					typedef typename _Matrix2D::const_row_iterator iterator;

					auto sub = matrix.select(
						[id, val](size_t, iterator first, iterator) { return val == first[id]; }
					);

					root->children.emplace_back(nullptr);
					_build(sub, root->children.back(), val, blacklist, count_list);
				}
			}
		}

		static void _destroy(link_type& root) {
			if (nullptr == root) { return; }
			for (auto& each : root->children) {
				_destroy(each);
			}
			delete root;
			root = nullptr;
		}

		static void _print(std::ostream& stream, link_type root, size_t indent) {
			if (nullptr == root) { return; }

			for (size_t i = 0; i < indent; ++i) { stream << ' '; }

			if (node_type::invalid_value != root->choice) {
				stream << "choice: " << root->choice << "->";
			}

			if (root->children.empty()) {
				stream << "label: " << root->value << std::endl;
				return;
			}

			stream << "attr: " << root->attribute << std::endl;
			indent += 4;

			for (auto& each : root->children) {
				_print(stream, each, indent);
			}
		}

	private:
		link_type m_root;
	};
}

namespace std {

	template <typename _Attr, typename _Val>
	std::ostream& operator<<(
		std::ostream& stream, const ml::decision_tree<_Attr, _Val>& tree
	) {
		tree.print(stream);
		return stream;
	}
}

#endif //_DECISION_TREE_H_

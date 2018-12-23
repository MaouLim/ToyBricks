/*
 * Created by Maou Lim on 2018/11/5.
 */

#ifndef _CANDIDATE_ELIMINATION_H_
#define _CANDIDATE_ELIMINATION_H_

#include <vector>
#include <list>
#include <cassert>
#include <functional>
#include <ostream>

namespace ml {

	template <typename _EnumTp>
	using candidate = std::vector<_EnumTp>;

	template <typename _EnumTp, typename _BoolTp = bool>
	using sample = std::pair<candidate<_EnumTp>, _BoolTp>;

	template <typename _EnumTp>
	using meta = std::vector<_EnumTp>;

	template <typename _EnumTp>
	using constraint = std::vector<_EnumTp>;

	template <typename _EnumTp>
	struct meta_info {
		static const _EnumTp nil;
		static const _EnumTp all;
	};

	template <typename _EnumTp>
	const _EnumTp meta_info<_EnumTp>::nil = static_cast<_EnumTp>(-1);

	template <typename _EnumTp>
	const _EnumTp meta_info<_EnumTp>::all = static_cast<_EnumTp>(-2);

	template <
	    typename _Constraint,
	    typename _Candidate
	>
	bool consistent(
		const _Constraint& constraint,
		const _Candidate&  candidate
	) {
		typedef typename _Constraint::value_type val_t1;
		typedef typename _Candidate::value_type  val_t2;
		static_assert(std::is_same<val_t1, val_t2>::value);

		const auto nil = meta_info<val_t1>::nil;
		const auto all = meta_info<val_t1>::all;

		assert(constraint.size() == candidate.size());
		auto size = constraint.size();

		for (size_t i = 0; i < size; ++i) {
			if (
				(nil == constraint[i] && nil          !=  candidate[i]) ||
				(all != constraint[i] && candidate[i] != constraint[i])
			) { return false; }
		}

		return true;
	}

	template <typename _Constraint>
	bool general_than(
		const _Constraint& left,
		const _Constraint& right
	) { return consistent(left, right); }

	template <
		typename _Constraint,
		typename _Candidate,
		typename _Predicate
	>
	void do_if_mismatch(
		_Constraint&      constraint,
		const _Candidate& candidate,
		_Predicate        predicate
	) {
		typedef typename _Constraint::value_type val_t1;
		typedef typename _Candidate::value_type  val_t2;
		static_assert(std::is_same<val_t1, val_t2>::value);

		const auto nil = meta_info<val_t1>::nil;
		const auto all = meta_info<val_t1>::all;

		assert(constraint.size() == candidate.size());
		auto size = constraint.size();

		for (size_t i = 0; i < size; ++i) {
			if (
				(nil == constraint[i] && nil          !=  candidate[i]) ||
				(all != constraint[i] && candidate[i] != constraint[i])
			) { constraint[i] = predicate(constraint[i], candidate[i]); }
		}
	}

	template <typename _EnumTp>
	class version_space {

		typedef constraint<_EnumTp> rule_type;
		typedef sample<_EnumTp>     sample_type;
		typedef meta_info<_EnumTp>  meta_info;
		typedef meta<_EnumTp>       meta_type;
		typedef _EnumTp             value_type;

	public:
		explicit version_space(const meta_type& limit) :
			m_limit(limit) {
			m_special.emplace_back(limit.size(), meta_info::nil);
			m_general.emplace_back(limit.size(), meta_info::all);
		}

		void fit(const sample_type& s) {
			if (s.second) {
				// this is a positive sample
				m_general.remove_if([&s](const rule_type& r) { return !consistent(r, s.first); });

				for (auto& each : m_special) {
					do_if_mismatch(each, s.first, &generalize);
				}

				m_special.remove_if([this](const rule_type& r) {
					for (auto& each : m_general) {
						if (general_than(each, r)) { return false; }
					}
					return true;
				});
			}
			else {
				// this is a negative sample

				m_special.remove_if([&s](const rule_type& r) { return consistent(r, s.first); });

				std::list<rule_type> tmp;
				for (auto& each : m_general) {
					if (consistent(each, s.first)) {
						auto num_of_attrs = m_limit.size();
						for (size_t i = 0; i < num_of_attrs; ++i) {
							for (_EnumTp e = 0; e < m_limit[i]; ++e) {
								if (s.first[i] == e) { continue; }
								rule_type r(each); r[i] = e;
								tmp.emplace_back(std::move(r));
							}
						}
					}
					else { tmp.emplace_back(std::move(each)); }
				}

				m_general.swap(tmp);

				m_general.remove_if([this](const rule_type& r) {
					for (auto & each : m_special) {
						if (general_than(r, each)) { return false; }
					}
					return true;
				});
			}
		}

		template <typename _SampleItr>
		void fit_all(_SampleItr first, _SampleItr last) {
			while (first != last) {
				this->fit(*first);
				++first;
			}
		}

		const std::list<rule_type>& general_rules() const { return m_general; }
		const std::list<rule_type>& special_rules() const { return m_special; }

		void print(std::ostream& stream) {
			stream << "Version Space - Special Rules" << std::endl;
			for (auto& rule : m_special) {
				for (auto& attr : rule) {
					stream << attr << ",";
				}
				stream << std::endl;
			}

			stream << "Version Space - General Rules" << std::endl;
			for (auto& rule : m_general) {
				for (auto& attr : rule) {
					stream << attr << ",";
				}
				stream << std::endl;
			}
		}

	private:
		static value_type generalize(
			const value_type& constraint, const value_type& candidate
		) {
			if (meta_info::nil == constraint) {
				return candidate;
			}
			if (
				meta_info::all == constraint ||
			         candidate != constraint
			) { return meta_info::all; }
			return candidate;
		}


	private:
		const meta_type&     m_limit;
		std::list<rule_type> m_general;
		std::list<rule_type> m_special; /* whether the size of m_special is equal to 1? */
	};
}

namespace std {

	template <typename _EnumTp>
	std::ostream& operator<<(
		std::ostream& stream, const ml::version_space<_EnumTp>& space
	) {
		space.print(stream);
		return stream;
	}
}

#endif //_CANDIDATE_ELIMINATION_H_

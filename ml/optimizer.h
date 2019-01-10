/*
 * Created by Maou Lim on 2018/12/20.
 */

#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_

#include "../common/defines.h"

namespace ml {

	template <
	    typename _Parameter, typename _Sample, typename _Step = double
	>
	class gradient_descent {
	public:
		typedef _Parameter param_type;
		typedef _Sample    sample_type;
		typedef _Step      step_type;

		explicit gradient_descent(step_type alpha) : m_alpha(alpha) { }
		virtual ~gradient_descent() = default;

		template <typename _SampleSpaceIterator>
		param_type& optimize(param_type&         theta,
							_SampleSpaceIterator first,
							_SampleSpaceIterator last ) {
			theta -= m_alpha * _delta(theta, first, last);
			return theta;
		}

		param_type& optimize(param_type& theta, const sample_type& sample) {
			theta -= m_alpha * gradient(theta, sample);
			return theta;
		}

		virtual param_type gradient(const param_type&, const sample_type&) = 0;

		step_type learning_rate() const { return m_alpha; }
		void set_learning_rate(step_type new_alpha) { m_alpha = new_alpha; }

	private:
		template <typename _SampleSpaceIterator>
		param_type _delta(const param_type&    theta,
		                  _SampleSpaceIterator first,
		                  _SampleSpaceIterator last ) {
			param_type result; result *= 0;
			tools::size_t count = 0;

			while (first != last) {
				//result += (theta * (first->first) - first->second) * first->first;
				result += gradient(theta, *first);
				++first; ++count;
			}

			return result / count;
		}

	private:
		step_type m_alpha;
	};
}

#endif //_OPTIMIZER_H_

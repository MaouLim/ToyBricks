/*
 * Created by Maou Lim on 2018/12/20.
 */

#ifndef _TRAINER_H_
#define _TRAINER_H_

#include <algorithm>
#include <cstddef>

namespace ml {

	template <
	    typename _Trainee,
	    typename _Sample,
	    typename _Optimizer,
	    typename _Container,
	    typename _Step
	>
	class trainer {
	public:
		typedef _Trainee   trainee_type;
		typedef _Optimizer optimizer_type;
		typedef _Container container_type;
		typedef _Step      step_type;
		typedef size_t     epoch_type;
		typedef double     error_type;

		typedef typename _Container::value_type sample_type;
		typedef typename _Container::iterator   sample_iterator;

		static_assert(std::is_same<sample_type, _Sample>::value);

		enum training_mode { one_by_one, random, mini_batch };

		template <typename _Condition>
		size_t train(
			_Condition    condition,
			training_mode mode     ,
			step_type     rate     ,
			size_t        batch = 1
		) {
			epoch_type epoch = 0;


			optimizer_type optimizer(rate);

			//while (condition(epoch, error, time)) {
			//	switch (mode) {
			//
			//	}
			//}

			return epoch;
		}

		virtual void optimize(optimizer_type&, trainee_type&, sample_iterator, sample_iterator) { }
		virtual void optimize(optimizer_type&, trainee_type&, const sample_type&) { }

		//virtual error_type diff(const sample_type&, const )

		template <typename _InputItr>
		void feed(_InputItr first, _InputItr last) {
			std::copy(first, last, std::back_insert_iterator<container_type>(m_samples));
		}

		void clear() { m_samples.clear(); }

	private:
		container_type m_samples;
	};
}

#endif //_TRAINER_H_

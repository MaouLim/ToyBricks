/*
 * Created by Maou Lim on 2019/2/26.
 */

#ifndef _TOYBRICKS_TENSOR_H_
#define _TOYBRICKS_TENSOR_H_

#include "../common/defines.h"
#include "../container/sequence.h"

namespace ml {

//	struct shape {
//
//
//	private:
//		tools::sequence<size_t>
//	};

	template <typename _Shape, typename _Value>
	class tensor {

		typedef tensor<_Shape, _Value>  self_type;
		typedef tools::sequence<_Value> buff_type;

	public:
		typedef _Value        value_type;
		typedef _Shape        shape_type;
		typedef shape_type    index_type;
		typedef tools::size_t size_type;
		typedef size_type     dim_type;

		shape_type shape() const { return m_shape; }
		dim_type dim() const { return m_shape.size(); }

		value_type at(const index_type& index) {
			return m_buff[index.hash_val()];
		}

		value_type at(const index_type& index) const {
			return m_buff[index.hash_val()];
		}

	private:
		shape_type m_shape;
		buff_type  m_buff;
	};
}

#endif //TOYBRICKS_TENSOR_H

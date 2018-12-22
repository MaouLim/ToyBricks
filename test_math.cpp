/*
 * Created by Maou Lim on 2018/12/21.
 */

#include <iostream>

#include "math/vector2.h"
#include "math/vector_function.h"
#include "math/vector_operation.h"

int main() {
	tools::vector2f v(1.0f, 2.0f);
	tools::vector2f u(2.0f, 3.0f);

	auto p = tools::dot(v, u) ;

	tools::normalize(v);

	std::cout << v.x << ", " << v.y;

	return 0;
}
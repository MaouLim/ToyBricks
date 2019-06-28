/*
 * Created by Maou Lim on 2018/12/21.
 */

#include <iostream>
#include <array>
#include "math/vector2.h"
#include "math/vector_algo.h"
#include "math/vector3.h"
#include "math/math_io.h"

#include "math/matrix.h"
#include "math/vector_operator.h"
#include "math/matrix_algo.h"
#include "math/vector4.h"

/**
 * @note vector class test case
 *
 * vec2i[0]: 1
 * vec2i[1]: 2
 * vec2i: 1        2
 * vec2i: 11       12
 * vec2i: 9        10
 * vec2i: 27       30
 * vec2f: 0.5      0.1
 * vec2f: 27.5     30.1
 * vec2f: 742.5    903
 * v1: 100 200     300     400     500
 * v1: 10  0       0       0       0
 * v1: 0   0       0       0       0
 */
void test_vector() {
	math::vector2i vec2i  { 1, 2 };

	std::cout << "vec2i[0]: " << vec2i[0] << std::endl;
	std::cout << "vec2i[1]: " << vec2i[1] << std::endl;
	std::cout << "vec2i: " << vec2i << std::endl;

	vec2i += 10;
	std::cout << "vec2i: " << vec2i << std::endl;
	vec2i -= 2;
	std::cout << "vec2i: " << vec2i << std::endl;
	vec2i *= 3;
	std::cout << "vec2i: " << vec2i << std::endl;

	math::vector2f vec2f  { 0.5f, 0.1f };
	std::cout << "vec2f: " << vec2f << std::endl;

	vec2f += vec2i;
	std::cout << "vec2f: " << vec2f << std::endl;
	vec2f *= vec2i;
	std::cout << "vec2f: " << vec2f << std::endl;

	math::vector<int, 5>    v1 = { 1, 2, 3 };
	math::vector<float, 1>  v2 = { 10.f };
	math::vector<double, 7> v3 = { 0.5, 0.4, 0.3, 0.3, 0.3, 0.1, 0.2 };
	math::vector<long, 5>   v4 = { 100, 200, 300, 400, 500 };

	v1 = v4;
	std::cout << "v1: " << v1 << std::endl;
	v1 = v2;
	std::cout << "v1: " << v1 << std::endl;
	v1 = v3;
	std::cout << "v1: " << v1 << std::endl;
}

/**
 * @note matrix class test case
 * mat1: 1 2
 * 3       4
 *
 * mat2: 10        20
 * 30      40
 *
 * mat3: 0 0
 * 0       0
 *
 * mat4: 10        20      1       2
 * 30      40      3       99
 *
 * mat4.column(3): 2       99
 * mat4.row(1): 30 40      3       99
 * mat4(1, 2): 3
 * mat4[0]: 10     20      1       2
 * mat1: 1 2
 * 3       4
 */
void test_matrix() {

	math::vector4d r1  { 10, 20, 1, 2  };
	math::vector4d r2  { 30, 40, 3, 99 };

	math::vector2i c1  { 10, 30 };
	math::vector2i c2  { 20, 40 };
	math::vector2i c3  { 1 , 3  };
	math::vector2i c4  { 2 , 99 };

	math::matrix<int, 2, 2>    mat1 = { 1, 2, 3, 4 };
	math::matrix<float, 2, 2>  mat2 = { 10, 20, 30, 40 };
	math::matrix<double, 2, 2> mat3;
	math::matrix<int, 2, 4>    mat4 = { c1, c2, c3, c4 };

	std::cout << "mat1: " << mat1 << std::endl;
	std::cout << "mat2: " << mat2 << std::endl;
	std::cout << "mat3: " << mat3 << std::endl;
	std::cout << "mat4: " << mat4 << std::endl;

	std::cout << "mat4.column(3): " << mat4.column(3) << std::endl;
	std::cout << "mat4.row(1): " << mat4.row(1) << std::endl;
	std::cout << "mat4(1, 2): " << mat4(1, 2) << std::endl;
	std::cout << "mat4[0]: " << mat4[0] << std::endl;

	mat2 = mat1;
	std::cout << "mat2: " << mat2 << std::endl;

	math::matrix3x2<int> mat5 = { 1, 2, 1, 2, 1, 2 };
	math::matrix3x2<float> mat6;

	mat6 = mat5;
	std::cout << "mat6: " << mat6 << std::endl;
}

/**
 * @note vector algorithm & operators test case.
 *
 * ci: 10
 * arri[5]: 2 4 4 5 6
 * *pi: 4
 * stdarri: 6 5 4 3 2
 * cv: 21
 * cv: 100
 * cv: 0
 * *pf: 1.111
 * vec: 0.01       0.04    1.23432 0.16    0.25
 * mat1: 2 3       4
 * 5       6       7
 *
 * mat2: 1 2       3
 * 4       5       6
 *
 * mat4: 2 6       7
 * 10      10      13
 *
 * mat5: 0 9       12
 * 25      24      42
 *
 * res: 14
 * vec1: 0.447214  0.447214        0.447214        0.447214        0.447214
 * dot(vec1, vec2): 3.1305
 * sum(vec1): 2.23607
 * sum_of_squares(vec1): 1
 * mean(vec2): 1.4
 *
 */
void test_vec_algo_and_op() {
	int ci = 10;                     float cf = 11.5f;
	int arri[5] = { 1, 2, 3, 4, 5 }; float arrf[5] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f };
	int* pi = arri + 1;              float* pf = arrf + 2;

	std::array<int, 5>   stdarri = { 5, 4, 3, 2, 1 };
	std::array<float, 5> stdarrf = { 0.5f, 0.4f, 0.3f, 0.2f, 0.1f };

	math::vector<int, 1> cv = { 20 };
	std::array<float, 1> ca = { 21.9f };

	math::vector<float , 5> vec;

	math::unary_op(ci, tools::increase<int>());
	math::unary_op(ci, tools::decrease<float>());
	math::unary_op(arri, tools::increase<int>());
	math::unary_op(pi, tools::increase<int>());
	math::unary_op(stdarri, tools::increase<int>());
	math::unary_op(cv, tools::increase<int>());

	std::cout << "ci: " << ci << std::endl;
	std::cout << "arri[5]: "
	          << arri[0] << tools::space
	          << arri[1] << tools::space
	          << arri[2] << tools::space
	          << arri[3] << tools::space
	          << arri[4] << tools::space
	          << std::endl;
	std::cout << "*pi: " << *pi << std::endl;
	std::cout << "stdarri: "
	          << stdarri[0] << tools::space
	          << stdarri[1] << tools::space
	          << stdarri[2] << tools::space
	          << stdarri[3] << tools::space
	          << stdarri[4] << tools::space
	          << std::endl;
	std::cout << "cv: " << cv << std::endl;

	math::unary_op(ci, cv, tools::square<int>());
	std::cout << "cv: " << cv << std::endl;

	math::unary_op(pf, cv, tools::square<int>());
	std::cout << "cv: " << cv << std::endl;

	math::unary_op(cv, pf, [](float v) { return 1.111f * (v + 1.f); });
	std::cout << "*pf: " << *pf << std::endl;

	math::unary_op(arrf, vec, tools::square<float>());
	std::cout << "vec: " << vec << std::endl;

	math::matrix<int, 2, 3> mat1 = { 1, 2, 3, 4, 5, 6 };
	math::matrix<int, 2, 3> mat2;
	math::matrix<int, 2, 3> mat3 = { 0, 3, 3, 5, 4, 6 };

	math::unary_op(mat1, [](math::matrix<int, 2, 3>::row_type r) { return r += 1; });
	std::cout << "mat1: " << mat1 << std::endl;

	math::unary_op(mat1, mat2, [](math::matrix<int, 2, 3>::row_type r) { return r -= 1; });
	std::cout << "mat2: " << mat2 << std::endl;

	auto mat4 = math::binary_op(
		mat1, mat3,
		[](const math::matrix<int, 2, 3>::row_type& l,
		   const math::matrix<int, 2, 3>::row_type& r) { return l + r; }
	);
	std::cout << "mat4: " << mat4 << std::endl;

	math::matrix<float, 2, 3> mat5;

	math::binary_op(
		mat1, mat3, mat5,
		[](const math::matrix<int, 2, 3>::row_type& l,
		   const math::matrix<int, 2, 3>::row_type& r) { return l * r; }
	);
	std::cout << "mat5: " << mat5 << std::endl;

	int res = 0;
	math::binary_op(pi, ci, res, tools::plus<int>());
	std::cout << "res: " << res << std::endl;

	math::vector<float , 5> vec1 = { 1, 1, 1, 1, 1 };
	math::vector<float , 5> vec2 = { 1, 2, 1, 2, 1 };

	math::normalize(vec1);
	std::cout << "vec1: " << vec1 << std::endl;

	std::cout << "dot(vec1, vec2): " << math::dot(vec1, vec2) << std::endl;
	std::cout << "sum(vec1): " << math::sum(vec1) << std::endl;
	std::cout << "sum_of_squares(vec1): " << math::sum_of_squares(vec1) << std::endl;
	std::cout << "mean(vec2): " << math::mean(vec2) << std::endl;
}

/**
 * @note matrix algorithm & operators test case.
 *
 * ci: 101
 * cf: 14.5
 * *pf: 14.5
 * arri[3]: 1 4 9
 * vd: 43.56       59.29   77.44
 * mat: 6.6        7.7     8.8
 * 7.6     8.7     9.8
 * 4.356   5.929   7.744
 *
 * mat: 5.6        6.7     7.8
 * 6.6     7.7     8.8
 * 3.356   4.929   6.744
 *
 * arri[3]: 43 59 77
 * vd: 43  59      77
 * *pf: 9999
 * cm: 1999
 *
 * vd: 8   7       6
 * mat3: 13        13      13      13
 * 13      13      13      13
 * 13      13      13      13
 *
 * vres: 0.666667  0.666667
 * cres: 2
 * mat5: 0.0769231 0.153846        0.3     0.444444
 * 0.625   2       1.16667 1.6
 * 2.25    3.33333 5.5     12
 *
 * arrf[3]: 5 6.66667 6
 * mat6: 0
 * 0
 * 0
 *
 * mat6: 100
 * 200
 * 300
 *
 * vecs[0]: 100    200
 * vecs[1]: 300    400
 * vecs[1]: 301    401
 * *p: 1.08        2.08
 * vecs[0]: 100    200
 * vecs[1]: 1.08   2.08
 */
void test_mat_algo_and_op() {

	int ci = 100;              float cf = 15.5;
	int arri[3] = { 1, 2, 3 }; float arrf[3] = { 9.3f, 5.4f, 1.7f };
	int* pi = &ci;             float* pf = &cf;

	math::vector3d vd { 6.6, 7.7, 8.8 };

	math::matrix<double, 3, 3> mat = { vd, vd + math::vector3d(1), vd * vd / 10 };

	math::matrix_unary_op(ci, tools::increase<int>());
	math::matrix_unary_op(pf, tools::decrease<float_t>());
	math::matrix_unary_op(arri, tools::square<int>());
	math::matrix_unary_op(vd, tools::square<double>());

	std::cout << "ci: " << ci << std::endl;
	std::cout << "cf: " << cf << std::endl;
	std::cout << "*pf: " << *pf << std::endl;
	std::cout << "arri[3]: "
	          << arri[0] << tools::space
	          << arri[1] << tools::space
	          << arri[2] << tools::space
	          << std::endl;
	std::cout << "vd: " << vd << std::endl;

	std::cout << "mat: " << mat << std::endl;

	math::matrix_unary_op(mat, tools::decrease<double>());
	std::cout << "mat: " << mat << std::endl;

	math::matrix_unary_op(vd, arri, tools::identity<float>());
	std::cout << "arri[3]: "
	          << arri[0] << tools::space
	          << arri[1] << tools::space
	          << arri[2] << tools::space
	          << std::endl;

	math::matrix_unary_op(arri, vd, tools::identity<float>());
	std::cout << "vd: " << vd << std::endl;

	math::matrix<double, 1, 1> cm = { 9999.0 };
	math::matrix_unary_op(cm, pf, tools::identity<float>());

	std::cout << "*pf: " << *pf << std::endl;

	*pf = 1999;
	math::matrix_unary_op(pf, cm, tools::identity<float>());
	std::cout << "cm: " << cm << std::endl;

	math::matrix<int, 3, 1> vec_like = { 8, 7, 6 };

	math::matrix_unary_op(vec_like, vd, tools::identity<float>());
	std::cout << "vd: " << vd << std::endl;

	math::matrix<int, 3, 4> mat1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	math::matrix<int, 3, 4> mat2 = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	auto mat3 = math::matrix_binary_op(mat1, mat2, tools::plus<int>());
	std::cout << "mat3: " << mat3 << std::endl;

	auto vres = math::matrix_binary_op(math::vector2f(2.f), math::vector2f(3.f), tools::divide<float>());
	std::cout << "vres: " << vres << std::endl;

	auto cres = math::matrix_binary_op(1, 2, tools::multiply<float>());
	std::cout << "cres: " << cres << std::endl;

	math::matrix<float , 3, 4> mat4 = { 13, 13, 10, 9, 8, 3, 6, 5, 4, 3, 2, 1 };
	math::matrix<double , 3, 4> mat5;
	math::matrix_binary_op(mat1, mat4, mat5, tools::divide<float>());

	std::cout << "mat5: " << mat5 << std::endl;

	math::matrix<float, 3, 1> mat6 = { 100, 200, 300 };
	math::vector3d            vec    { 20, 30 ,50 };

	math::matrix_binary_op(mat6, vec, arrf, tools::divide<float>());
	std::cout << "arrf[3]: "
	          << arrf[0] << tools::space
	          << arrf[1] << tools::space
	          << arrf[2] << tools::space
	          << std::endl;


	math::matrix_unary_op(mat6, tools::zero<float>());
	std::cout << "mat6: " << mat6 << std::endl;
	math::matrix_binary_op(arrf, vec, mat6, tools::multiply<float>());
	std::cout << "mat6: " << mat6 << std::endl;

	math::matrix<float, 2, 2> mat7 = { 100, 200, 300, 400 };
	math::vector2f vecs[2];

	math::matrix_unary_op(mat7, vecs, tools::identity<float>());
	std::cout << "vecs[0]: " << vecs[0] << std::endl;
	std::cout << "vecs[1]: " << vecs[1] << std::endl;

	math::matrix_unary_op(vecs, mat7, tools::square<float>());

	math::vector2f* p = vecs + 1;
	math::matrix<float, 2, 1> mat8 = { 1080, 2080 };

	math::matrix_unary_op(p, tools::increase<float>());
	std::cout << "vecs[1]: " << vecs[1] << std::endl;

	math::matrix_unary_op(mat8, p, [](float v) { return v / 1000.f; });
	std::cout << "*p: " << *p << std::endl;
	std::cout << "vecs[0]: " << vecs[0] << std::endl;
	std::cout << "vecs[1]: " << vecs[1] << std::endl;

	auto row_mat = math::as_row(vecs[0]);
	std::cout << "row_mat: " << row_mat << std::endl;

	auto col_mat = math::as_column(vecs[1]);
	std::cout << "col_mat: " << col_mat << std::endl;
}

int main() {

	//test_vector();
	//test_matrix();
	//test_vec_algo_and_op();
	test_mat_algo_and_op();

	return 0;
}

















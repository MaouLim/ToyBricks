#include <iostream>
#include <random>
#include <ctime>

#include <array>

typedef std::array<double, 4> arr_t;

double sigmoid(double x) {
	return 1.0 / (1.0 + std::exp(-x));
}

int main() {

	return 0;
}

//typedef std::array<double, 4> arr_t;
//
//const size_t size = 1000;
//arr_t inputs[size];
//
//double target_func(const arr_t& x) {
//	return 1 * x[0] + 2 * x[1] + 3 * x[2] + 4 * x[3] +
//	       10 * std::sin(x[1] + x[2] + x[3]);
//}
//
//double dot(const arr_t& x, const arr_t& y) {
//	return x[0] * y[0] + x[1] * y[1] +
//	       x[2] * y[2] + x[3] * y[3];
//}
//
//int main() {
//
//	for (size_t i = 0; i < 1000; ++i) {
//		inputs[i][0] = 1;
//		inputs[i][1] = i % 10;
//		inputs[i][2] = (i / 10) % 10;
//		inputs[i][3] = (i / 100) % 10;
//	}
//
//	arr_t theta;
//
//	std::mt19937_64 rand_engine(time(nullptr));
//	std::uniform_real_distribution rand_real(0.1, 0.9);
//	std::uniform_int_distribution  rand_int(0, 1000);
//
//	const size_t batch_size = 100;
//	const double alpha      = 0.02;
//
//	for (auto& each : theta) {
//		each = rand_real(rand_engine);
//	}
//
//	for (size_t epoch = 0; epoch < 20000; ++epoch) {
//		arr_t tmp_theta;
//		size_t index = rand_int(rand_engine);
//
//		for (size_t i = 0; i < 4; ++i) {
//			double delta(0.0);
//
//			for (size_t j = 0; j < batch_size; ++j) {
//				arr_t& x = inputs[(index + j) % size];
//				delta += x[i] * (dot(theta, x) - target_func(x));
//			}
//
//			delta /= batch_size;
//
//			tmp_theta[i] = theta[i] - alpha * delta;
//		}
//
//		memcpy(&theta[0], &tmp_theta[0], 4 * sizeof (double));
//	}
//
//	for (auto each : theta) {
//		std::cout << each << ' ';
//	}
//
//	return 0;
//}
/*
 * Created by Maou Lim on 2018/12/20.
 */

#include <iostream>
#include <random>
#include <ctime>

#include "container/sequence.h"
#include "ml/optimizer.h"
#include "math/vector4.h"
#include "math/vector_function.h"
#include "math/vector_io.h"

inline tools::vector4d operator*(double scalar, const tools::vector4d& v) {
	tools::vector4d result;
	tools::transform(v, result, [scalar](size_t, double val) { return val * scalar; });
	return result;
}

inline tools::vector4d operator/(const tools::vector4d& v, double scalar) {
	tools::vector4d result;
	tools::transform(v, result, [scalar](size_t, double val) { return val / scalar; });
	return result;
}

typedef std::pair<tools::vector4d, double> sample;
typedef tools::sequence<sample>            sample_space;

class gradient_descent_with_rmse :
	public ml::gradient_descent<tools::vector4d, sample, double> {
public:

	explicit gradient_descent_with_rmse(double alpha) : gradient_descent(alpha) { }

	param_type gradient(
		const param_type& theta, const sample_type& sample
	) override {
		return (tools::dot(theta, sample.first) - sample.second) * sample.first;
	}
};

double target_func(const tools::vector4d& x) {
	return 1 * x[0] + 2 * x[1] + 3 * x[2] + 4 * x[3] +
	       10 * std::sin(x[1] + x[2] + x[3]);
}

int main() {
	// constants
	const size_t inputs_size = 1000;
	const size_t max_epoch   = 200;
	const size_t batch_size  = 20;

	// generate inputs
	tools::vector4d inputs[inputs_size];
	for (size_t i = 0; i < inputs_size; ++i) {
		inputs[i][0] = 1;
		inputs[i][1] = i % 10;
		inputs[i][2] = (i / 10) % 10;
		inputs[i][3] = (i / 100) % 10;
	}

	sample_space samples;
	samples.reserve(inputs_size);
	for (size_t i = 0; i < inputs_size; ++i) {
		samples.emplace_back((tools::vector4d) inputs[i], target_func(inputs[i]));
	}

	std::mt19937_64 rand_engine(time(nullptr));
	std::uniform_int_distribution<size_t> rand_int(0, inputs_size - 1);

	// initialize parameters
	tools::vector4d theta = { 0.5, 0.1, 0.2, 0.4 };
	gradient_descent_with_rmse optimizer(0.002);

	sample_space mini_batch; mini_batch.reserve(batch_size);

	// start training
	for (size_t epoch = 0; epoch < max_epoch; ++epoch) {
		// to select mini-batch
		mini_batch.clear();
		for (size_t i = 0; i < batch_size; ++i) {
			mini_batch.emplace_back(samples[rand_int(rand_engine)]);
		}

		optimizer.optimize(theta, mini_batch.begin(), mini_batch.end());
	}

	// output theta
	std::cout << theta;

	return 0;
}
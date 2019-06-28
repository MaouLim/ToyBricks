/*
 * Created by Maou Lim on 2018/12/20.
 */

#include <iostream>
#include <random>
#include <ctime>

#include "../container/sequence.h"
#include "../ml/optimizer.h"
#include "../math/vector4.h"
#include "../math/vector_function.h"
#include "../math/math_io.h"

inline tools::vector4d operator+(const tools::vector4d& left, const tools::vector4d& right) {
	return apply(left, right, tools::plus<double>());
}

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

	explicit gradient_descent_with_rmse(double alpha) :
		gradient_descent(alpha) { }

	param_type gradient(
		const param_type& theta, const sample_type& sample
	) override {
		return (tools::dot(theta, sample.first) - sample.second) * sample.first;
	}
};

inline double target_func(const tools::vector4d& x) {
	return 1 * x[0] + 2 * x[1] + 3 * x[2] + 4 * x[3];
}

inline double func_with_noisy(const tools::vector4d& x) {
	return target_func(x) + 1 * std::sin(x[1] + x[2] + x[3]);
}

static std::mt19937_64 rand_engine(time(nullptr));

int main() {
	/* constants */
	const size_t training_set_sz = 1000;
	const size_t testing_set_sz  = 200;
	const size_t max_epoch       = 100;
	const size_t batch_size      = 20;

	/* generate inputs */
	tools::vector4d inputs[training_set_sz];
	for (size_t i = 0; i < training_set_sz; ++i) {
		inputs[i][0] = 1;
		inputs[i][1] = i % 10;
		inputs[i][2] = (i / 10) % 10;
		inputs[i][3] = (i / 100) % 10;
	}

	sample_space training_set;
	training_set.reserve(training_set_sz);
	for (size_t i = 0; i < training_set_sz; ++i) {
		training_set.emplace_back(
			(tools::vector4d) inputs[i],
			func_with_noisy(inputs[i])
		);
	}

	std::uniform_int_distribution<size_t> rand_int(0, training_set_sz - 1);

	/* initialize parameters */
	tools::vector4d theta = { 0.5, 0.1, 0.2, 0.4 };
	gradient_descent_with_rmse optimizer(2e-3);

	sample_space mini_batch; mini_batch.reserve(batch_size);

	/* start training */
	for (size_t epoch = 0; epoch < max_epoch; ++epoch) {
		mini_batch.clear();
		for (size_t i = 0; i < batch_size; ++i) {
			mini_batch.emplace_back(training_set[rand_int(rand_engine)]);
		}

		optimizer.optimize(theta, mini_batch.begin(), mini_batch.end());
	}

	/* output linear-regression parameter theta */
	std::cout << "Linear-Regression parameter: " << theta << std::endl;

	size_t count_correct = 0;
	double max_error     = 5e-2;

	for (auto& each : training_set) {
		auto output = tools::dot(theta, each.first);
		auto diff = std::abs(output - each.second);
		auto relative_err = diff / output;
		if (relative_err < max_error) { ++count_correct; }
	}

	std::cout << "Accuracy on training set: "
	          << (float) count_correct / training_set_sz
	          << std::endl;

	/* generate testing-set */

	std::uniform_real_distribution<double> rand_real(-100.0, 100.0);

	count_correct = 0;

	for (size_t i = 0; i < testing_set_sz; ++i) {
		auto test_input = tools::vector4d(
			1.0, rand_real(rand_engine), rand_real(rand_engine), rand_real(rand_engine)
		);

		auto output = tools::dot(theta, test_input);
		auto diff = std::abs(output - target_func(test_input));
		auto relative_err = diff / output;
		if (relative_err < max_error) { ++count_correct; }
	}

	std::cout << "Accuracy on testing set: "
	          << (float) count_correct / testing_set_sz
	          << std::endl;
	return 0;
}
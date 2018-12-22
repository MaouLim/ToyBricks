/*
 * Created by Maou Lim on 2018/12/22.
 */

#include <iostream>
#include <utility>
#include <random>
#include <ctime>

#include "container/sequence.h"
#include "ml/optimizer.h"
#include "math/vector3.h"
#include "math/vector_function.h"
#include "math/vector_io.h"

typedef int                               label;
typedef std::pair<tools::vector3d, label> sample;
typedef tools::sequence<sample>           sample_space;

inline tools::vector3d operator*(double scalar, const tools::vector3d& v) {
	tools::vector3d result;
	tools::transform(v, result, [scalar](size_t, double val) { return val * scalar; });
	return result;
}

inline tools::vector3d operator/(const tools::vector3d& v, double scalar) {
	tools::vector3d result;
	tools::transform(v, result, [scalar](size_t, double val) { return val / scalar; });
	return result;
}

class gradient_descent_with_rmse :
	public ml::gradient_descent<tools::vector3d, sample> {
public:

	explicit gradient_descent_with_rmse(double alpha) : gradient_descent(alpha) { }

	param_type gradient(
		const param_type& theta, const sample_type& sample
	) override {
		return (tools::sigmoid(tools::dot(theta, sample.first)) - sample.second) * sample.first;
	}
};

static std::mt19937_64 rand_engine(time(nullptr));

inline label target_classifier(const tools::vector3d& vec) {
	return 0.0 < (-3 * vec[0] + 2 * vec[1] - vec[2]);
}

label classifier_with_noisy(const tools::vector3d& vec) {
	static std::uniform_int_distribution<size_t> rand_int(1, 100);
	static std::uniform_int_distribution<label>  rand_label(0, 1);
	if (50 < rand_int(rand_engine)) {
		return target_classifier(vec);
	}
	else {
		return rand_label(rand_engine);
	}
}

int main() {

	// constants
	const size_t inputs_size = 400;
	const size_t max_epoch   = 100;
	const size_t batch_size  = 20;

	// generate inputs
	tools::vector3d inputs[inputs_size];
	for (size_t i = 0; i < inputs_size; ++i) {
		inputs[i][0] = 1;
		inputs[i][1] = -10 + i % 20;
		inputs[i][2] = -10 + (i / 20) % 20;
	}

	sample_space samples;
	samples.reserve(inputs_size);
	for (size_t i = 0; i < inputs_size; ++i) {
		samples.emplace_back((tools::vector3d) inputs[i], classifier_with_noisy(inputs[i]));
	}

	std::mt19937_64 rand_engine(time(nullptr));
	std::uniform_int_distribution<size_t> rand_int(0, inputs_size - 1);

	// initialize parameters
	tools::vector3d theta = { 0.5, 0.1, 0.2 };
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

	// output logistic-regression classifier parameter theta
	std::cout << "Logistic-Regression classifier parameter: " << theta << std::endl;

	double     max_error = 10e-5;
	size_t count_correct = 0;

	for (auto& each : samples) {
		if (std::abs(
			    tools::sigmoid(tools::dot(theta, each.first)) - each.second
			) < max_error
		) { ++count_correct; }
	}

	std::cout << "Accuracy on training set: " << (float) count_correct / inputs_size << std::endl;

	/* create testing set to test the generalization of the classifier */
	const size_t test_set_size = 100;
	std::uniform_real_distribution<double> rand_real(-100.0, 100.0);

	count_correct = 0;

	for (size_t i = 0; i < test_set_size; ++i) {
		auto test_input = tools::vector3d(
			1.0, rand_real(rand_engine), rand_real(rand_engine)
		);

		if (std::abs(
			    tools::sigmoid(tools::dot(theta, test_input)) - target_classifier(test_input)
			) < max_error
		) { ++count_correct; }
	}

	std::cout << "Accuracy on testing set: " << (float) count_correct / test_set_size << std::endl;

	return 0;
}
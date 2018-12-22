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

inline tools::vector3d operator+(const tools::vector3d& left, const tools::vector3d& right) {
	return apply(left, right, tools::plus<double>());
}

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

	explicit gradient_descent_with_rmse(double alpha, double r) :
		gradient_descent(alpha), regular(r) { }

	param_type gradient(
		const param_type& theta, const sample_type& sample
	) override {
		return (tools::sigmoid(tools::dot(theta, sample.first)) - sample.second) * sample.first + regular * theta;
	}

	double regular;
};

static std::mt19937_64 rand_engine(time(nullptr));

inline label target_classifier(const tools::vector3d& vec) {
	return 0.0 < (-3 * vec[0] + 2 * vec[1] - vec[2]);
}

inline label classifier_with_noisy(const tools::vector3d& vec) {
	static std::uniform_int_distribution<size_t> rand_int(1, 100);
	static std::uniform_int_distribution<label>  rand_label(0, 1);
	if (50 < rand_int(rand_engine)) {
		return target_classifier(vec);
	}
	else {
		return rand_label(rand_engine);
	}
}

inline label model(const tools::vector3d& theta, const tools::vector3d& vec) {
	return 0.5 < tools::sigmoid(tools::dot(theta, vec)) ? 1 : 0;
}

int main() {

	/* constants */
	const size_t training_set_sz = 400;
	const size_t testing_set_sz  = 100;
	const size_t max_epoch       = 300;
	const size_t batch_size      = 20;

	/* generate training-set */
	tools::vector3d inputs[training_set_sz];
	for (size_t i = 0; i < training_set_sz; ++i) {
		inputs[i][0] = +1.0;
		inputs[i][1] = -10.0 + i % 20;
		inputs[i][2] = -10.0 + (i / 20) % 20;
	}

	sample_space training_set;
	training_set.reserve(training_set_sz);
	for (size_t i = 0; i < training_set_sz; ++i) {
		training_set.emplace_back(
			(tools::vector3d) inputs[i],
			classifier_with_noisy(inputs[i])
		);
	}

	std::uniform_int_distribution<size_t> rand_int(0, training_set_sz - 1);

	/* initialize parameters */
	tools::vector3d theta = { 0.5, 0.1, 0.2 };
	gradient_descent_with_rmse optimizer(2e-2, 1e-2);

	sample_space mini_batch; mini_batch.reserve(batch_size);

	/* start training */
	for (size_t epoch = 0; epoch < max_epoch; ++epoch) {
		// to select mini-batch
		mini_batch.clear();
		for (size_t i = 0; i < batch_size; ++i) {
			mini_batch.emplace_back(training_set[rand_int(rand_engine)]);
		}

		optimizer.optimize(theta, mini_batch.begin(), mini_batch.end());
	}

	/* output logistic-regression classifier parameter theta */
	std::cout << "Logistic-Regression classifier parameter: " << theta << std::endl;

	size_t count_correct = 0;

	for (auto& each : training_set) {
		if (each.second == model(theta, each.first)) { ++count_correct; }
	}

	std::cout << "Accuracy on training set: "
	          << (float) count_correct / training_set_sz
	          << std::endl;

	/* generate testing-set to test the generalization of the classifier */

	std::uniform_real_distribution<double> rand_real(-100.0, 100.0);
	count_correct = 0;

	for (size_t i = 0; i < testing_set_sz; ++i) {
		auto test_input = tools::vector3d(
			1.0, rand_real(rand_engine), rand_real(rand_engine)
		);

		if (target_classifier(test_input) == model(theta, test_input)) { ++count_correct; }
	}

	std::cout << "Accuracy on testing set: "
	          << (float) count_correct / testing_set_sz
	          << std::endl;

	return 0;
}
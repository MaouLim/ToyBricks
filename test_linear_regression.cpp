/*
 * Created by Maou Lim on 2018/12/20.
 */

#include <iostream>
#include <random>
#include <ctime>

#include "ml/optimizer.h"
#include "container/sequence.h"

typedef tools::fixed_seq<double, 4> vector4d;
typedef std::pair<vector4d, double> sample;
typedef tools::sequence<sample>     sample_space;

inline vector4d operator+(const vector4d& left, const vector4d& right) {
	vector4d result;

	for (size_t i = 0; i < 4; ++i) {
		result[i] = left[i] + right[i];
	}

	return result;
}

inline vector4d& operator+=(vector4d& left, const vector4d& right) {
	for (size_t i = 0; i < 4; ++i) {
		left[i] += right[i];
	}

	return left;
}

inline vector4d operator-(const vector4d& left, const vector4d& right) {
	vector4d result;

	for (size_t i = 0; i < 4; ++i) {
		result[i] = left[i] - right[i];
	}

	return result;
}

inline vector4d& operator-=(vector4d& left, const vector4d& right) {
	for (size_t i = 0; i < 4; ++i) {
		left[i] -= right[i];
	}

	return left;
}

inline vector4d operator*(double scale, const vector4d& vec) {
	vector4d result;

	for (size_t i = 0; i < 4; ++i) {
		result[i] = scale * vec[i];
	}

	return result;
}

inline vector4d operator*(const vector4d& vec, double scale) {
	return operator*(scale, vec);
}

inline vector4d& operator*=(vector4d& vec, double scale) {
	for (size_t i = 0; i < 4; ++i) {
		vec[i] *= scale;
	}

	return vec;
}

inline vector4d operator/(const vector4d& vec, double scale) {
	vector4d result;

	for (size_t i = 0; i < 4; ++i) {
		result[i] = vec[i] / scale;
	}

	return result;
}

inline vector4d& operator/=(vector4d& vec, double scale) {
	for (size_t i = 0; i < 4; ++i) {
		vec[i] /= scale;
	}

	return vec;
}

inline double dot(const vector4d& left, const vector4d& right) {
	double result(0);

	for (size_t i = 0; i < 4; ++i) {
		result += left[i] * right[i];
	}

	return result;
}

class gradient_descent_with_rmse :
	public ml::gradient_descent<vector4d, sample, double> {
public:

	explicit gradient_descent_with_rmse(double alpha) : gradient_descent(alpha) { }

	param_type gradient(
		const param_type& theta, const sample_type& sample
	) override {
		return (dot(theta, (sample.first)) - sample.second) * sample.first;
	}
};

double target_func(const vector4d& x) {
	return 1 * x[0] + 2 * x[1] + 3 * x[2] + 4 * x[3] +
	       10 * std::sin(x[1] + x[2] + x[3]);
}

int main() {
	// constants
	const size_t inputs_size = 1000;
	const size_t max_epoch   = 200;
	const size_t batch_size  = 20;

	// generate inputs
	vector4d inputs[inputs_size];
	for (size_t i = 0; i < inputs_size; ++i) {
		inputs[i][0] = 1;
		inputs[i][1] = i % 10;
		inputs[i][2] = (i / 10) % 10;
		inputs[i][3] = (i / 100) % 10;
	}

	sample_space samples;
	samples.reserve(inputs_size);
	for (size_t i = 0; i < inputs_size; ++i) {
		samples.emplace_back((vector4d) inputs[i], target_func(inputs[i]));
	}

	std::mt19937_64 rand_engine(time(nullptr));
	std::uniform_int_distribution<size_t> rand_int(0, inputs_size - 1);

	// initialize parameters
	vector4d theta = { 0.5, 0.1, 0.2, 0.4 };
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
	for (auto each : theta) {
		std::cout << each << ' ';
	}

	return 0;
}
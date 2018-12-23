/*
 * Created by Maou Lim on 2018/12/20.
 */

#include <iostream>

#include "ml/candidate_elimination.h"

enum sky         { sunny, rainy };
enum air_temp    { t_warm, t_cold };
enum humidity    { normal, high };
enum wind        { strong };
enum water       { w_warm, w_cool };
enum forecast    { same, change };
enum enjoy_sport { no, yes };

ml::sample<int> s1({ sunny, t_warm, normal, strong, w_warm, same   }, yes);
ml::sample<int> s2({ sunny, t_warm, high  , strong, w_warm, same   }, yes);
ml::sample<int> s3({ rainy, t_cold, high  , strong, w_warm, change }, no );
ml::sample<int> s4({ sunny, t_warm, high  , strong, w_cool, change }, yes);

int main() {

	ml::version_space<int> space(
		ml::meta<int>({ 2, 2, 2, 1, 2, 2 })
	);

	space.fit(s1);
	space.fit(s2);
	space.fit(s3);
	space.fit(s4);

	std::cout << space << std::endl;
	return 0;
}
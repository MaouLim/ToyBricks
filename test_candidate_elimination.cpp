/*
 * Created by Maou Lim on 2018/12/20.
 */

#include <iostream>

#include "ml/candidate_elimination.h"

int main() {

	ml::version_space<int> space(ml::meta<int>({ 2, 2, 2, 1, 2, 2 }));

	ml::sample<int> s1({ 1, 1, 1, 1, 1, 1 }, true );
	ml::sample<int> s2({ 1, 1, 0, 1, 1, 1 }, true );
	ml::sample<int> s3({ 0, 0, 0, 1, 1, 0 }, false);
	ml::sample<int> s4({ 1, 1, 0, 1, 0, 0 }, true );

	space.fit(s1);
	space.fit(s2);
	space.fit(s3);
	space.fit(s4);

	space.print(std::cout);

	return 0;
}
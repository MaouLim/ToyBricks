#include <iostream>

#include "container/matrix.h"
#include "ml/decision_tree.h"

typedef int                       attr_type;
typedef int                       value_type;
typedef tools::matrix<value_type> sample_space;

enum outlook     { sunny, overcast, rain };
enum temperature { hot, mild, cool };
enum humidity    { high, normal };
enum windy       { windy_no, windy_yes };
enum play_or_not { no, yes };

const value_type training_data[] = {
	sunny   , hot , high  , windy_no , no ,
	sunny   , hot , high  , windy_yes, no ,
	overcast, hot , high  , windy_no , yes,
	rain    , mild, high  , windy_no , yes,
	rain    , cool, normal, windy_no , yes,
	rain    , cool, normal, windy_yes, no ,
	overcast, cool, normal, windy_yes, yes,
	sunny   , mild, high  , windy_no , no ,
	sunny   , cool, normal, windy_no , yes,
	rain    , mild, normal, windy_no , yes,
	sunny   , mild, normal, windy_yes, yes,
	overcast, mild, high  , windy_yes, yes,
	overcast, hot , normal, windy_no , yes,
	rain    , mild, high  , windy_yes, no
};

int main() {

	const sample_space training_set(14, 5, training_data, training_data + 14 * 5);

	ml::decision_tree<attr_type, value_type> tree;
	tree.build(training_set);

	std::cout << tree << std::endl;

	return 0;
}





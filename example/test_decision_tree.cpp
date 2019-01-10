#include <iostream>

#include "../container/matrix.h"
#include "../ml/decision_tree.h"

typedef int                       attr_type;
typedef int                       value_type;
typedef tools::matrix<value_type> sample_space;

enum outlook     { sunny, overcast, rain };
enum temperature { hot, mild, cool };
enum humidity    { high, normal };
enum windy       { weak, strong };
enum play_or_not { no, yes };

const value_type training_data[] = {
	sunny   , hot , high  , weak  , no ,
	sunny   , hot , high  , strong, no ,
	overcast, hot , high  , weak  , yes,
	rain    , mild, high  , weak  , yes,
	rain    , cool, normal, weak  , yes,
	rain    , cool, normal, strong, no ,
	overcast, cool, normal, strong, yes,
	sunny   , mild, high  , weak  , no ,
	sunny   , cool, normal, weak  , yes,
	rain    , mild, normal, weak  , yes,
	sunny   , mild, normal, strong, yes,
	overcast, mild, high  , strong, yes,
	overcast, hot , normal, weak  , yes,
	rain    , mild, high  , strong, no
};

int main() {

	const sample_space training_set(14, 5, training_data, training_data + 14 * 5);

	ml::decision_tree<attr_type, value_type> tree;
	tree.build(training_set);

	std::cout << tree << std::endl;

	return 0;
}





#include <iostream>

#include <algorithm>
#include <random>
#include <ctime>

#include "container/type_base.h"

struct A {
	int    a;
	double b;
	char   c;

	A() { std::cout << "construct" << std::endl; }

	A(int a, double b, char c) : a(a), b(b), c(c) {
		std::cout << "construct" << std::endl;
	}

	~A() { std::cout << "destruct" << std::endl; }
};


int main() {

	typedef tools::_or<typename tools::_is_scalar<A>::type,
		typename tools::_is_trivially_destructible<A>::type
	>::type res_type;

	static_assert(std::is_same<res_type, tools::_false_type>::value);


//	std::mt19937 rand_engine((unsigned int) time(nullptr));
//	std::uniform_int_distribution<int> rand_int(-100, 100);
//
//	tools::_rb_tree<int, int, tools::self<int>, tools::less<int>> tree;
//
//	tree.insert_equal(1);
//	tree.insert_equal(2);
//	tree.insert_equal(5);
//	tree.insert_equal(-3);
//	tree.insert_equal(-2);
//
//	tree.insert_unique(0);
//
//	for (auto i = tree.begin(); tree.end() != i; i++) {
//		std::cout << *i << " ";
//	}
//
//	for (auto i = tree.rbegin(); tree.rend() != i; ++i) {
//		std::cout << *i << " ";
//	}

//	tools::_bitree_base<
//		tools::_simple_bitree_node<int>,
//		tools::traversal::postorder
//	> tree;
//
//	assert(tree.begin() == tree.end());
//
//	auto res1 = tree.create_root(1);
//	assert(res1.second);
//
//	auto res2 = tree.insert_left(res1.first, 2);
//	auto res3 = tree.insert_right(res1.first, 3);
//
//	tree.insert_right(res2.first, 4);
//	tree.insert_right(res3.first, 5);
//
//	for (auto iter = tree.begin(); tree.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//	std::cout << std::endl;
//
//	for (auto iter = tree.rbegin(); tree.rend() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//
//	auto i = tree.begin();
//
//	assert(++(--i) == tree.begin());
//
//	tree.erase(res1.first);
//
//	for (auto iter = tree.begin(); tree.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//	std::cout << std::endl;
//
//	for (auto iter = tree.rbegin(); tree.rend() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}

//	tools::_bstree_base<
//		tools::_simple_bitree_node<int>,
//		int, tools::self<int>, tools::less<int, int>
//	> tree;
//
//	assert(tree.empty());
//
//	std::cout << "size: " << tree.size() << std::endl;
//
//	tree.insert_equal(5);
//
//	std::cout << "size: " << tree.size() << std::endl;
//	tree.insert_equal(3);
//
//	std::cout << "size: " << tree.size() << std::endl;
//	tree.insert_equal(0);
//
//	std::cout << "size: " << tree.size() << std::endl;
//	tree.insert_equal(6);
//
//	std::cout << "size: " << tree.size() << std::endl;
//	tree.insert_equal(5);
//
//	for (auto iter = tree.begin(); tree.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//
//	for (auto iter = tree.rbegin(); tree.rend() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//
//	std::cout << "size: " << tree.size() << std::endl;

//	tools::sequence<int> seq;
//
//	std::cout << seq.empty() << std::endl;
//
//	for (int i = 0; i < 10; ++i) {
//		seq.push_back(i);
//	}
//
//	for (auto iter = seq.begin(); seq.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//	std::cout << seq.capacity() << std::endl;
//	seq.reserve(18);
//	std::cout << seq.capacity() << std::endl;
//
//	seq.insert(seq.begin(), 10, 99);
//
//	for (auto iter = seq.begin(); seq.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//
//	std::cout << seq.capacity() << ", " << seq.size() << std::endl;
//
//	std::cout << std::endl << seq.empty() << ", " << seq.size() << ", " << seq.capacity() << std::endl;
//
//	auto iter = seq.end() - 1;
//	seq.erase(iter);
//
//	std::cout << seq.size();
//
//	for (auto iter = seq.begin(); seq.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}

//	tools::unidirectional_list<int> list;
//
//	std::cout << "empty: " << list.empty() << std::endl;
//
//	for (int i = 0; i < 10; ++i) {
//		list.push_front(i);
//		list.push_back(20 - i);
//	}
//
//	while (!list.empty()) {
//		list.pop_front();
//	}
//
//	std::cout << "empty: " << list.empty() << std::endl;
//	std::cout << "size : " << list.size() << std::endl;
//
//	for (auto iter = list.begin(); list.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}

//	tools::stack<A, tools::sequence<A>> stack;
//
//	std::cout << stack.empty() << std::endl;
//
//	stack.emplace(1, 2.0, 'a');
//
//	std::cout << stack.empty() << std::endl;

//	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//
//	tools::sequence<int> seq(arr, arr + 10);
//
//	for (auto iter = seq.rbegin(); seq.rend() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}


//	std::vector<int> vec(arr, arr + 10);
//	std::sort(vec.rbegin(), vec.rend());
//
//	std::vector<int>::const_iterator it = vec.begin();
//	std::vector<int>::iterator other_it = it;

	//for (auto iter = vec.rbegin(); iter != vec.begin(); )

//	tools::priority_queue<int> queue(arr, arr + 10);
//	std::cout << queue.empty() << " " << queue.top() << std::endl;
//
//	queue.push(15);
//	queue.push(18);
//
//	while (!queue.empty()) {
//		std::cout << queue.top() << std::endl;
//		queue.pop();
//	}

	return 0;
}















cmake_minimum_required(VERSION 3.12)
project(ToyBricks)

set(CMAKE_CXX_STANDARD 14)

add_executable(MathModule
        test_math.cpp
        math/vector2.h
        math/math_type.h
        math/vector3.h
        math/math_io.h
        math/vector_operator.h
        common/defines.h
        common/functor.h
        ml/tensor.h
        math/matrix.h
        math/vector.h
        math/vector_algo.h
        math/matrix_algo.h
        math/math_common.h
)

add_executable(ContainerModule
        test_container.cpp
        container/algorithm.h
        container/avl_tree.h
        container/hash_map.h
        container/hash_set.h
        container/hashtable.h
        container/heap.h
        container/iterator.h
        container/matrix.h
        container/memory.h
        container/queue.h
        container/rb_tree.h
        container/sequence.h
        container/unidirectional_list.h
        container/stack.h
        container/tree_base.h
        common/type_base.h
        common/defines.h
        common/functor.h
        container/bidirectional_list.h container/pair.h container/graph.h container/adjacency_list.h)

add_executable(CandidateEliminationAlgorithm example/test_candidate_elimination.cpp ml/candidate_elimination.h math/vector4.h)
add_executable(DecisionTreeAlgorithm example/test_decision_tree.cpp ml/decision_tree.h)
add_executable(LinearRegression example/test_linear_regression.cpp ml/optimizer.h)
add_executable(LogisticRegression example/test_logistic_regression.cpp ml/optimizer.h)
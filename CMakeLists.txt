cmake_minimum_required(VERSION 3.12)
project(ToyBricks)

set(CMAKE_CXX_STANDARD 14)

add_executable(MathModule
        test_math.cpp
        math/vector2.h
        math/math_base.h
        math/vector_traits.h
        math/vector3.h
        math/vector_function.h
        math/vector_io.h
        math/vector_operation.h
        common/defines.h
        common/functor.h
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
        container/single_list.h
        container/stack.h
        container/tree_base.h
        container/type_base.h
        common/defines.h
        common/functor.h
)

add_executable(CandidateEliminationAlgorithm test_candidate_elimination.cpp ml/candidate_elimination.h math/vector4.h)
add_executable(DecisionTreeAlgorithm test_decision_tree.cpp ml/decision_tree.h)
add_executable(LinearRegression test_linear_regression.cpp ml/optimizer.h)
add_executable(LogisticRegression test_logistic_regression.cpp ml/optimizer.h)
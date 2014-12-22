find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../src)

set(files
 	WorldTest.cpp
 	ComponentManagerTest.cpp
 	BitsTest.cpp
 	EntitySystemTest.cpp)

add_executable(unit-tests ${files})
add_dependencies(unit-tests gtest)

target_link_libraries(unit-tests
	arethusa
	${GTEST_LIBRARIES}
	${GTEST_MAIN_LIBRARIES}
	pthread)

add_custom_command(
	TARGET unit-tests POST_BUILD
	COMMAND valgrind --track-origins=yes --tool=memcheck --leak-check=full --num-callers=40 ./unit-tests)

GTEST_ADD_TESTS(unit-tests ${files})
add_test(ArethusaTests unit-tests)

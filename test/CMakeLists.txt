find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../src)

set(files
 	WorldTest.cpp
 	ComponentManagerTest.cpp
 	MapperTest.cpp
 	ManagerTest.cpp
 	BitsTest.cpp
 	EntitySystemTest.cpp
 	EntityTest.cpp)

add_executable(unit-tests ${files})

target_link_libraries(unit-tests
	arethusa
	${GTEST_LIBRARIES}
	${GTEST_MAIN_LIBRARIES}
	pthread)

add_custom_target(valgrind COMMAND valgrind --track-origins=yes --tool=memcheck --leak-check=full --num-callers=40 ./unit-tests)
# add_custom_target(valgrind COMMAND valgrind --track-origins=yes --tool=memcheck --leak-check=full --num-callers=40 ./unit-tests --gtest_filter=Bits.*)

GTEST_ADD_TESTS(unit-tests "" ${files})

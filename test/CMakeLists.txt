set(GTEST "gtest-1.7.0")
set(GTEST_ZIP "${GTEST}.zip")

if(NOT EXISTS "../test/lib/${GTEST}")
	file(DOWNLOAD
		"https://googletest.googlecode.com/files/${GTEST_ZIP}"
		"../test/lib/${GTEST_ZIP}")
	execute_process(COMMAND	unzip ${GTEST_ZIP}
		WORKING_DIRECTORY ../test/lib)
endif()

add_subdirectory(lib/${GTEST})
include_directories(include  ../src lib/${GTEST}/include lib/${GTEST}/src)

enable_testing()

# Add test executable target
add_executable(unit-tests
	WorldTest.cpp
	ComponentManagerTest.cpp
	BitsTest.cpp)

target_link_libraries(
	unit-tests
	arethusa
	gtest
	gtest_main)

add_custom_command(
	TARGET unit-tests POST_BUILD
	COMMAND valgrind --track-origins=yes --tool=memcheck --leak-check=full --num-callers=40 ./unit-tests)

#add_custom_command(
#	TARGET unit-tests POST_BUILD
#	COMMAND unit-tests)

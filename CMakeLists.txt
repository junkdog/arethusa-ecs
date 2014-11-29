cmake_minimum_required(VERSION 2.8.4)

project(arethusa)
include(ExternalProject)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

ExternalProject_Add(googletest
	SVN_REPOSITORY http://googletest.googlecode.com/svn/tags/release-1.7.0/
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			   -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
			   -Dgtest_force_shared_crt=ON
	TIMEOUT 10
	INSTALL_COMMAND ""
	#LOG_DOWNLOAD ON
	#LOG_CONFIGURE ON
	#LOG_BUILD ON
	)

add_subdirectory(src)
# add_subdirectory(test)


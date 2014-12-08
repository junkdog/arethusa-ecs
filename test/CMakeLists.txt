set(GTEST "gtest-1.7.0")
set(GTEST_ZIP "${GTEST}.zip")

# if(NOT EXISTS "../test/lib/${GTEST}")
#	file(DOWNLOAD
#		"https://gtest.googlecode.com/files/${GTEST_ZIP}"
#		"../test/lib/${GTEST_ZIP}")
#	execute_process(COMMAND	unzip ${GTEST_ZIP}
#		WORKING_DIRECTORY ../test/lib)
# endif()
#
#add_subdirectory(lib/${GTEST})

## probably re-enable this
#set(gtest_DIR "${Asteroids_SOURCE_DIR}/external/gtest")
#set(gtest_LIBRARY "${gtest_DIR}/lib/libgtest.a")
#set(gtest_INCLUDE_DIR "${gtest_DIR}/include")

include(ExternalProject)

#ExternalProject_Add(gtest
#	PREFIX ${gtest_DIR}
	# INSTALL_DIR ${gtest_DIR}
	# CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${gtest_DIR}
#	URL http://googletest.googlecode.com/files/${GTEST_ZIP}
#	INSTALL_COMMAND ""
	# LOG_DOWNLOAD ON
	# LOG_CONFIGURE ON
	# LOG_BUILD ON
#)

ExternalProject_Add(googletest
    URL http://googletest.googlecode.com/files/${GTEST_ZIP}
    TIMEOUT 10
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    #CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    #           -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    #           -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    #           -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
#    LOG_DOWNLOAD ON
#    LOG_CONFIGURE ON
#    LOG_BUILD ON
)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
include_directories(${source_dir}/include ../src)



# Library
#ExternalProject_Get_Property(googletest binary_dir)
#set(GTEST_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
#set(GTEST_LIBRARY gtest)

#add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
#set_property(TARGET ${GTEST_LIBRARY}
#             PROPERTY IMPORTED_LOCATION ${GTEST_LIBRARY_PATH} )

#add_dependencies(${GTEST_LIBRARY} gtest)

# include_directories(include  ../src lib/${GTEST}/include lib/${GTEST}/src)
#include_directories(../src)

enable_testing()

# Specify include dir
#ExternalProject_Get_Property(googletest binary_dir)
#ExternalProject_Get_Property(googletest source_dir)
include_directories(${source_dir}/include)

#find_package(GTest)
#include_directories(${GTEST_INCLUDE_DIRS})


# Add test executable target
add_executable(unit-tests
	WorldTest.cpp
	ComponentManagerTest.cpp)
add_dependencies(unit-tests googletest)

# Specify MainTests link libraries
ExternalProject_Get_Property(googletest binary_dir)

#target_link_libraries(unit-tests gtest gtest_main)
#target_link_libraries(unit-tests arethusa ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
#target_link_libraries(unit-tests googletest)
target_link_libraries(unit-tests
    arethusa
    ${binary_dir}/gtest.a)



#target_link_libraries(
#	unit-tests
#	arethusa
#	gtest
#	gtest_main)

# FIXME: re-enable
#add_custom_command(
#	TARGET unit-tests POST_BUILD
#	COMMAND valgrind --track-origins=yes --tool=memcheck --leak-check=full --num-callers=40 ./unit-tests)

#add_custom_command(
#	TARGET unit-tests POST_BUILD
#	COMMAND unit-tests)

set(sources
	bleh.cpp
	Bits.cpp
	Manager.cpp
	ComponentManager.cpp
	EntityManager.cpp
	System.cpp
	SystemManager.cpp
	EditProcessor.cpp
	World.cpp)
add_library(arethusa STATIC ${sources})

install(
	DIRECTORY "src"
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
)

install(
	TARGETS arethusa
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

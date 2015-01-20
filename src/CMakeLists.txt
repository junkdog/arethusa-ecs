set(sources
	Util.cpp
	Entity.cpp
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
	DIRECTORY "."
	DESTINATION "include/arethusa"
	FILES_MATCHING PATTERN "*.h"
)

install(
	TARGETS arethusa
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

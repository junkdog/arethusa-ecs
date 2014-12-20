set(sources
	bleh.cpp
	Bits.cpp
	Manager.cpp
	ComponentManager.cpp
	EntityManager.cpp
	System.cpp
	SystemManager.cpp
	World.cpp)
add_library(arethusa STATIC ${sources})

install(
	TARGETS arethusa
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

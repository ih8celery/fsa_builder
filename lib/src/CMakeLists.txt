set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

add_library (fsa_builder SHARED builder.cpp machine.cpp token.cpp state.cpp)

install (TARGETS fsa_builder DESTINATION lib)

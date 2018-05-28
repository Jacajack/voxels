cmake_minimum_required(VERSION 2.6)
project(lobor)
set (lobor_VERSION_MAJOR 0)
set (lobor_VERSION_MINOR 1)

#Needed libraries
set( PROJECT_LINK_LIBS "GL" "glfw" "GLEW" "png16" )

#I will have to create separate include dir later
include_directories("src" "src/core")

#Sources
file( GLOB SOURCES "src/core/*.cpp" "src/demo.cpp" "src/lobor.cpp" )

#Demo compilation and linking
add_executable(demo ${SOURCES})
target_link_libraries(demo ${PROJECT_LINK_LIBS} )

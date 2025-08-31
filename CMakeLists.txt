cmake_minimum_required(VERSION 3.20)
project(openglproject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.4 REQUIRED)

add_executable(openglproject src/main.cpp)
target_link_libraries(openglproject PRIVATE glfw)

add_custom_target(run
	COMMAND openglproject
	DEPENDS openglproject
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

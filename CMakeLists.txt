cmake_minimum_required(VERSION 3.20)
project(openglproject VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Variables
set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/includes)
set(SOURCES 
	src/main.cpp
	src/glad.c
)

# Include Packages
find_package(glfw3 3.4 REQUIRED)

# Create Executable
add_executable(openglproject ${SOURCES})

# Include Folders
target_include_directories(openglproject PRIVATE ${INCLUDES_DIR})

# Link Libraries
target_link_libraries(openglproject PRIVATE glfw)

# Custom target thing to run thing faster
add_custom_target(run
	COMMAND openglproject
	DEPENDS openglproject
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

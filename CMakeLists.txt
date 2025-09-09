cmake_minimum_required(VERSION 3.20)
project(openglproject VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Variables
set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/includes)
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/headers)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

set(SOURCES 
	sources/glad.c
	sources/main.cpp
	sources/window_functions.cpp
	sources/stb_image_implementation.cpp
)

# Include Packages
find_package(glfw3 3.4 REQUIRED)

# Create Executable
add_executable(openglproject ${SOURCES})

# Include Folders
target_include_directories(openglproject PRIVATE 
	${INCLUDES_DIR}
	${HEADERS_DIR}
	${ASSETS_DIR}
)

find_package(OpenGL REQUIRED)

# Link Libraries
target_link_libraries(openglproject PRIVATE glfw OpenGL::GL)

# Custom target thing to run thing faster
add_custom_target(run
	COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
	COMMAND openglproject
	DEPENDS openglproject
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

cmake_minimum_required(VERSION 3.10)
project(Muscadet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress OpenGL Policy Warning (prefer GLVND when available)
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Include directories
include_directories(third_party)

# Source files
# Gather source files excluding the GLM directory
file(GLOB_RECURSE SOURCES 
    src/*.cpp 
    src/*.hpp
    third_party/*.cpp
)
list(FILTER SOURCES EXCLUDE REGEX "third_party/glm.*")

# Add GLAD source manually
set(GLAD_SOURCE third_party/glad/glad.c)

# Libraries
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add executable
add_executable(Muscadet ${SOURCES} ${GLAD_SOURCE})

# Link libraries
target_link_libraries(Muscadet 
    SDL2::SDL2                # Modern SDL2 target
    OpenGL::GL                # OpenGL target
    glm::glm                  # Modern GLM target
)

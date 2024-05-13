cmake_minimum_required(VERSION 3.27.2)

# Be nice to visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Be able to use fetch content to get libraries
include(FetchContent)

project("OpenGL-Template")

add_executable(${PROJECT_NAME})

# Set the folder where the executable is created
# To be able split executables form other temporary build files
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# Set the c++ version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Prevents compiler-specific extensions to C++ because they might allow code to compile on your machine but not on other people's machine
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# Enable good warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Get source files from the src folder
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

# Set src folder as an include directory to be able to include files from the same folder (put .cpp and .hpp files in the same folder)
target_include_directories(${PROJECT_NAME} PRIVATE "src/")


# Pass the CMAKE_SOURCE_DIR to the code, so that we can make our paths relative to it---
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"
)

# ---Add libraries---

# ---GLFW---
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    # Disable some stuff we don't need
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# ---glad---
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    # Set some options for glad
    set(GLAD_PROFILE "compatibility" CACHE STRING "OpenGL profile")
    set(GLAD_API "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
    set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

## ---glm---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
)

FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

# ---img---
FetchContent_Declare(
    img
    GIT_REPOSITORY https://github.com/CoolLibs/img
    GIT_TAG da961a78b622db4f4b26df690b38ac3054e61806
)

FetchContent_MakeAvailable(img)
target_link_libraries(${PROJECT_NAME} PRIVATE img::img)

# ---simpleText--- (fork from https://github.com/podgorskiy/SimpleText)
# Optional library for rendering text
FetchContent_Declare(
    simpletext
    GIT_REPOSITORY https://github.com/dsmtE/SimpleText/
)

FetchContent_MakeAvailable(simpletext)

# Create an interface library for simpletext
add_library(simpletext INTERFACE)
target_include_directories(simpletext SYSTEM INTERFACE ${simpletext_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE simpletext)
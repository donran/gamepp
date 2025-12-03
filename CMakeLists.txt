cmake_minimum_required(VERSION 3.12)


project(gamepp)

set(TARGET_NAME "${PROJECT_NAME}_bin")

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIB_SOURCES ${PROJECT_SOURCE_DIR}/libs/*.h)
add_executable(
    ${TARGET_NAME}
)


# include_directories(include)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PROJECT_LINK_LIBS -lGL)
else()
    message(FATAL_ERROR "System not supported yet")
    return()
endif()


target_sources(${TARGET_NAME} PUBLIC ${SOURCES} ${LIB_SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE glfw OpenGL::GL)
target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_LINK_LIBS} GLEW::GLEW glm::glm)

cmake_minimum_required(VERSION 3.12)


project(gamepp)

set(TARGET_NAME "${PROJECT_NAME}_bin")
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Boost REQUIRED COMPONENTS
    filesystem
)

set(CMAKE_CXX_STANDARD 23)
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
target_link_libraries(${TARGET_NAME} PRIVATE Boost::filesystem)


# Static files

file(GLOB_RECURSE ASSET_FILES ${PROJECT_SOURCE_DIR}/assets/*)
message(STATUS "Adding assets from ${PROJECT_SOURCE_DIR}/assets/*")
foreach(item ${ASSET_FILES})
    file(RELATIVE_PATH item_relative ${PROJECT_SOURCE_DIR}/assets ${item})
    message(STATUS "\tAdding asset ${item_relative}")
    configure_file(${item} "${CMAKE_CURRENT_BINARY_DIR}/assets/${item_relative}" COPYONLY)

endforeach()

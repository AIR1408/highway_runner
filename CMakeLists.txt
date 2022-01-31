cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(Runner)

file(GLOB MY_SOURCES sources/*.cpp)
add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_property(TARGET ${PROJECT_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set(CMAKE_CXX_FLAGS "-O2")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)

target_precompile_headers(${PROJECT_NAME} PUBLIC include/pch.h)

include_directories(external/glm)

include_directories("${PROJECT_SOURCE_DIR}/include/")

target_link_libraries(${PROJECT_NAME} glad glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
					

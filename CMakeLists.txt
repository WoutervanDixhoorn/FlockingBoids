cmake_minimum_required(VERSION 3.26)
project("boid-sim")

set(CMAKE_CXX_STANDARD 23)

#Add DouterGraphics
if(NOT TARGET douter-graphics)
    add_subdirectory(vendor/DG)
endif()

file(GLOB_RECURSE SIM_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
add_executable(boid-sim ${SIM_SOURCES})

target_link_libraries(boid-sim PRIVATE douter-graphics)

#Custom Targets
add_custom_target(CopyAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:boid-sim>/assets"
)
add_dependencies(boid-sim CopyAssets)
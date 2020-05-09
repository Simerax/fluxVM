cmake_minimum_required(VERSION 3.5)

# Name of your Library if you change this make sure to change ../CMakeLists.txt as well!
project("test_fluxVM")
set(LIB_NAME "fluxVM")
set (CMAKE_C_STANDARD 99)

include_directories("../lib/")

# Check headerfiles are generated at build time - thats why we look inside the build dir
include_directories("../build/ext/check/src/")
include_directories("../build/ext/check/")

link_directories("../build/ext/check/src/")

file(GLOB source_files "*.c")

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC "check")

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)

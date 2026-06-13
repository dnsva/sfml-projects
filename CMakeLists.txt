cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(spiral src/ulam_spiral/main.cpp)
add_executable(triangle src/sierpinski_triangle/main.cpp)
add_executable(conway src/conway/main.cpp)
add_executable(graphs src/graphs/main.cpp)

target_compile_definitions(conway PRIVATE
    FONT_PATH="${CMAKE_SOURCE_DIR}/src/conway/arial.ttf"
)

#target_link_libraries(main PRIVATE sfml-graphics)
#target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(spiral PRIVATE sfml-graphics)
target_compile_features(spiral PRIVATE cxx_std_17)

target_link_libraries(triangle PRIVATE sfml-graphics)
target_compile_features(triangle PRIVATE cxx_std_17)

target_link_libraries(conway PRIVATE sfml-graphics)
target_compile_features(conway PRIVATE cxx_std_17)

target_link_libraries(graphs PRIVATE sfml-graphics)
target_compile_features(graphs PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

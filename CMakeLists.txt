cmake_minimum_required(VERSION 3.16)
project(AudioEditor)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 1.0.${PATCH_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(externals/fltk)
add_subdirectory(externals/libsndfile)
add_subdirectory(externals/opus)

include_directories(externals/fltk)
include_directories(externals/libsndfile/include)
include_directories(externals/opus/include)

add_executable(AudioEditor main.cpp)

target_link_libraries(AudioEditor PRIVATE fltk sndfile opus)

if(WIN32)
    set_target_properties(AudioEditor PROPERTIES OUTPUT_NAME "AudioEditor-${PROJECT_VERSION}-Windows")
endif()

install(TARGETS AudioEditor DESTINATION bin)

target_compile_options(AudioEditor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "dmitryanons@gmail.com")

if(WIN32)
    set(CPACK_GENERATOR ZIP) 
    set(CPACK_SOURCE_GENERATOR ZIP)
elseif(UNIX)
    set(CPACK_GENERATOR DEB) 
    set(CPACK_SOURCE_GENERATOR TGZ)
endif()

include(CPack)

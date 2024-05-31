cmake_minimum_required(VERSION 3.28)
project(dll_detours_hook)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(COMPILE_OPTION
      
)

set(LINK_OPTION
       
)


message("CMAKE_SOURCE_DIR is : ${CMAKE_SOURCE_DIR}")

file(GLOB Main ${CMAKE_SOURCE_DIR}/dll_detours_hook/*.cpp)
file(GLOB MainHeader ${CMAKE_SOURCE_DIR}/dll_detours_hook/*.h)

file(GLOB StaticLibs ${CMAKE_SOURCE_DIR}/libs/*.lib)

include_directories(
        ${CMAKE_SOURCE_DIR}/dll_detours_hook
)


add_library(${PROJECT_NAME} SHARED
        ${Main}
        ${MainHeader}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION})
target_link_options(${PROJECT_NAME} PRIVATE ${LINK_OPTION})

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${StaticLibs}
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
else ()
endif ()

# ����� DEBUG Live ����, ����ִ�� cmake_debugger.sh �ű�;
# �� install ��� dylib link �� app;
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/release)
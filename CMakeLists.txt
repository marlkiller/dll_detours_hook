cmake_minimum_required(VERSION 3.28)
project(dll_detours_hook)

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 为 vs studio 配置设置编译选项, 使用 /Od 关闭优化
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")

message("CMAKE_SOURCE_DIR is : ${CMAKE_SOURCE_DIR}")

# Source files
file(GLOB Main ${CMAKE_SOURCE_DIR}/dll_detours_hook/*.cpp)
file(GLOB MainHeader ${CMAKE_SOURCE_DIR}/dll_detours_hook/*.h)

file(GLOB AppSources ${CMAKE_SOURCE_DIR}/dll_detours_hook/apps/*.cpp)
file(GLOB AppHeaders ${CMAKE_SOURCE_DIR}/dll_detours_hook/apps/*.h)

file(GLOB UtilHeaders ${CMAKE_SOURCE_DIR}/dll_detours_hook/utils/*.h)
file(GLOB UtilSources ${CMAKE_SOURCE_DIR}/dll_detours_hook/utils/*.cpp) 

file(GLOB StaticLibs ${CMAKE_SOURCE_DIR}/libs/*.lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/dll_detours_hook
    ${CMAKE_SOURCE_DIR}/dll_detours_hook/apps
    ${CMAKE_SOURCE_DIR}/dll_detours_hook/utils
)

# Build DLL
add_library(${PROJECT_NAME} SHARED
    ${Main}
    ${AppSources}
    ${UtilSources} 
)

## Build exe
#add_executable (${PROJECT_NAME} 
#        ${Main}
#        ${AppSources}
#        ${UtilSources}
#)

# Add headers to the target to make them visible in IDEs
target_sources(${PROJECT_NAME} PRIVATE
    ${MainHeader}
    ${AppHeaders}
    ${UtilHeaders}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${StaticLibs}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif ()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/release)
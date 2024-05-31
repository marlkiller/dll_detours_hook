cmake_minimum_required(VERSION 3.28)
project(dll_detours_hook)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 为 vs studio 配置设置编译选项, 使用 /Od 关闭优化
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")


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

## 编译为 dll 库
add_library(${PROJECT_NAME} SHARED
        ${Main}
        ${MainHeader}
)

## 编译为 bin 可执行程序
#add_executable (${PROJECT_NAME} 
#        ${Main}
#        ${MainHeader}
#)

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

# 如果想 DEBUG Live 调试, 得先执行 cmake_debugger.sh 脚本;
# 将 install 后的 dylib link 到 app;
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/release)
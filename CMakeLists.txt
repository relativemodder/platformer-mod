cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(platformer-mod)

add_library(platformer-mod SHARED src/main.cpp)


include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:HJfod/cocos-headers#1ec524e")
CPMAddPackage("gh:matcool/gd.h#fcdef01")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")

target_compile_definitions(platformer-mod PRIVATE CC_DLL=)

target_include_directories(platformer-mod PRIVATE
    ${gd.h_SOURCE_DIR}/include ${gd.h_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include
)

target_link_libraries(platformer-mod mat-dash cocos2d minhook)

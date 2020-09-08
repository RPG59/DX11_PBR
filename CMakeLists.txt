cmake_minimum_required(VERSION 3.8)
project(DX11_PBR)
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_WIN32_EXECUTABLE)

file(GLOB project_SRC "PBR/*.h" "PBR/*.cpp" "PBR/vendor/imgui/*.h" "PBR/vendor/imgui/*.cpp")
add_executable(project WIN32 ${project_SRC})

SET(TARGET_H PBR/D3DContext.h)
SET(TARGET_CPP PBR/D3DContext.cpp)
add_definitions(-DUNICODE -D_UNICODE)

include_directories(${PROJECT_SOURCE_DIR}/dep/include)

target_link_libraries(project ${PROJECT_SOURCE_DIR}/dep/src/assimp/debug/assimp-vc142-mtd.lib)
target_link_libraries(project D3D11.lib)
target_link_libraries(project d3dcompiler.lib)
target_link_libraries(project dxgi.lib)



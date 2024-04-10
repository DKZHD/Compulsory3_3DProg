# CMakeList.txt : CMake project for Compulsory3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Compulsory3")

include_directories(includes)

set (SRC
"src/Compulsory3.cpp" "includes/glad.c" "src/Terrain/Terrain.cpp" "src/Mesh/Mesh.cpp" "src/Shader/shader.cpp" "src/Camera/camera.cpp" "src/Mesh/Player.cpp" "src/Mesh/Path.cpp" "src/Core/Core.cpp")

set(HEADERS
"src/Compulsory3.h" "src/Terrain/Terrain.h" "src/Mesh/Mesh.h" "src/Shader/shader.h" "src/Camera/camera.h" "src/Mesh/Player.h" "src/Mesh/Path.h" "src/Core/Core.h")

add_subdirectory(libs/glfw-3.3.9)
add_executable (Compulsory3 ${SRC} ${HEADERS})

target_link_libraries(Compulsory3 glfw)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Compulsory3 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

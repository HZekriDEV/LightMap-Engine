# CMakeList.txt : CMake project for Lightmap Engine, include source and define
# project specific logic here.
#

# The GLFW download in Libraries contains prebuilt Windows libraries rather
# than GLFW's source tree, so expose the appropriate one as a CMake target.
set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw")

if(MSVC)
  if(MSVC_VERSION GREATER_EQUAL 1950)
    set(GLFW_LIBRARY_DIR "lib-vc2026")
  elseif(MSVC_VERSION GREATER_EQUAL 1930)
    set(GLFW_LIBRARY_DIR "lib-vc2022")
  elseif(MSVC_VERSION GREATER_EQUAL 1920)
    set(GLFW_LIBRARY_DIR "lib-vc2019")
  elseif(MSVC_VERSION GREATER_EQUAL 1910)
    set(GLFW_LIBRARY_DIR "lib-vc2017")
  else()
    message(FATAL_ERROR "The bundled GLFW binaries do not support this Visual C++ version")
  endif()

  add_library(glfw STATIC IMPORTED)
  set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_ROOT}/${GLFW_LIBRARY_DIR}/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_ROOT}/include"
  )
elseif(MINGW)
  add_library(glfw STATIC IMPORTED)
  set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_ROOT}/lib-mingw-w64/libglfw3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_ROOT}/include"
  )
else()
  find_package(glfw3 REQUIRED)
endif()

# GLM is header-only for this project; its tests and install rules are not
# needed when it is included as a dependency.
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("Libraries/glm")

add_library(LightmapCore STATIC
  "Core/Window.cpp"
  "Core/Window.h"
)

target_compile_features(LightmapCore PRIVATE cxx_std_20)

target_include_directories(LightmapCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Core"
)

target_compile_definitions(LightmapCore PUBLIC
  GLFW_INCLUDE_VULKAN
)

target_link_libraries(LightmapCore PRIVATE
  PUBLIC  
  Vulkan::Vulkan
  glfw
  glm::glm
)

add_executable(LMEditor
  "LMEditor/Application.cpp"
  "LMEditor/Application.h"
  "LMEditor/main.cpp"
)

target_compile_features(LMEditor PRIVATE cxx_std_20)

target_link_libraries(LMEditor PRIVATE
  LightmapCore
)

# TODO: Add tests and install targets if needed.

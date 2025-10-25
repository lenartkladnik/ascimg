cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    ascimg
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare executable ----

add_executable(ascimg_exe source/main.cpp)
add_executable(ascimg::exe ALIAS ascimg_exe)

set_property(TARGET ascimg_exe PROPERTY OUTPUT_NAME ascimg)

target_compile_features(ascimg_exe PRIVATE cxx_std_20)

find_package(OpenCV REQUIRED)
target_link_libraries(ascimg_exe PRIVATE ${OpenCV_LIBS})

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT ascimg_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of ascimg"
  )
endif()

include(cmake/dev-mode.cmake)

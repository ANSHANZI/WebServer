file(GLOB_RECURSE hwy_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(hwy_shared SHARED ${hwy_sources})
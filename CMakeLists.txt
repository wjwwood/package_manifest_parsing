cmake_minimum_required(VERSION 2.8.3)
project(package_manifest_parsing)

include(cmake/FindLibXML2.cmake)

include_directories(include ${LibXML2_INCLUDE_DIRS})

add_library(pkg src/package_manifest_parsing/pkg.c)
target_link_libraries(pkg ${LibXML2_LIBRARIES})

add_executable(parse src/parse.c)
target_link_libraries(parse pkg)

# - Try to find LibXML2 headers and libraries.
#
# Usage of this module as follows:
#
#     find_package(LibXML2)
#
# Variables defined by this module:
#
#  LibXML2_FOUND              System has LibXML2 libs/headers
#  LibXML2_LIBRARIES          The LibXML2 libraries
#  LibXML2_INCLUDE_DIRS       The location of LibXML2 headers

find_path(LibXML2_ROOT_DIR
    NAMES include/libxml2/libxml/tree.h)

find_library(LibXML2_LIBRARIES
    NAMES xml2
    HINTS ${LibXML2_ROOT_DIR}/lib)

find_path(LibXML2_INCLUDE_DIRS
    NAMES libxml/tree.h
    HINTS ${LibXML2_ROOT_DIR}/include/libxml2)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibXML2 DEFAULT_MSG
    LibXML2_LIBRARIES
    LibXML2_INCLUDE_DIRS)

mark_as_advanced(LibXML2_ROOT_DIR LibXML2_LIBRARIES LibXML2_INCLUDE_DIRS)

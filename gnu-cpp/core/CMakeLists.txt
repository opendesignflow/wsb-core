cmake_minimum_required (VERSION 2.8)
project (WSB_CORE)

## Include common project properties
include("../project.cprops")

## Override temporary the install path
#set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/home/rleys/projects/libraries/")


##########################################
# Libraries needed
##########################################

## Core Libraries
odfi_libs_find(pthread log4cpp rt xml2)

##### Lib Std libraries xmlsec1
odfi_libs_find(uuid)

##### Add include dirs for libraries

# xmlsec1
#include_directories(/usr/include/libxml2)

# libXML2
#include_directories(/usr/include/xmlsec1)
include_directories(/usr/include/libxml2)

##########################################
# Compilation Parameters
##########################################


## Debug
set(CMAKE_BUILD_TYPE Debug)

## Show compiler command lines
set(CMAKE_VERBOSE_MAKEFILE false)



##########################################
## Output directories
##########################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "./build/")

##########################################
# Sources definitions
##########################################

## Config
#configure_file (
#  "${${CMAKE_PROJECT_NAME}_BINARY_DIR}/include/euvni-ws-rmb-core.h"
# )



## Packages definitions ##############
odfi_package_add(
        .
        wsb-core/
        wsb-core/broker
        wsb-core/common
        wsb-core/common/dispatch
        wsb-core/common/dispatch_common
        wsb-core/common/dispatch_pthread
        wsb-core/common/io
        wsb-core/common/xml
        wsb-core/engine
        wsb-core/network
        wsb-core/message

        wsb-libstd/intermediary
        wsb-libstd/connector
        wsb-libstd/connector/tcp
        wsb-libstd/message/soap
        wsb-libstd/xml/
        wsb-libstd/xml/saml2
        wsb-libstd/xml/wsse  )

#### VDOM + LIBXML ##########################

odfi_package_unsplitted_add(

    .

    vxml/
    vxml/vdom
    vxml/vdom/utils
    vxml/vxpath

    vxml-libxml2/
    vxml-libxml2/vdom
    vxml-libxml2/vxpath
    )


##########################################
## WSB Core Library
##########################################

add_library(wsb_core SHARED ${OdfiSources})
target_link_libraries(wsb_core ${OdfiLibs})

## Compiler options
set_property(TARGET wsb_core PROPERTY COMPILE_FLAGS "-std=gnu++0x -Wall -Wl,-z,defs")
set_property(TARGET wsb_core PROPERTY LINK_FLAGS "-std=gnu++0x -Wall -Wl,-z,defs")

## Enable openMP
odfi_enable_openmp(wsb_core)

##########################################
# Documentation Generation
##########################################
INCLUDE(Documentation)
option(BUILD_DOCUMENTATION ON)
option(DOCUMENTATION_HTML_HELP ON)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(
            doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)


##########################################
## Packaging
##########################################

git_get_revision_number()

## Common Informations
#######################

# Name and versions
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_PACKAGE_VERSION_MAJOR ${${CMAKE_PROJECT_NAME}_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${${CMAKE_PROJECT_NAME}_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${${CMAKE_PROJECT_NAME}_VERSION_PATCH})
SET(CPACK_PACKAGE_VENDOR "OSI")

# Architecture
odfi_package_get_architecture()
message(STATUS "Package Architecture: ${ODFI_PACKAGE_ARCH}")

### Installation infos
##############################

## Install binary and libraries
install(TARGETS wsb_core
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)



## Install includes
install(DIRECTORY include/
        DESTINATION include/osi/wsf
        COMPONENT headers
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY src/
        DESTINATION include/osi/wsf
        COMPONENT headers
        FILES_MATCHING PATTERN "*.h")

## Install documentation
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/doc
        DESTINATION share/doc/wsf/wsb-core)

### RPM
##############################
list(APPEND CPACK_GENERATOR RPM)

set(CPACK_RPM_PACKAGE_ARCHITECTURE ${ODFI_PACKAGE_ARCH})
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")

# Deps
set(CPACK_RPM_PACKAGE_REQUIRES
                "libc6 >= 2.15, libxerces-c3.1 >= 3.1, libxqilla6 >= 2.3, liblog4cpp5 >= 1.0-4")

## Deb
##############################

INCLUDE(CPack)

#####################################
## Tests                           ##
#####################################
include(tests/tests_cmake.txt)





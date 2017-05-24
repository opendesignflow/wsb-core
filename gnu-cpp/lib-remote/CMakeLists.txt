cmake_minimum_required (VERSION 2.8)
project (WSB_LIB_REMOTE)

## Include common project properties
include("../project.cprops")

## Override temporary the install path
#set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "./install/")

##########################################
# Libraries needed
##########################################
#odfi_libs_find(pthread xerces-c xqilla sctp log4cpp)

## wsb-core
include_directories(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/include/)
odfi_libs_add_paths(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/build/)

## Search libs
odfi_libs_find(wsb_core log4cpp xerces-c)

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

IF (CMAKE_CROSSCOMPILING)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/")
ELSE (CMAKE_CROSSCOMPILING)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build/")
ENDIF (CMAKE_CROSSCOMPILING)

##########################################
# Sources definitions
##########################################

## Config
#configure_file (
#  "${WSRMB_CORE_BINARY_DIR}/include/euvni-ws-rmb-core.h"
# )



## Packages definitions ##############
odfi_package_add(
                 .
                 wsb-libremote/ 
                 wsb-libremote/intermediaries 
                 wsb-libremote/intermediaries/callup ) 


##########################################
## WSB Library
##########################################
add_library(wsb_lib_remote SHARED ${OdfiSources})
target_link_libraries(wsb_lib_remote ${OdfiLibs})

## Compiler options
set_property(TARGET wsb_lib_remote PROPERTY COMPILE_FLAGS "-std=gnu++0x -Wall")
set_property(TARGET wsb_lib_remote PROPERTY LINK_FLAGS "-std=gnu++0x -Wall -Wl,-z,defs")

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


### Installation infos
##############################

## Install binary and libraries
install(TARGETS wsb_lib_remote
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime)


        
## Install includes   
install(DIRECTORY include/ 
        DESTINATION include/osi/wsf
        COMPONENT headers
        FILES_MATCHING PATTERN "*.h")

## Install documentation   
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/doc
        DESTINATION share/doc/wsf/wsb-libremote)

### RPM
##############################



## Deb
##############################

#####################################
## Tests                           ##
#####################################
INCLUDE(tests/tests_cmake.txt)




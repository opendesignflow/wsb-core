cmake_minimum_required (VERSION 2.8)
project (WSB_LIB_TCL)

## Include common project properties
include("../project.cprops")

##########################################
# Libraries needed
##########################################

## WSB Core ###############################
include_directories(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/include)
odfi_libs_add_paths(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/build/)

## Search libs
odfi_libs_find(tcl wsb_core)


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
#  "${${CMAKE_PROJECT_NAME}_BINARY_DIR}/include/euvni-ws-rmb-core.h"
# )

## Packages definitions ##############
set(PACKAGES . )


## Includes ############################
foreach (package ${PACKAGES})
    include_directories("${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/${package}")
endforeach(package)




## Sources #############################
foreach (package ${PACKAGES})
    MESSAGE(STATUS "Adding source directory: ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/src/${package}")
    aux_source_directory(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/src/${package}/ src)
endforeach(package)


## Library
#####################
add_library(ws-rmb-lib-tcl SHARED ${src})
target_link_libraries(ws-rmb-lib-tcl ${OdfiLibs})

## Compiler options
set_property(TARGET ws-rmb-lib-tcl PROPERTY COMPILE_FLAGS "-std=gnu++0x")

## Test Programs
#######################

## TCL Interpertor
add_executable(test-tcl-interp tests/test-tcl-interp.cpp)
target_link_libraries(test-tcl-interp ws-rmb-lib-tcl)


## Loopback script invoker
add_executable(test-lpb-tcl tests/test-lpb-tcl.cpp)
target_link_libraries(test-lpb-tcl ws-rmb-lib-tcl)
set_property(TARGET test-lpb-tcl PROPERTY COMPILE_FLAGS "-std=gnu++0x")




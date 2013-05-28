cmake_minimum_required (VERSION 2.8)
project (WSB_EXAMPLES)

## Include common project properties
include("../project.cprops")


##########################################
# Libraries needed
##########################################
#odfi_libs_find(pthread xerces-c xqilla sctp log4cpp)

## wsb-core
include_directories(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/include/)
odfi_libs_add_paths(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../core/build/)

## wsb-libremote
include_directories(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../lib-remote/include/)
odfi_libs_add_paths(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/../lib-remote/build/)

## Search libs
odfi_libs_find(wsb_core wsb_lib_remote log4cpp xerces-c)

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
## Example Targets
##########################################


#### WS Adressing and RCPHandler #########
##########################################

add_executable(mstream_wsaddressing_rpchandler src/mstream_wsaddressing_rpchandler.cpp)
set_property(TARGET mstream_wsaddressing_rpchandler PROPERTY COMPILE_FLAGS "-std=gnu++0x -Wall")
target_link_libraries(mstream_wsaddressing_rpchandler ${OdfiLibs})




#
# Copyright (C) 2018-2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(RUNTIME_SRCS_UTILITIES_BASE
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/api_intercept.h
  ${CMAKE_CURRENT_SOURCE_DIR}/perf_profiler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/perf_profiler.h
  ${CMAKE_CURRENT_SOURCE_DIR}/tag_allocator.h
)

get_property(NEO_CORE_UTILITIES GLOBAL PROPERTY NEO_CORE_UTILITIES)
list(APPEND RUNTIME_SRCS_UTILITIES_BASE ${NEO_CORE_UTILITIES})

get_property(NEO_CORE_UTILITIES_WINDOWS GLOBAL PROPERTY NEO_CORE_UTILITIES_WINDOWS)
get_property(NEO_CORE_UTILITIES_LINUX GLOBAL PROPERTY NEO_CORE_UTILITIES_LINUX)

set_property(GLOBAL PROPERTY RUNTIME_SRCS_UTILITIES_BASE ${RUNTIME_SRCS_UTILITIES_BASE})

target_sources(${NEO_STATIC_LIB_NAME} PRIVATE ${RUNTIME_SRCS_UTILITIES_BASE})
if(WIN32)
  target_sources(${NEO_STATIC_LIB_NAME} PRIVATE ${NEO_CORE_UTILITIES_WINDOWS})
else()
  target_sources(${NEO_STATIC_LIB_NAME} PRIVATE ${NEO_CORE_UTILITIES_LINUX})
endif()

# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(dynamix-c
    LANGUAGES C
)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.4.3)

list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

#######################################
# dev mode
set(ICM_DEV_C_STANDARD 11)
include(icm_dev_mode)

if(ICM_DEV_MODE AND MSVC)
    # on MSVC set windows SDK to one that supports stdalign in dev mode
    # without dev mode the lib will check and optionally enable features
    set(CMAKE_SYSTEM_VERSION 10.0.20348.0)
endif()

#######################################
# options

option(DYNAMIX_C_STATIC "DynaMix C: build as a static library" OFF)
option(DYNAMIX_C_BUILD_UNIT_TESTS "DynaMix C: enable testing and build tests" ${ICM_DEV_MODE})
option(DYNAMIX_C_BUILD_EXAMPLES "DynaMix C: build examples" ${ICM_DEV_MODE})
option(DYNAMIX_C_BUILD_SCRATCH "DynaMix C: build scratch project (dev experiments)" ${ICM_DEV_MODE})
option(DYNAMIX_C_BUILD_TUTORIALS "DynaMix C: build tutorials" ${ICM_DEV_MODE})
option(DYNAMIX_C_BUILD_BENCH "DynaMix C: build benchmarks" ${ICM_DEV_MODE})

# Off by default since it's slow
option(DYNAMIX_C_BUILD_COMPILER_BENCH "DynaMix C: build compilation time benchmarks (requires manual code generation step)" OFF)

# Optionally disable this so dynamix doesn't get installed along with other projects which are using it
option(DYNAMIX_C_CONFIGURE_INSTALL "DynaMix C: add install configuration" ON)
option(DYNAMIX_C_CONFIGURE_INSTALL_HEADERS "DynaMix C: add install configuration for headers (for a dev library)" ON)

#######################################
# c++

add_subdirectory(code/dnmx)

if(DYNAMIX_C_BUILD_SCRATCH)
    add_subdirectory(scratch)
endif()

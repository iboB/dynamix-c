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
# code

add_subdirectory(scratch)

# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_add_lib)

icm_add_lib(dynamix-c DYNAMIX_C)
add_library(dynamix::dynamix-c ALIAS dynamix-c)

target_include_directories(dynamix-c INTERFACE ..)

target_sources(dynamix-c PRIVATE
    api.h
    mixin_type_info.h
    mixin_type_info.c
    obj_type_info.h
    obj_type_info.c
    domain.h
    domain.c
 )

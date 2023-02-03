# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_executable(dynamix-c-scratch
    scratch.c
)
target_link_libraries(dynamix-c-scratch dynamix::dynamix-c)

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "mixin_type_info.h"

#include <string.h>

void dnmx_default_mixin_ctor(const dnmx_mixin_type_info* info, void* ptr) {
    memset(ptr, 0, info->size);
}

void dnmx_default_mixin_copy(const dnmx_mixin_type_info* info, void* ptr, const void* src) {
    memcpy(ptr, src, info->size);
}

void dnmx_default_mixin_move(const dnmx_mixin_type_info* info, void* ptr, void* src) {
    dnmx_default_mixin_copy(info, ptr, src);
}

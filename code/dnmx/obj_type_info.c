// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "obj_type_info.h"
#include "mixin_type_info.h"

bool dnmx_obj_type_info_has_mixin_by_name(const dnmx_obj_type_info* info, const char* name) {
    for (uint32_t i = 0; i < info->num_mixins; ++i) {
        if (strcmp(name, info->mixins[i]->name) == 0) return true;
    }
    return false;
}

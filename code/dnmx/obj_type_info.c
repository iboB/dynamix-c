// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "obj_type_info.h"
#include "mixin_type_info.h"

#include <string.h>

bool dnmx_obj_type_info_has_mixin_by_name(const dnmx_obj_type_info* info, dnmx_sv name) {
    for (uint32_t i = 0; i < info->num_mixins; ++i) {
        if (dnmx_sv_eq(name, info->mixins[i]->name)) return true;
    }
    return false;
}

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "obj_type.h"
#include "mixin_type_info.h"

#include <string.h>
#include <assert.h>

bool dnmx_obj_type_has_mixin_by_name(const dnmx_obj_type* type, dnmx_sv name) {
    assert(type);
    for (uint32_t i = 0; i < type->num_mixins; ++i) {
        if (dnmx_sv_eq(name, type->mixins[i]->name)) return true;
    }
    return false;
}

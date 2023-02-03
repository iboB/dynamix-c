// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "obj.h"
#include "obj_type.h"

static const dnmx_obj_type EMPTY_OBJ_TYPE;
const dnmx_obj_type* const DNMX_NULL_OBJ_TYPE = &EMPTY_OBJ_TYPE;

// used by objects with no mixin data, so they would return nullptr on get_mixin
// without the need to check or crash
static const dnmx_obj_mixin_data EMPTY_MIXIN_DATA;

const dnmx_obj DNMX_OBJ_INIT = {.type = &EMPTY_OBJ_TYPE, .mixin_data = &EMPTY_MIXIN_DATA};

void dnmx_obj_init(dnmx_obj* obj) {
    obj->type = &EMPTY_OBJ_TYPE;
    obj->mixin_data = &EMPTY_MIXIN_DATA;
}

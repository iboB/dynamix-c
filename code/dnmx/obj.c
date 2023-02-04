// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "obj.h"
#include "obj_type.h"
#include "mixin_type_info.h"
#include "alloc_util.h"

#include <stdlib.h>

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

// allocate and prepare mixin data for object with a new type
// in case obj is not empty:
// * if a mixin is in both types, it will copy pointers to mixins buffers the returned mixin data
// * if a mixin is in new_type only, it will allocate (BUT NOT INIT) buffers for it the returned
static dnmx_obj_mixin_data* uninitialized_prepare_data(const dnmx_obj* obj, const dnmx_obj_type* new_type) {
    const dnmx_obj_type* old_type = obj->type;
    const dnmx_obj_mixin_data* old_mixin_data = obj->mixin_data;

    dnmx_obj_mixin_data* new_mixin_data = (dnmx_obj_mixin_data*)calloc(new_type->num_mixins + DNMX_MIXIN_INDEX_OFFSET, sizeof(dnmx_obj_mixin_data));
    if (!new_mixin_data) return NULL; // out of memory

    for (uint32_t i = 0; i < new_type->num_mixins; ++i) {
        const dnmx_mixin_type_info* new_info = old_type->mixins[i];
        dnmx_mixin_id id = new_info->id;
        uint32_t new_index = new_type->sparse_mixin_indices[id];
        if (dnmx_obj_type_has_mixin(old_type, id)) {
            // mixin is in both types
            uint32_t old_index = old_type->sparse_mixin_indices[id];
            new_mixin_data[new_index] = old_mixin_data[old_index]; // redirect
        }
        else {
            // mixin is in new type only, so allocate buffer
        }
    }

    return new_mixin_data;
}

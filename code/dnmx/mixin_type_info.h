// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "mixin_id.h"

#include <stdint.h>
#include <stddef.h>

typedef struct dnmx_mixin_type_info dnmx_mixin_type_info;

typedef void(*dnmx_mixin_ctor_func)(const dnmx_mixin_type_info* info, void* ptr);
typedef void(*dnmx_mixin_copy_func)(const dnmx_mixin_type_info* info, void* ptr, const void* src);
typedef void(*dnmx_mixin_move_func)(const dnmx_mixin_type_info* info, void* ptr, void* src);
typedef void(*dnmx_mixin_dtor_func)(const dnmx_mixin_type_info* info, void* ptr);

struct dnmx_mixin_type_info {
    /// The mixin's id
    dnmx_mixin_id id;

    const char* name;

    size_t size;
    size_t alignment;

    // dnmx_mixin_allocator* allocator;

    // any of these can be null
    dnmx_mixin_ctor_func ctor;
    dnmx_mixin_dtor_func dtor;
    dnmx_mixin_copy_func copy_ctor;
    dnmx_mixin_copy_func copy_asgn;
    dnmx_mixin_move_func move_ctor;
    dnmx_mixin_move_func move_asgn;

    // dnmx_message_for_mixin* message_infos;

    uintptr_t user_data;

    // dnmx_metric num_mixins;
};

DYNAMIX_C_API void dnmx_default_mixin_ctor(const dnmx_mixin_type_info* info, void* ptr);

// no need for default dtor. it would be empty anyway and the lib checks for that

DYNAMIX_C_API void dnmx_default_mixin_copy(const dnmx_mixin_type_info* info, void* ptr, const void* src);
DYNAMIX_C_API void dnmx_default_mixin_move(const dnmx_mixin_type_info* info, void* ptr, void* src);

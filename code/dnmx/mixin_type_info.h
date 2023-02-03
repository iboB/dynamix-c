// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "mixin_id.h"

#include <stdint.h>
#include <stddef.h>

typedef void(*dnmx_mixin_constructor_func)(void* ptr);
typedef void(*dnmx_mixin_copy_func)(void* ptr, const void* src);
typedef void(*dnmx_mixin_move_func)(void* ptr, void* src);
typedef void(*dnmx_mixin_destructor_func)(void* ptr);

typedef struct dnmx_mixin_type_info {
    /// The mixin's id
    dnmx_mixin_id id;

    const char* name;

    size_t size;
    size_t alignment;

    // dnmx_mixin_allocator* allocator;

    dnmx_mixin_constructor_func default_constructor;
    dnmx_mixin_destructor_func destructor;
    dnmx_mixin_copy_func copy_constructor;
    dnmx_mixin_copy_func copy_assignment;
    dnmx_mixin_move_func move_constructor;
    dnmx_mixin_move_func move_assignment;

    // dnmx_message_for_mixin* message_infos;

    uintptr_t user_data;

    // dnmx_metric num_mixins;
} dnmx_mixin_type_info;

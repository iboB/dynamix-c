// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "mixin_id.h"

#include "bits/sv.h"

#include <stdint.h>
#include <stddef.h>

typedef struct dnmx_mixin_type_info dnmx_mixin_type_info;

typedef void(*dnmx_mixin_init_func)(const dnmx_mixin_type_info* info, void* ptr);
typedef void(*dnmx_mixin_copy_func)(const dnmx_mixin_type_info* info, void* ptr, const void* src);
typedef void(*dnmx_mixin_move_func)(const dnmx_mixin_type_info* info, void* ptr, void* src);
typedef void(*dnmx_mixin_destroy_func)(const dnmx_mixin_type_info* info, void* ptr);

// contains the information for a mixin type
// register with a domain
// will be used by address
// the user is responsible to preserve the lifetime of a mixin type info until it's no longer needed
// (program ends, or type info gets unregistered)
struct dnmx_mixin_type_info {
    // the mixin id
    // must be initialized to 0 (DNMX_INVALID_MIXIN_ID) on unregistered infos
    // will be set by the domain once registered
    // having a non-zero id implies the type info is registered with some domain
    // to register a type in multiple domains, you need make copies of the info per domain
    dnmx_mixin_id id;

    // must be set, cannot be empty
    // must be unique in a domain
    dnmx_sv name;

    // memory allocated for mixin and alignment of that memory
    // can both be zero
    // if size is not zero, alignment must be a power of 2
    // if size is zero, alignment is ignored
    size_t size;
    size_t alignment;

    // dnmx_mixin_allocator* allocator;

    // any of these can be null
    dnmx_mixin_init_func init;
    dnmx_mixin_destroy_func destroy;
    dnmx_mixin_copy_func copy_init;
    dnmx_mixin_copy_func copy_asgn;
    dnmx_mixin_move_func move_init;
    dnmx_mixin_move_func move_asgn;

    // dnmx_message_for_mixin* message_infos;

    // optional user data which is accessible where the info is provided
    // the library does not touch this value
    uintptr_t user_data;

    // dnmx_metric num_mixins;
};

DYNAMIX_C_API void dnmx_default_mixin_init(const dnmx_mixin_type_info* info, void* ptr);

// no need for default destroy. it would be empty anyway and the lib checks for that

DYNAMIX_C_API void dnmx_default_mixin_copy(const dnmx_mixin_type_info* info, void* ptr, const void* src);
DYNAMIX_C_API void dnmx_default_mixin_move(const dnmx_mixin_type_info* info, void* ptr, void* src);

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct dnmx_mixin_type_info dnmx_mixin_type_info;
typedef struct dnmx_obj_type_info dnmx_obj_type_info;

typedef struct dnmx_domain {
    const dnmx_mixin_type_info** sparse_mixins; // sparse array
    uint32_t num_sparse_mixins;

    dnmx_obj_type_info** obj_types; // non-sparse array
    uint32_t num_obj_types;

    // maybe use this if benchmarks suggest it:
    // uint32_t cap_obj_types;
} dnmx_domain;

DYNAMIX_C_API bool dnmx_domain_register_mixin(dnmx_domain* d, dnmx_mixin_type_info* info);
DYNAMIX_C_API bool dnmx_domain_unregister_mixin(dnmx_domain* d, const dnmx_mixin_type_info* info);

// will create the type info if it doesn't exist
// the provided list of infos must be sorted by id
// the requested type must have at least one mixin
DYNAMIX_C_API const dnmx_obj_type_info* dnmx_domain_get_obj_type_info(dnmx_domain* d, const dnmx_mixin_type_info** mixins, uint32_t num_mixins);

// frees the domain data for a domain
// any calls related to the domain after this call will lead to undefined behavior
// (likely a crash or memory corruption)
DYNAMIX_C_API void dnmx_domain_clear(dnmx_domain* d);



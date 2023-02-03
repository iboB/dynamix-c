// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "bits/sv.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct dnmx_mixin_type_info dnmx_mixin_type_info;
typedef struct dnmx_obj_type dnmx_obj_type;

typedef struct dnmx_domain {
    // purely optional
    // useful for debugging
    dnmx_sv name;

    // sparse array
    const dnmx_mixin_type_info** sparse_mixins;
    uint32_t num_sparse_mixins;

    // non-sparse array
    // TODO: benchmark with a hash table here
    dnmx_obj_type** obj_types;
    uint32_t num_obj_types;

    // maybe use this if benchmarks suggest it:
    // uint32_t cap_obj_types;
} dnmx_domain;

DYNAMIX_C_API bool dnmx_domain_register_mixin(dnmx_domain* d, dnmx_mixin_type_info* info);
DYNAMIX_C_API bool dnmx_domain_unregister_mixin(dnmx_domain* d, dnmx_mixin_type_info* info);

// will return an existing type or create a new one if such doesn't exist
// the requested type must have at least one mixin
// the provided list of infos must be sorted by id
// the mixins must be previously registered with the domain
// the returned value will be invalidated if a pariticipating mixin gets unregistered
DYNAMIX_C_API const dnmx_obj_type* dnmx_domain_get_obj_type_info(dnmx_domain* d, const dnmx_mixin_type_info* const* mixins, uint32_t num_mixins);

// frees the domain data for a domain
// any calls related to the domain after this call will lead to undefined behavior
// (likely a crash or memory corruption)
// will not clear name!
DYNAMIX_C_API void dnmx_domain_clear(dnmx_domain* d);

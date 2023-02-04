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

// a domain is used to register mixins and features (like messages)
// it manages object types
// a non-empty object is always "of a domain"
// mixins and features are not usable across domains
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

// register mixin info
// will use the type info by address
// the user is responsible to preserve the lifetime of the mixin type info
DYNAMIX_C_API bool dnmx_domain_register_mixin(dnmx_domain* d, dnmx_mixin_type_info* info);

// unregister type info
// will also remove all object types which use this mixin
// the user is responsible to make sure that there are no living objects with such a mixin
// if such remain, using them in any way (even to destroy them) is ub (likely crash)
DYNAMIX_C_API bool dnmx_domain_unregister_mixin(dnmx_domain* d, dnmx_mixin_type_info* info);

// will return an existing type or create a new one if such doesn't exist
// the requested type must have at least one mixin
// the provided list of infos must be sorted by id
// the mixins must be previously registered with the domain
// the returned value will be invalidated if a pariticipating mixin gets unregistered
DYNAMIX_C_API const dnmx_obj_type* dnmx_domain_get_obj_type(dnmx_domain* d, const dnmx_mixin_type_info* const* mixins, uint32_t num_mixins);

// clears and destroys the domain data for a domain
// any calls related to the domain after this call will lead to undefined behavior
// (likely a crash or memory corruption)
// will not clear name!
DYNAMIX_C_API void dnmx_domain_clear(dnmx_domain* d);
#define dnmx_domain_destroy dnmx_domain_clear

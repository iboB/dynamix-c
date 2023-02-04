// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "mixin_id.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct dnmx_obj_type dnmx_obj_type;
typedef struct dnmx_obj dnmx_obj;

// mixin data in an object
typedef struct dnmx_obj_mixin_data {
    uint8_t* buffer; // beginning of buffer
    uint8_t* mixin; // pointer to mixin within buffer
} dnmx_obj_mixin_data;

// virtual mixin for default message implementation
// used only so as to not have a null pointer cast to the appropriate type for default implementations
// which could be treated as an error in some debuggers
typedef struct dnmx_obj_virtual_mixin_data {
    dnmx_obj* obj;
    struct { void* unused; } mixin;
} dnmx_obj_virtual_mixin_data;

struct dnmx_obj {
    const dnmx_obj_type* type;

    // each element of this array points to a buffer which cointains:
    // a pointer to the object at the front
    // the rest of the bytes are for the mixin itself
    // thus each mixin can get its own object
    const dnmx_obj_mixin_data* mixin_data;

    dnmx_obj_virtual_mixin_data virtual_mixin_data;
};

// type of all empty objects
// this pointer is not null
extern DYNAMIX_C_API const dnmx_obj_type* const DNMX_NULL_OBJ_TYPE;

/////////////////////////////////////////////
// initializers:
// always use one of these to initialize an object
// all of these assume the object is uninitialized

extern DYNAMIX_C_API const dnmx_obj DNMX_OBJ_INIT;
DYNAMIX_C_API void dnmx_obj_init(dnmx_obj* obj);

// may return false if copy is not possible
DYNAMIX_C_API bool dnmx_obj_init_copy(dnmx_obj* tgt, const dnmx_obj* src);

// will clear src
DYNAMIX_C_API void dnmx_obj_init_move(dnmx_obj* tgt, dnmx_obj* src);

/////////////////////////////////////////////
// asignments
// they assume a valid object (never uninitialized)

// changes the type of the target to the source type:
// - will call assignment funcs for mixins that exist in both.
// - will init_copy funcs for mixins which are new to target.
// - will destroy target mixins that don't exist in source.
// may return false if copy is not possible
DYNAMIX_C_API bool dnmx_obj_asgn_copy(dnmx_obj* tgt, const dnmx_obj* src);

// assigns of mixins that exist in both objects.
// does not change the type of the target.
// will call assignment operators for mixins that exist in both objects.
// may return false if copy is not possible
DYNAMIX_C_API bool dnmx_obj_asgn_copy_matching(dnmx_obj* tgt, const dnmx_obj* src);

// will clear src
DYNAMIX_C_API void dnmx_obj_asgn_move(dnmx_obj* tgt, dnmx_obj* src);

// move-assign of mixins that exist in both objects.
// will call move-assignment funcs of the mixins.
// Will not change the type of the target or the source,
// but will leave matching mixins in the source in a moved-out-from state.
// may return false if copy is not possible (missing move-asgn funcs)
DYNAMIX_C_API bool dnmx_obj_asgn_move_matching(dnmx_obj* tgt, dnmx_obj* src);

/////////////////////////////////////////////
//

// checks whether all of the object's mixins have copy-init and assig funcs.
// returns false if either is missing from at least one of its mixins
// (note that there might be cases where _copy or _copy_matching won't throw
// even though this function returns false).
DYNAMIX_C_API void dnmx_obj_is_copyable(const dnmx_obj* tgt);


/////////////////////////////////////////////
// desryction

// frees and clears all data in an object
// leaves it in an empty state
DYNAMIX_C_API void dnmx_obj_clear(dnmx_obj* obj);

// destroys object
// leaves it in an uninitialized state
// must be called for every object which has been initialized
DYNAMIX_C_API void dnmx_obj_destroy(dnmx_obj* obj);

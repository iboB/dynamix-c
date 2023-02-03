// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "mixin_id.h"
#include "message.h"

#include "bits/sv.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct dnmx_mixin_type_info dnmx_mixin_type_info;
typedef struct dnmx_domain dnmx_domain;
typedef struct dnmx_msg_for_mixin dnmx_msg_for_mixin;

// message data for the vtable which consists of tighly packed elements
// for faster acciess
typedef struct dnmx_obj_type_vtable_msg {
    uint32_t mixin_index; // index of mixin within the _compact_mixins vector
    dnmx_func_ptr caller; // type erased pointer to caller function
    const dnmx_msg_for_mixin* msg;
} dnmx_obj_type_vtable_msg;

typedef struct dnmx_obj_type_vtable_entry {
    // used when building the buffer to hold the top-bid message for the top priority
    // also used in the unicast message macros for optimization - to call the top-bid
    // message without the indrection from dereferencing begin
    // also for multicasts which fall back to a default msg implementation this is used to
    // hold the pointer to the default implementation
    dnmx_obj_type_vtable_msg top_bid_msg;

    // a dynamically allocated array of all message datas
    // for unicasts it will hold pointers to all top-prirority messages for each bid
    // or be nullptr if there are no bids except a single one. It's used for DYNAMIX_CALL_NEXT_BIDDER
    // for multicasts it will hold groups of message datas sorted by priority sorted by bid
    // thus calling DYNAMIX_CALL_NEXT_BIDDER will result in a search in this array
    // (being progressively slower for the deph of bidders we use)
    // WARNING: for multicasts end points to the top-bid end only
    // when multiple bids are involved the buffer will continue after end until a nullptr address is pointed
    // also for multicasts it will be even slower depending on how many messages with the same bid exist
    // we pay this price to achieve the maximum performance for the straight-forward simple message call case
    dnmx_obj_type_vtable_msg* begin;
    dnmx_obj_type_vtable_msg* end;
} dnmx_obj_type_vtable_entry;

// special indices in an object's mixin_data member
enum dnmx_obj_type_reserved_mixin_indices {
    // index 0 is reserved for a null mixin data. It's used to return nullptr on queries for non member mixins
    // (without having to check with an if or worse yet - a loop)
    DNMX_NULL_MIXIN_DATA_INDEX,

    // index 1 is reserved for a virtual mixin. It's used to be cast to the default message implementators
    DNMX_DEFAULT_MSG_IMPL_INDEX,

    // offset of the mixin indices in the object's mixin_data member
    DNMX_MIXIN_INDEX_OFFSET
};

typedef struct dnmx_obj_type {
    const dnmx_domain* domain; // domain of this type info

    dnmx_obj_type_vtable_entry* vtable;
    uint32_t vtable_size;

    // a buffer for all dynamically additional message pointers used in the vtable
    dnmx_obj_type_vtable_msg* message_data_buffer;

    // compact array of mixins infos of this type, sorted by id, no null items
    const dnmx_mixin_type_info** mixins;
    uint32_t num_mixins;

    // indices of mixins in the obj's mixin_data per mixin_id
    // if an index is zero, it is not a part of this type
    uint32_t* sparse_mixin_indices;
    uint32_t num_sparse_mixin_indices;

    // single buffer which is used to store all data for the obj type info
    // all pointers from above except domain point into it
    void* buf;
} dnmx_obj_type;

static inline bool dnmx_obj_type_has_mixin(const dnmx_obj_type* type, dnmx_mixin_id id) {
    if (id >= type->num_sparse_mixin_indices) return false;
    return type->sparse_mixin_indices[id] != 0;
}

DYNAMIX_C_API bool dnmx_obj_type_has_mixin_by_name(const dnmx_obj_type* type, dnmx_sv name);

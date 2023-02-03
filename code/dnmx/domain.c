// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "domain.h"
#include "mixin_type_info.h"
#include "obj_type_info.h"

#include "bits/preprocessor.h"

#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <stdalign.h>

#define S_V_P sizeof(void*)

bool dnmx_domain_register_mixin(dnmx_domain* d, dnmx_mixin_type_info* info) {
    if (!info) return false; // info is nullptr
    if (info->id != DNMX_INVALID_MIXIN_ID) return false; // info has id
    if (!info->name) return false; // unnamed info
    if (!info->size) return false; // zero size, malformed type
    if (!info->alignment) return false; // zero alignment, malformed type

    if (!d->sparse_mixins) {
        d->num_sparse_mixins = 4;
        d->sparse_mixins = (dnmx_mixin_type_info**)calloc(d->num_sparse_mixins, S_V_P);
        if (!d->sparse_mixins) return false; // out of memory
    }

    assert(d->sparse_mixins > 0);
    assert(DNMX_INVALID_MIXIN_ID == 0);

    dnmx_mixin_id free_id = DNMX_INVALID_MIXIN_ID;
    for (uint32_t i = d->num_sparse_mixins - 1; i > DNMX_INVALID_MIXIN_ID; --i) {
        const dnmx_mixin_type_info* sparse = d->sparse_mixins[i];
        if (sparse) {
            assert(sparse->id == i);
            if (strcmp(info->name, sparse->name) == 0) return false; // duplicate name
        }
        else {
            free_id = i;
        }
    }

    if (free_id == DNMX_INVALID_MIXIN_ID) {
        // no free slot has been found dugin the iteration, so add more

        free_id = d->num_sparse_mixins;

        d->num_sparse_mixins *= 3;
        d->num_sparse_mixins += 1;
        d->num_sparse_mixins /= 2;

        void* new_buf = realloc(d->sparse_mixins, S_V_P * d->num_sparse_mixins);
        if (!new_buf) {
            // out of memory
            // restore size and return error
            d->num_sparse_mixins = free_id;
            return false;
        }
        d->sparse_mixins = (dnmx_mixin_type_info**)new_buf;

        const uint32_t grown_by = d->num_sparse_mixins - free_id;
        memset(d->sparse_mixins + free_id, 0, S_V_P * grown_by);
    }

    info->id = free_id;
    d->sparse_mixins[free_id] = info;
    return true;
}

static void free_obj_type_info(dnmx_obj_type_info* type) {
    free(type->buf);
    free(type);
}

bool dnmx_domain_unregister_mixin(dnmx_domain* d, const dnmx_mixin_type_info* info) {
    if (!info) return false; // unregister null
    const uint32_t id = info->id;
    if (id >= d->num_sparse_mixins) return false; // invalid id? bad id?
    if (d->sparse_mixins[id] != info) return false; // not sparse here

    d->sparse_mixins[id] = NULL;

    // since this mixin is no longer valid
    // remove all object type infos which reference it
    uint32_t back = 0;
    for (uint32_t i = 0; i < d->num_obj_types; ++i) {
        dnmx_obj_type_info* type = d->obj_types[i];
        if (type->sparse_mixin_indices[id]) {
            free_obj_type_info(type);
            ++back;
        }
        else if (back) {
            // reorder
            d->obj_types[i - back] = type;
        }
    }

    if (back) {
        // some elements were removed
        assert(back <= d->num_obj_types);
        d->num_obj_types -= back;
        if (d->num_obj_types == 0) {
            free(d->obj_types);
            d->obj_types = NULL;
        }
        else {
            // shrink the array
            // if this realloc returns null, we can only crash, I guess...
            d->obj_types = realloc(d->obj_types, S_V_P * d->num_obj_types);
        }
    }

    return true;
}

static bool is_sorted(const dnmx_mixin_type_info** mixins, uint32_t num_mixins) {
    const dnmx_mixin_type_info* prev = mixins[0];
    for (uint32_t i = 1; i < num_mixins; ++i) {
        const dnmx_mixin_type_info* cur = mixins[i];
        if (cur->id <= prev->id) return false;
    }
    return true;
}

static bool is_same_type(const dnmx_obj_type_info* type, const dnmx_mixin_type_info** mixins, uint32_t num_mixins) {
    if (type->num_mixins != num_mixins) return false;
    const dnmx_mixin_type_info** tmixins = type->mixins;
    for (uint32_t i = 0; i < num_mixins; ++i) {
        if (tmixins[i] != mixins[i]) return false;
    }
    return true;
}



// we allocate a single buffer in the dnmx_obj_type_info
// this is the order in which elements are placed inside
// to avoid manually fixin the alignment, we check here that the alignments are in a non-increasing order
static_assert(alignof(dnmx_obj_type_info_call_table_entry) >= alignof(dnmx_obj_type_info_call_table_msg), "fix dnmx_obj_type_info buffer");
static_assert(alignof(dnmx_obj_type_info_call_table_msg) >= alignof(void*), "fix dnmx_obj_type_info buffer");
static_assert(alignof(void*) >= alignof(uint32_t), "fix dnmx_obj_type_info buffer");

#define BUF_SIZE(p, name) (sizeof(p->name[0]) * p->I_DYNAMIX_PP_CAT(num_, name))

const dnmx_obj_type_info* dnmx_domain_get_obj_type_info(dnmx_domain* d, const dnmx_mixin_type_info** mixins, uint32_t num_mixins) {
    assert(num_mixins > 0);
    // we need the mixins to be sorted so we can search in the list of type infos
    assert(is_sorted(mixins, num_mixins));

    // search for type
    for (uint32_t i = 0; i < d->num_obj_types; ++i) {
        const dnmx_obj_type_info* type = d->obj_types[i];
        if (is_same_type(type, mixins, num_mixins)) return type;
    }

    // we need to create a new type info
    dnmx_obj_type_info* new_type = (dnmx_obj_type_info*)malloc(sizeof(dnmx_obj_type_info));
    if (!new_type) return NULL; // out of memory
    new_type->domain = d;

    // prepare single buffer for type

    // calc buf components

    new_type->num_mixins = num_mixins;
    const size_t mixins_buf_size = BUF_SIZE(new_type, mixins);

    // mixins are sorted
    // this means that the last one has the greatest id and there's no point in allocating more
    new_type->num_sparse_mixin_indices = mixins[num_mixins - 1]->id + 1;
    const size_t sparse_mixin_indices_buf_size = BUF_SIZE(new_type, sparse_mixin_indices);

    // alloc and fill buf
    new_type->buf = malloc(mixins_buf_size + sparse_mixin_indices_buf_size);
    if (!new_type->buf) {
        free_obj_type_info(new_type);
        return NULL; // out of memory
    }

    uint8_t* bptr = (uint8_t*)new_type->buf;

    new_type->mixins = (const dnmx_mixin_type_info**)bptr;
    memcpy(new_type->mixins, mixins, mixins_buf_size);
    bptr += mixins_buf_size;

    new_type->sparse_mixin_indices = (uint32_t*)bptr;
    memset(new_type->sparse_mixin_indices, 0, sparse_mixin_indices_buf_size);

    // add new type to types
    void* new_buf = realloc(d->obj_types, S_V_P * (d->num_obj_types + 1));
    if (!new_buf) {
        free_obj_type_info(new_type);
        return NULL; // out of memory
    }
    d->obj_types = (dnmx_obj_type_info**)new_buf;
    d->obj_types[d->num_obj_types] = new_type;
    d->num_obj_types += 1;
    return new_type;
}

void dnmx_domain_clear(dnmx_domain* d) {
    d->num_sparse_mixins = 0;
    free(d->sparse_mixins);
    d->sparse_mixins = NULL;

    for (uint32_t i = 0; i < d->num_obj_types; ++i) {
        dnmx_obj_type_info* type = d->obj_types[i];
        free_obj_type_info(type);
    }
    d->num_obj_types = 0;
    free(d->obj_types);
    d->obj_types = NULL;
}

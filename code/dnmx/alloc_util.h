// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <stddef.h>
#include <stdint.h>

// rounds s up to the nearest multiple of n
static inline size_t dnmx_next_multiple(size_t s, size_t n) {
    return ((s + n - 1) / n) // divide rounding up
        * n;  // and scale
}

// suggested size for the buffer of mixin data in an object
// this is conservative
// a custom allocator may have better guarantees and allocate less
static inline size_t dnmx_size_for_mixin_data_buf(size_t size, size_t alignment) {
    // if alignment is less than sizeof(void*) we would still have to align the object pointer in the buf
    // so we force the alignment to fit the pointer
    // this we will guarantee that there's room for an aligned pointer and the object inside
    if (alignment < sizeof(void*)) alignment = sizeof(void*);
    return size + alignment + sizeof(void*);
}

// offset in the mixin a mixin_data buffer so as to leave enough room for the object pointer before it
static inline size_t dnmx_mixin_offset_in_data_buf(const void* buf, size_t alignment) {
    // if alignment is less than sizeof(void*) we would still have to align the object pointer in the buf
    // so we force the alignment to fit the pointer
    // this we will guarantee that there's room for an aligned pointer and the object inside
    if (alignment < sizeof(void*)) alignment = sizeof(void*);
    uintptr_t buf_addr = (uintptr_t)buf;
    uintptr_t mixin_addr = dnmx_next_multiple(buf_addr + sizeof(void*), alignment);
    return mixin_addr - buf_addr;
}

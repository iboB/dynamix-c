// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <dnmx/alloc_util.h>
#include "s-unity.h"

void setUp(void) {}
void tearDown(void) {}

#define cnt(ar) (sizeof(ar)/sizeof(ar[0]))

void util(void) {
    const size_t alignments[] = {0, 1, 2, 4, 8, 16, 32, 64, 128};
    uint8_t buf[1024] = {0};
    for (size_t ia = 0; ia < cnt(alignments); ++ia) {
        const size_t a = alignments[ia];
        for (size_t is = 1; is < 2; ++is) {
            const size_t s = is * ia;
            const size_t buf_size = dnmx_size_for_mixin_data_buf(s, a);
            for (size_t off = 0; off <= a; ++off) {
                const uint8_t* p_buf_begin = buf + off;
                const size_t moff = dnmx_mixin_offset_in_data_buf(p_buf_begin, a);
                const uintptr_t buf_begin = (uintptr_t)p_buf_begin;
                const uintptr_t buf_end = buf_begin + buf_size;
                const uintptr_t mpos = buf_begin + moff;
                if (a) {
                    T_CHECK(mpos % a == 0); // mixin buf alignment
                }
                T_CHECK(buf_end - mpos >= s); // enough room for mixin
                const uintptr_t objpos = mpos - sizeof(void*);
                T_CHECK(objpos % sizeof(void*) == 0); // obj ptr alignment
            }
        }
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(util);
    return UNITY_END();
}
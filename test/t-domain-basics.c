// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "w-unity.h"
#include <dnmx/domain.h>

void setUp(void) {}
void tearDown(void) {}

void empty_domain(void) {
    dnmx_domain dom = {0};
    dnmx_domain_clear(&dom);
    TEST_ASSERT_NULL(dom.obj_types);
    TEST_ASSERT_EQUAL(dom.num_obj_types, 0);
    TEST_ASSERT_NULL(dom.sparse_mixins);
    TEST_ASSERT_EQUAL(dom.num_sparse_mixins, 0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(empty_domain);
    return UNITY_END();
}

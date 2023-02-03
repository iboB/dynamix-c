// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

// include unity but disable its warnings
// and add shorthand macros

#include <splat/warnings.h>

PRAGMA_WARNING_PUSH
DISABLE_MSVC_WARNING(4061)
#include <unity.h>
PRAGMA_WARNING_POP

#define T_NULL      TEST_ASSERT_NULL
#define T_NOT_NULL  TEST_ASSERT_NOT_NULL
#define T_EQ        TEST_ASSERT_EQUAL
#define T_EQ_PTR    TEST_ASSERT_EQUAL_PTR
#define T_TRUE      TEST_ASSERT_TRUE
#define T_FALSE     TEST_ASSERT_FALSE
#define T_CHECK     TEST_ASSERT
#define T_GT        TEST_ASSERT_GREATER_THAN

#define T_SV_EQ(a, b) T_TRUE(dnmx_sv_eq(a, b))
#define T_SV_EXPECT(expected, sv) T_SV_EQ(dnmx_make_sv_str(expected), sv)

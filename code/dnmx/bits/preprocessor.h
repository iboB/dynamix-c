// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#define I_DYNAMIX_PP_EMPTY()

#define I_DYNAMIX_PP_CAT(a, b) I_DYNAMIX_PP_INTERNAL_CAT(a, b)
#define I_DYNAMIX_PP_INTERNAL_CAT(a, b) a##b

#define I_DYNAMIX_PP_STRINGIZE(x) #x

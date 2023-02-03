// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#if !defined(SYMBOL_IMPORT)
#   if defined(_WIN32)
#       define SYMBOL_EXPORT __declspec(dllexport)
#       define SYMBOL_IMPORT __declspec(dllimport)
#   else
#       define SYMBOL_EXPORT __attribute__((__visibility__("default")))
#       define SYMBOL_IMPORT
#   endif
#endif

#if DYNAMIX_C_SHARED
#   if BUILDING_DYNAMIX_C
#       define DYNAMIX_C_API SYMBOL_EXPORT
#   else
#       define DYNAMIX_C_API SYMBOL_IMPORT
#   endif
#else
#   define DYNAMIX_C_API
#endif

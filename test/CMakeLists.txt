# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:ThrowTheSwitch/Unity@2.5.2)
CPMAddPackage(gh:iboB/splat@1.2.2)

include(icm_testing)

macro(dynamix_c_test test)
    icm_add_test(
        NAME dynamix-c-${test}
        TARGET test-dynamix-c-${test}
        LIBRARIES
            unity::framework
            dynamix::dynamix-c
            splat::splat
        SOURCES
            ${ARGN}
        FOLDER test
    )
endmacro()

dynamix_c_test(bits-sv t-bits-sv.c)

dynamix_c_test(domain-basics t-domain-basics.c)

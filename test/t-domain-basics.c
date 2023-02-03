// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "s-unity.h"
#include <dnmx/domain.h>
#include <dnmx/mixin_type_info.h>

#include <stdalign.h>

void setUp(void) {}
void tearDown(void) {}

void empty_domain(void) {
    dnmx_domain dom = {0};
    dnmx_domain_clear(&dom);
    T_NULL(dom.obj_types);
    T_EQ(0, dom.num_obj_types);
    T_NULL(dom.sparse_mixins);
    T_EQ(0, dom.num_sparse_mixins);
}

typedef struct position {
    int x;
    int y;
} position;

dnmx_mixin_type_info get_position_mixin_type_info(void) {
    dnmx_mixin_type_info ret = {0};
    ret.name = "position";
    ret.size = sizeof(position);
    ret.alignment = alignof(position);
    return ret;
}

typedef struct person {
    const char* name;
    int age;
} person;

dnmx_mixin_type_info get_person_mixin_type_info(void) {
    dnmx_mixin_type_info ret = {0};
    ret.name = "person";
    ret.size = sizeof(person);
    ret.alignment = alignof(person);
    return ret;
}

typedef struct inventory {
    const char** items;
    size_t num_items;
} inventory;

dnmx_mixin_type_info get_inventory_mixin_type_info(void) {
    dnmx_mixin_type_info ret = {0};
    ret.name = "inventory";
    ret.size = sizeof(inventory);
    ret.alignment = alignof(inventory);
    return ret;
}

typedef struct stats {
    int str;
    int dex;
    int cha;
    int wis;
    const char** feats;
    size_t num_feats;
} stats;

dnmx_mixin_type_info get_stats_mixin_type_info(void) {
    dnmx_mixin_type_info ret = {0};
    ret.name = "stats";
    ret.size = sizeof(stats);
    ret.alignment = alignof(stats);
    return ret;
}

void reg_unreg_mixin(void) {
    dnmx_domain dom = {0};

    dnmx_mixin_type_info pos_info = get_position_mixin_type_info();

    // adding a new mixin type info should occupy slot 1
    T_EQ(DNMX_INVALID_MIXIN_ID, pos_info.id);
    T_TRUE(dnmx_domain_register_mixin(&dom, &pos_info));
    T_EQ(1, pos_info.id);
    T_NOT_NULL(dom.sparse_mixins);
    T_GT(1, dom.num_sparse_mixins);
    T_NULL(dom.sparse_mixins[0]);
    T_EQ(&pos_info, dom.sparse_mixins[1]);
    for (uint32_t i = 2; i < dom.num_sparse_mixins; ++i) {
        T_NULL(dom.sparse_mixins[i]);
    }

    // remove pos_info
    T_TRUE(dnmx_domain_unregister_mixin(&dom, &pos_info));
    pos_info.id = DNMX_INVALID_MIXIN_ID;

    // sparse mixins must be preserved as a buffer, but it should be all-null
    T_GT(1, dom.num_sparse_mixins);
    T_NOT_NULL(dom.sparse_mixins);
    for (uint32_t i = 0; i < dom.num_sparse_mixins; ++i) {
        T_NULL(dom.sparse_mixins[i]);
    }

    // adding it again should place it in slot 1 once more
    T_TRUE(dnmx_domain_register_mixin(&dom, &pos_info));
    T_EQ(1, pos_info.id);
    T_EQ(&pos_info, dom.sparse_mixins[1]);

    // adding another should place it at slot 2
    dnmx_mixin_type_info pers_info = get_person_mixin_type_info();
    T_EQ(DNMX_INVALID_MIXIN_ID, pers_info.id);
    T_TRUE(dnmx_domain_register_mixin(&dom, &pers_info));
    T_GT(2, dom.num_sparse_mixins);
    T_EQ(2, pers_info.id);
    T_EQ(&pos_info, dom.sparse_mixins[1]); // pos is preserved
    T_EQ(&pers_info, dom.sparse_mixins[2]); // pers is here
    for (uint32_t i = 3; i < dom.num_sparse_mixins; ++i) {
        T_NULL(dom.sparse_mixins[i]);
    }

    // removing pos should free slot 1
    T_TRUE(dnmx_domain_unregister_mixin(&dom, &pos_info));
    pos_info.id = DNMX_INVALID_MIXIN_ID;

    dnmx_domain_clear(&dom);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(empty_domain);
    RUN_TEST(reg_unreg_mixin);
    return UNITY_END();
}

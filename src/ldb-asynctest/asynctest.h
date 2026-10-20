/*
 * SPDX-FileCopyrightText: 2026 LI Daobing
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <ldb-asynctask/async_task.h>
#include <list.h>

struct ldb_async_suite;
struct ldb_async_case;
struct ldb_async_test {
    struct ldb_async_task intf;
    struct ldb_list suites;
};

struct ldb_async_test* ldb_async_test_new(void);
bool ldb_async_test_add(struct ldb_async_test* test,
                        struct ldb_async_suite* suite);

struct ldb_async_suite {
    struct ldb_async_task intf;
    struct ldb_list link;
    struct ldb_list cases;
};

struct ldb_async_suite* ldb_async_suite_new(void);
bool ldb_async_suite_add(struct ldb_async_suite* suite,
                         struct ldb_async_case* case_);

struct ldb_async_case {
    struct ldb_async_task intf;
    struct ldb_list link;
};
struct ldb_async_case* ldb_async_case_new(void);

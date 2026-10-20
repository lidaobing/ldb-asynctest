/*
 * SPDX-FileCopyrightText: 2026 LI Daobing
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/* C11/C17 with GCC or Clang (__typeof__ and statement expressions).
 * Circular, intrusive, doubly linked list. Not internally synchronized.
 * Initialize every head/node before use. Never copy a linked head/node.
 */
struct ldb_list {
    struct ldb_list* next;
    struct ldb_list* prev;
};

#define LDB_LIST_INIT(name) {&(name), &(name)}
#define LDB_LIST_HEAD(name) struct ldb_list name = LDB_LIST_INIT(name)

static inline void ldb_list_init(struct ldb_list* node) {
    node->next = node;
    node->prev = node;
}

static inline bool ldb_list_empty(const struct ldb_list* head) {
    return head->next == head;
}

/* Internal insertion primitive; node must not already be linked. */
static inline void ldb_list_insert_between(struct ldb_list* node,
                                           struct ldb_list* prev,
                                           struct ldb_list* next) {
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

static inline void ldb_list_add(struct ldb_list* node, struct ldb_list* head) {
    ldb_list_insert_between(node, head, head->next);
}

static inline void ldb_list_add_tail(struct ldb_list* node,
                                     struct ldb_list* head) {
    ldb_list_insert_between(node, head->prev, head);
}

/* Removes and reinitializes node. Do not pass a nonempty list head. */
static inline void ldb_list_del(struct ldb_list* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    ldb_list_init(node);
}

/* ptr is evaluated once. Exact pointer type is required: void *, unrelated
 * pointers and const pointers are rejected by the mutable entry macro.
 * member must be a struct ldb_list member. ptr must address that member of
 * a live object, never NULL or the list head sentinel.
 */
#define ldb_list_entry(ptr, type, member)                                    \
    __extension__({                                                          \
        _Static_assert(                                                      \
            _Generic(&((type*)0)->member, struct ldb_list*: 1, default: 0),  \
            "member must have type struct ldb_list");                        \
        _Static_assert(                                                      \
            _Generic((ptr), __typeof__(&((type*)0)->member): 1, default: 0), \
            "list_entry: incompatible member pointer type");                 \
        struct ldb_list* ldb_list_entry_ptr_ = (ptr);                        \
        (type*)((char*)ldb_list_entry_ptr_ - offsetof(type, member));        \
    })

/* Read-only recovery accepts either mutable or const member pointers. */
#define ldb_list_entry_const(ptr, type, member)                             \
    __extension__({                                                         \
        _Static_assert(                                                     \
            _Generic(&((type*)0)->member, struct ldb_list*: 1, default: 0), \
            "member must have type struct ldb_list");                       \
        _Static_assert(_Generic((ptr),                                      \
                           struct ldb_list*: 1,                             \
                           const struct ldb_list*: 1,                       \
                           default: 0),                                     \
                       "list_entry_const: incompatible member pointer");    \
        const struct ldb_list* ldb_list_entry_ptr_ = (ptr);                 \
        (const type*)((const char*)ldb_list_entry_ptr_ -                    \
                      offsetof(type, member));                              \
    })

/* pos is a struct ldb_list * (or const pointer for read-only traversal).
 * head must be a stable expression without side effects.
 */
#define ldb_list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

#define ldb_list_for_each_reverse(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)

/* Allows removal/free of current pos, but not of the saved next node. */
#define ldb_list_for_each_safe(pos, tmp, head)                       \
    for ((pos) = (head)->next, (tmp) = (pos)->next; (pos) != (head); \
         (pos) = (tmp), (tmp) = (pos)->next)

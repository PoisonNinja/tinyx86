#pragma once

#include <stddef.h>

#define containerof(ptr, type, member)                     \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })

struct list_element {
    struct list_element *next, *prev;
};

#define LIST_COMPILE_INIT(list)       \
    {                                 \
        .next = &list, .prev = &list, \
    }

static inline void list_runtime_init(struct list_element *list)
{
    list->next = list;
    list->prev = list;
}

static inline void list_add(struct list_element *list,
                            struct list_element *element)
{
    element->next = list->next;
    element->prev = list;
    list->next->prev = element;
    list->next = element;
}

static inline void list_add_tail(struct list_element *list,
                                 struct list_element *element)
{
    element->next = list;
    element->prev = list->prev;
    list->prev->next = element;
    list->prev = element;
}

static inline void list_delete(struct list_element *element)
{
    element->next->prev = element->prev;
    element->prev->next = element->next;
    element->next = NULL;
    element->prev = NULL;
}

static inline int list_empty(struct list_element *list)
{
    return list->next == list;
}

#define list_first_entry(list, type, member) \
    containerof((list)->next, type, member)

#define list_next_entry(pos, member) \
    containerof((pos)->member.next, typeof(*(pos)), member)

#define list_for_each(list, member, pos)                          \
    for ((pos) = containerof((list)->next, typeof(*pos), member); \
         &(pos)->member != (list);                                \
         pos = containerof((pos)->member.next, typeof(*pos), member))

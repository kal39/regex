#pragma once

#define GEN_LIST_DEF(list_type, data_type)                                     \
    typedef struct list_type {                                                 \
        unsigned int cap;                                                      \
        unsigned int len;                                                      \
        data_type def_val;                                                     \
        data_type* dat;                                                        \
    } list_type;                                                               \
                                                                               \
    list_type* list_type##_create(unsigned int cap, data_type def_val);        \
    void list_type##_destroy(list_type* list);                                 \
                                                                               \
    unsigned int list_type##_len(list_type* list);                             \
                                                                               \
    data_type list_type##_peek(list_type* list);                               \
    data_type* list_type##_peek_ref(list_type* list);                          \
                                                                               \
    data_type list_type##_pop(list_type* list);                                \
                                                                               \
    void list_type##_push(list_type* list, data_type item);                    \
                                                                               \
    data_type list_type##_get(list_type* list, unsigned int idx);              \
    data_type* list_type##_get_ref(list_type* list, unsigned int idx);         \
                                                                               \
    void list_type##_remove(list_type* list, unsigned int idx);                \
    void list_type##_remove_ref(list_type* list, data_type* ref);              \
                                                                               \
    void list_type##_set(list_type* list, unsigned int idx, data_type item);

#define GEN_LIST_IMPL(list_type, data_type)                                    \
    list_type* list_type##_create(unsigned int cap, data_type def_val) {       \
        list_type* list = malloc(sizeof *list);                                \
        list->cap = cap > 0 ? cap : 1;                                         \
        list->len = 0;                                                         \
        list->def_val = def_val;                                               \
        list->dat = malloc(sizeof *list->dat * cap);                           \
        return list;                                                           \
    }                                                                          \
                                                                               \
    void list_type##_destroy(list_type* list) {                                \
        free(list->dat);                                                       \
        free(list);                                                            \
    }                                                                          \
                                                                               \
    unsigned int list_type##_len(list_type* list) {                            \
        return list->len;                                                      \
    }                                                                          \
                                                                               \
    data_type list_type##_peek(list_type* list) {                              \
        return list->len > 0 ? list->dat[list->len - 1] : list->def_val;       \
    }                                                                          \
                                                                               \
    data_type* list_type##_peek_ref(list_type* list) {                         \
        return list->len > 0 ? &list->dat[list->len - 1] : NULL;               \
    }                                                                          \
                                                                               \
    data_type list_type##_pop(list_type* list) {                               \
        return list->len > 0 ? list->dat[--list->len] : list->def_val;         \
    }                                                                          \
                                                                               \
    void list_type##_push(list_type* list, data_type item) {                   \
        if (list->len == list->cap) {                                          \
            list->cap *= 2;                                                    \
            list->dat = realloc(list->dat, sizeof *list->dat * list->cap);     \
        }                                                                      \
        list->dat[list->len++] = item;                                         \
    }                                                                          \
                                                                               \
    data_type list_type##_get(list_type* list, unsigned int idx) {             \
        return idx < list->len ? list->dat[idx] : list->def_val;               \
    }                                                                          \
                                                                               \
    data_type* list_type##_get_ref(list_type* list, unsigned int idx) {        \
        return idx < list->len ? &list->dat[idx] : NULL;                       \
    }                                                                          \
                                                                               \
    void list_type##_remove(list_type* list, unsigned int idx) {               \
        if (idx > list->len) return;                                           \
        for (unsigned int i = idx; i < list->len - 1; i++)                     \
            list->dat[i] = list->dat[i + 1];                                   \
        list->len--;                                                           \
    }                                                                          \
                                                                               \
    void list_type##_remove_ref(list_type* list, data_type* ref) {             \
        if (ref < list->dat) return;                                           \
        unsigned int idx = (ref - list->dat) / sizeof *list->dat;              \
        list_type##_remove(list, idx);                                         \
    }                                                                          \
                                                                               \
    void list_type##_set(list_type* list, unsigned int idx, data_type item) {  \
        if (idx < list->len) list->dat[idx] = item;                            \
    }

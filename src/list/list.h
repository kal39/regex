#pragma once

#define GEN_LIST_DEF(list_type, data_type)                                     \
    typedef struct list_type {                                                 \
        int cap;                                                               \
        int len;                                                               \
        data_type def_val;                                                     \
        data_type* dat;                                                        \
    } list_type;                                                               \
                                                                               \
    list_type* list_type##_create(int cap, data_type def_val);                 \
    void list_type##_destroy(list_type* list);                                 \
                                                                               \
    int list_type##_len(list_type* list);                                      \
                                                                               \
    data_type* list_type##_peek_ref(list_type* list);                          \
    data_type list_type##_peek(list_type* list);                               \
    data_type list_type##_pop(list_type* list);                                \
    void list_type##_push(list_type* list, data_type item);                    \
                                                                               \
    data_type* list_type##_get_ref(list_type* list, int idx);                  \
    data_type list_type##_get(list_type* list, int idx);                       \
    void list_type##_set(list_type* list, int idx, data_type item);            \
    data_type list_type##_remove(list_type* list, int idx);

#define GEN_LIST_IMPL(list_type, data_type)                                    \
    list_type* list_type##_create(int cap, data_type def_val) {                \
        list_type* list = malloc(sizeof(list_type));                           \
        list->cap = cap >= 2 ? cap : 2;                                        \
        list->len = 0;                                                         \
        list->def_val = def_val;                                               \
        list->dat = malloc(sizeof(data_type) * list->cap);                     \
        return list;                                                           \
    }                                                                          \
                                                                               \
    void list_type##_destroy(list_type* list) {                                \
        free(list->dat);                                                       \
        free(list);                                                            \
    }                                                                          \
                                                                               \
    int list_type##_len(list_type* list) {                                     \
        return list->len;                                                      \
    }                                                                          \
                                                                               \
    data_type* list_type##_peek_ref(list_type* list) {                         \
        return list->len > 0 ? &list->dat[list->len - 1] : NULL;               \
    }                                                                          \
                                                                               \
    data_type list_type##_peek(list_type* list) {                              \
        return list->len > 0 ? list->dat[list->len - 1] : list->def_val;       \
    }                                                                          \
                                                                               \
    data_type list_type##_pop(list_type* list) {                               \
        return list->len > 0 ? list->dat[--list->len] : list->def_val;         \
    }                                                                          \
                                                                               \
    void list_type##_push(list_type* list, data_type item) {                   \
        if (list->len == list->cap) {                                          \
            list->cap *= 2;                                                    \
            list->dat = realloc(list->dat, sizeof(data_type) * list->cap);     \
        }                                                                      \
        list->dat[list->len++] = item;                                         \
    }                                                                          \
                                                                               \
    data_type* list_type##_get_ref(list_type* list, int idx) {                 \
        return 0 <= idx && idx < list->len ? &list->dat[idx] : NULL;           \
    }                                                                          \
                                                                               \
    data_type list_type##_get(list_type* list, int idx) {                      \
        return 0 <= idx && idx < list->len ? list->dat[idx] : list->def_val;   \
    }                                                                          \
                                                                               \
    void list_type##_set(list_type* list, int idx, data_type item) {           \
        if (0 <= idx && idx < list->len) list->dat[idx] = item;                \
    }                                                                          \
                                                                               \
    data_type list_type##_remove(list_type* list, int idx) {                   \
        if (0 <= idx && idx < list->len) {                                     \
            data_type item = list->dat[idx];                                   \
            for (int i = idx; i < list->len - 1; i++)                          \
                list->dat[i] = list->dat[i + 1];                               \
            list->len--;                                                       \
            return item;                                                       \
        }                                                                      \
        return list->def_val;                                                  \
    }

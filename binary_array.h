#ifndef _HEADER_BIN_ARRAY_T_
#define _HEADER_BIN_ARRAY_T_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define bin_u_char unsigned char
#define bin_u_long unsigned long
#define bin_u_int uintptr_t

#define __DEFAULT_RS_CAPACITY__ 10
#define __MIN_ON2_THRESHOLD__ 80

typedef enum { bin_idx_eq, bin_idx_gt, bin_idx_lt } bin_idx_condtion;

typedef void* (*bin_arr_malloc_fn)(size_t);
typedef void* (*bin_arr_realloc_fn)(void*, size_t);
typedef void (*bin_arr_free_fn)(void*);

static bin_arr_malloc_fn __bin_arr_malloc_fn = malloc;

__attribute__((unused))
static bin_arr_realloc_fn __bin_arr_realloc_fn = realloc;
__attribute__((unused))
static bin_arr_free_fn __bin_arr_free_fn = free;

typedef int (*idx_cmp_func)(const void*, const void*);
typedef void (*free_node_fn)(void*);

static inline int __def_int_sorted_cmp_func__ (const void* a, const void*b) {
    return ( *(const int*)a - * (const int*)b );
}

static inline int __def_long_sorted_cmp_func__ (const void* a, const void*b) {
    return ( *(const long*)a - * (const long*)b );
}

static inline int __def_float_sorted_cmp_func__ (const void* a, const void*b) {
    const float aa = *(const float*)a, bb = *(const float*)b;
    if (aa > bb) return 1;
    if (aa < bb) return -1;
    return 0;
}

static inline int __def_double_sorted_cmp_func__ (const void* a, const void*b) {
    const double aa = *(const double*)a, bb = *(const double*)b;
    if (aa > bb) return 1;
    if (aa < bb) return -1;
    return 0;
}

static inline int __def_char_sorted_cmp_func__ (const void* a, const void*b) {
    return (int)( *(const char*)a - * (const char*)b );
}

static inline int __def_cstr_sorted_cmp_func__(const void *a, const void *b) {
    return strcmp(*(const char **)(uintptr_t)a, *(const char **)(uintptr_t)b);
}

static inline int __def_cstr2_sorted_cmp_func__(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

typedef struct {
    size_t size;
    union {
        bin_u_char **ptrs;
        bin_u_int *_uintptrs_; // for internal merging
    };
    size_t capacity;
} bin_array_rs;

typedef struct {
    union {
        bin_u_char **arr_ref;
        bin_u_int *uintptr_arr;
    };
    bin_u_long offset;
    idx_cmp_func cmp_func;
} bin_array_idx;

typedef struct {
    bin_u_int   size;
    bin_u_int   capacity;

    bin_u_int num_of_index;
    bin_array_idx * _index_arr_;
} bin_array_t;

bin_array_t *bin_array_create(bin_u_int size, bin_u_int num_of_index);
void bin_array_clear(bin_array_t *a, free_node_fn free_node);
void bin_array_destroy(bin_array_t *a, free_node_fn free_node);
#ifndef DISABLE_BA_SWAP
void bin_array_safety_swap(bin_array_t **curr, bin_array_t *new_a, free_node_fn free_node, unsigned int buffer_time_sec);
#endif
int bin_add_index_(bin_array_t *a, bin_u_long offset, idx_cmp_func cmp_func);
bin_u_char ** get_index_array(bin_array_t *a, bin_u_int index_num);

int bin_array_push(bin_array_t *a, void* node);
int bin_array_push_n(bin_array_t *a,  bin_u_char* node, bin_u_int num);


#define bin_add_index(__a__, __struct_type__, __field_member__, __cmp_func__) \
bin_add_index_(__a__, offsetof(__struct_type__, __field_member__), __cmp_func__)

#define bin_sort_rs_by(__rs__, __cmp_func__) \
qsort(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __cmp_func__)

/** Result Merging other result **/
bin_array_rs* bin_array_rs_create(size_t capacity);
int bin_push_rs(bin_array_rs *rs, void* data);
int bin_push_rs_n(bin_array_rs *rs, bin_u_int start_ind, bin_u_int width,  bin_u_char **arr_ref);
bin_array_rs* bin_rs_rm_dup_by(bin_array_rs *rs, idx_cmp_func cmp_func, bool free_after_merge);
bin_array_rs* bin_union_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge);
bin_array_rs* bin_intersect_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge);
bin_array_rs* bin_append_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge);
void bin_free_rs(bin_array_rs *rs);

/** Result Sorting Asc and Desc **/
#ifdef __APPLE__
#define bin_sort_rs(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
int (^__f__)() = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}else{\
int (^__f__)() = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}\
})

#define bin_sort_rs_desc(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
int (^__f__)() = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}else{\
int (^__f__)() = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}\
})
#else // Linux OS or windows os
#define bin_sort_rs(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
int (*__f__)(const void *a, const void *b) =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}else{\
int (*__f__)(const void *a, const void *b) =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}\
})

#define bin_sort_rs_desc(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
int (*__f__)(const void *a, const void *b) =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}else{\
int (*__f__)(const void *a, const void *b) =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(bin_u_char*), __f__);\
}\
})

#endif
/** End Result Sorting Acs And Desc **/


void* basearch_direct_one(const void *key, bin_array_t *a, bin_u_long offset);
bin_array_rs* basearch_index(const void *key, bin_array_t *a, bin_u_long offset, bin_idx_condtion cond);


#define ba_search_one(__a__, __struct_type__, __field_member__, __key__)\
basearch_direct_one(__key__, __a__, offsetof(__struct_type__, __field_member__))

#define ba_search_eq(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq)

#define ba_search_lt(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_lt)

#define ba_search_gt(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_gt)


/*** For multiple eq query scope ***/

#define ba_search_multi_eq2(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = basearch_index(__VA_ARGS__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq3(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq2(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})


#define ba_search_multi_eq4(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq3(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq5(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq4(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq6(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq5(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq7(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq6(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq8(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq7(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq9(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq8(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define ba_search_multi_eq10(__a__, __struct_type__, __field_member__, __key__, ...) ({\
bin_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), bin_idx_eq);\
bin_array_rs *__rs2__ = ba_search_multi_eq9(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
bin_array_rs *__rs10__ = bin_union_rs(__rs1__, __rs2__, true);\
__rs10__;})


/*** End for multiple eq query scope ***/

static inline int
bin_array_init(bin_array_t *array, bin_u_int capacity, bin_u_int num_of_index)
{

    if (num_of_index == 0)  {
        perror("at least 1 index");
        return 0;
    }

    array->num_of_index = num_of_index;

    array->size = 0;
    array->capacity = capacity;

    array->_index_arr_ = (bin_array_idx *) __bin_arr_malloc_fn(num_of_index * sizeof(bin_array_idx));
    size_t i;
    for (i = 0; i < num_of_index; i++) {
        array->_index_arr_[i].arr_ref = (bin_u_char **) __bin_arr_malloc_fn(capacity * sizeof(bin_u_char *));
        array->_index_arr_[i].cmp_func = NULL; // init 0 until index added
        array->_index_arr_[i].offset = -1; // init -1 until index added
        if (array->_index_arr_[i].arr_ref == NULL)
            return 0;
    }

    return 1;
}

#ifdef __cplusplus
}
#endif

#endif /* _HEADER_BIN_ARRAY_T_ */

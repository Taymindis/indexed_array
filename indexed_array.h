#ifndef _HEADER_IDX_ARRAY_T_
#define _HEADER_IDX_ARRAY_T_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define idxarr_u_char unsigned char
#define idxarr_u_long unsigned long
#define idxarr_u_int uintptr_t

#define __DEFAULT_RS_CAPACITY__ 10
#define __MIN_ON2_THRESHOLD__ 80

typedef enum { idxarr_eq, idxarr_gt, idxarr_lt } idxarr_condtion;
typedef enum {
    idxarr_sstr_dataType, // stack string
    idxarr_hstr_dataType, // heap string
    idxarr_int_dataType,
    idxarr_long_dataType,
    idxarr_float_dataType,
    idxarr_double_dataType,
    idxarr_char_dataType
} idxarr_data_type;

typedef void* (*idxarr_malloc_fn)(size_t);
typedef void* (*idxarr_realloc_fn)(void*, size_t);
typedef void (*idxarr_free_fn)(void*);

static idxarr_malloc_fn __idxarr_malloc_fn = malloc;

__attribute__((unused))
static idxarr_realloc_fn __idxarr_realloc_fn = realloc;
__attribute__((unused))
static idxarr_free_fn __idxarr_free_fn = free;

#define __idx_is_valid_string__(_idx_elt__) ({ sizeof(*_idx_elt__) == sizeof(char)? 1: 0; })

typedef int (*idxarr_cmp_func)(const void*, const void*);


#ifdef __APPLE__
typedef int (^idx_cmp_func)(const void*, const void*);
#else
typedef int (*idx_cmp_func)(const void*, const void*);
#endif


typedef void (*free_node_fn)(void*);


typedef struct {
    union {
        void *elt;
        char ckey;
        const char *skey;
        int ikey;
        long lkey;
        float fkey;
        double dkey;
    };
    size_t klen;
} __idxarr_args_with_meta__;

typedef struct {
    size_t size;
    union {
        idxarr_u_char **ptrs;
        idxarr_u_int *_uintptrs_; // for internal merging
    };
    size_t capacity;
} idx_array_rs;

typedef struct {
    union {
        idxarr_u_char **arr_ref;
        idxarr_u_int *uintptr_arr;
    };
    idxarr_u_long offset;
    idxarr_data_type dtype;
    idx_cmp_func cmp_func;
} idxarr_array_idx;

typedef struct {
    idxarr_u_int   size;
    idxarr_u_int   capacity;

    idxarr_u_int num_of_index;
    idxarr_array_idx * _index_arr_;
} idx_array_t;

idx_array_t *idxarr_create(idxarr_u_int size, idxarr_u_int num_of_index);
void idxarr_clear(idx_array_t *a, free_node_fn free_node);
void idxarr_destroy(idx_array_t *a, free_node_fn free_node);
#ifndef DISABLE_BA_SWAP
void idxarr_safety_swap(idx_array_t **curr, idx_array_t *new_a, free_node_fn free_node, unsigned int buffer_time_sec);
#endif
idxarr_u_char ** idxarr_get_index_array(idx_array_t *a, idxarr_u_int index_num);

int idxarr_push(idx_array_t *a, void* node);
int idxarr_push_n(idx_array_t *a,  idxarr_u_char* node, idxarr_u_int num);

#ifdef __APPLE__
#define idxarr_get_int_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = ^int(const void* a, const void *b){\
int __arg__  =  *(int*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_long_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = ^int(const void* a, const void *b){\
long __arg__  =  *(long*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_float_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = ^int(const void* a, const void *b){\
float __arg__  =  *(float*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_double_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ = ^int(const void* a, const void *b){\
double __arg__  =  *(double*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_char_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ = ^int(const void* a, const void *b){\
char __arg__  =  *(char*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_str_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ = ^int(const void* a, const void *b){\
const char *__arg__  =  (const char *)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strcmp(__arg__, __b__->__field__);\
};\
__f__;})
#else

#ifdef __cplusplus

#define idxarr_get_int_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
int __arg__  =  *(int*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_long_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
long __arg__  =  *(long*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_float_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
float __arg__  =  *(float*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_double_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
double __arg__  =  *(double*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_char_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
char __arg__  =  *(char*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
};\
__f__;})
#define idxarr_get_str_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__ = [&](const void*a,const void* b) -> int {\
const char *__arg__  =  (const char *)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strcmp(__arg__, __b__->__field__);\
};\
__f__;})

#else// C compiler

#define idxarr_get_int_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
int __arg__  =  *(int*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
__f__;})
#define idxarr_get_long_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
long __arg__  =  *(long*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
__f__;})
#define idxarr_get_float_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
float __arg__  =  *(float*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
__f__;})
#define idxarr_get_double_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
double __arg__  =  *(double*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
__f__;})
#define idxarr_get_char_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
char __arg__  =  *(char*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
if (__arg__ > __b__->__field__) return 1;\
if (__arg__ < __b__->__field__) return -1;\
return 0;\
}\
__fn__;\
});\
__f__;})
#define idxarr_get_str_cmp_func(__struct_type__, __field__)({\
idx_cmp_func __f__;\
__f__ =\
({\
int __fn__ (const void* a, const void *b) {\
const char *__arg__  =  (const char *)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strcmp(__arg__, __b__->__field__);\
}\
__fn__;\
});\
__f__;})
#endif

#endif

#define idxarr_add_int_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_int_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_int_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_long_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_long_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_long_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_float_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_float_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_float_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_double_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_double_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_double_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_char_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_char_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_char_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_heap_str_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_hstr_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_str_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})
#define idxarr_add_stack_str_index(__a__, __struct_type__, __field_member__)({\
int status = 0;\
if(__a__ && __a__->num_of_index > 0) {\
size_t i;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func == NULL) {\
__a__->_index_arr_[i].offset = offsetof(__struct_type__, __field_member__);\
__a__->_index_arr_[i].dtype = idxarr_sstr_dataType;\
__a__->_index_arr_[i].cmp_func = idxarr_get_str_cmp_func(__struct_type__, __field_member__);\
status = 1;\
break;\
}}}\
status;})


#define idxarr_sort_rs_by(__rs__, __cmp_func__) \
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __cmp_func__)

/** Algorithm **/
long basearch_index_eq_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp);
long basearch_index_gt_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp);
long basearch_index_lt_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp);
idx_array_rs* basearch_index_by_func(const char *key, idx_array_t *a, idxarr_u_long offset, idx_cmp_func cmp_func);

/** Result Merging other result **/
idx_array_rs* idxarr_rs_create(size_t capacity);
int idxarr_push_rs(idx_array_rs *rs, void* data);
int idxarr_push_rs_n(idx_array_rs *rs, idxarr_u_int start_ind, idxarr_u_int width,  idxarr_u_char **arr_ref);
idx_array_rs* idxarr_rs_rm_dup_by(idx_array_rs *rs, idxarr_cmp_func cmp_func, bool free_after_merge);
idx_array_rs* idxarr_union_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
idx_array_rs* idxarr_intersect_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
idx_array_rs* idxarr_append_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
void idxarr_free_rs(idx_array_rs *rs);

/** Result Sorting Asc and Desc **/
#ifdef __APPLE__
#define idxarr_search_str_start_with(__a__, __struct_type__, __field_member__, __key__)({\
assert(__idx_is_valid_string__(__key__) && "please do not need to dereference your string, it is already a pointer.");\
idx_cmp_func __f__ = ^int(const void *a, const void *b){\
__idxarr_args_with_meta__ *key_arg = (__idxarr_args_with_meta__*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strncmp(key_arg->skey, __b__->__field_member__, key_arg->klen);\
};\
basearch_index_by_func(__key__, __a__, offsetof(__struct_type__, __field_member__), __f__);\
})
#define idxarr_sort_rs(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})

#define idxarr_sort_rs_desc(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ = ^int(const void *a, const void *b){\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort_b(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})
#else // Linux OS or windows os
#ifdef __cplusplus

#define idxarr_search_str_start_with(__a__, __struct_type__, __field_member__, __key__)({\
assert(__idx_is_valid_string__(__key__) && "please do not need to dereference your string, it is already a pointer.");\
idx_cmp_func __f__ = [&](const void*a, const void*b) -> int {\
__idxarr_args_with_meta__ *key_arg = (__idxarr_args_with_meta__*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strncmp(key_arg->skey, __b__->__field_member__, key_arg->klen);\
};\
basearch_index_by_func(__key__, __a__, offsetof(__struct_type__, __field_member__), __f__);\
})

#define idxarr_sort_rs(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ = [&](const void*a, const void*b) -> int {\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ = [&](const void*a, const void*b) -> int {\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})

#define idxarr_sort_rs_desc(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ = [&](const void*a, const void*b) -> int {\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
};\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ = [&](const void*a, const void*b) -> int {\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
if (__a__->__field__ > __b__->__field__) return 1;\
if (__a__->__field__ < __b__->__field__) return -1;\
return 0;\
};\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})
#else// C compiler
#define idxarr_search_str_start_with(__a__, __struct_type__, __field_member__, __key__)({\
assert(__idx_is_valid_string__(__key__) && "please do not need to dereference your string, it is already a pointer.");\
idx_cmp_func __f__ = ({\
int __fn__ (const void *a, const void *b) {\
__idxarr_args_with_meta__ *key_arg = (__idxarr_args_with_meta__*)a;\
__struct_type__ *__b__ = (__struct_type__*)b;\
return strncmp(key_arg->skey, __b__->__field_member__, key_arg->klen);\
}\
__fn__;\
});\
basearch_index_by_func(__key__, __a__, offsetof(__struct_type__, __field_member__), __f__);\
})
#define idxarr_sort_rs(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)a;\
__struct_type__ *__b__ = *(__struct_type__**)b;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ =\
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
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})

#define idxarr_sort_rs_desc(__rs__, __struct_type__, __field__, __is_string_data_type__) ({\
if(__is_string_data_type__){\
idx_cmp_func __f__ =\
({\
int __fn__ (const void *a, const void *b) {\
__struct_type__ *__a__ = *(__struct_type__**)b;\
__struct_type__ *__b__ = *(__struct_type__**)a;\
return strcmp((const char*)(uintptr_t)__a__->__field__, (const char*)(uintptr_t)__b__->__field__);\
}\
__fn__;\
});\
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}else{\
idx_cmp_func __f__ =\
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
qsort(__rs__->ptrs, __rs__->size, sizeof(idxarr_u_char*), __f__);\
}\
})

#endif

#endif
/** End Result Sorting Acs And Desc **/

#define basearch_direct_one(__key__, __a__, __offset__)({\
size_t i;\
void *rt=NULL;\
idxarr_array_idx *arr_idx;\
long asize, lt_ind, gt_ind;\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func != NULL && __a__->_index_arr_[i].offset == __offset__) {\
arr_idx = __a__->_index_arr_ + i;\
asize = __a__->size;\
lt_ind = basearch_index_lt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
gt_ind = basearch_index_gt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
if ( lt_ind >= asize || gt_ind <= 0 || gt_ind - lt_ind < 2) {\
break;\
}\
if (lt_ind < 0)\
lt_ind = 0;\
else lt_ind++;\
if (lt_ind >= 0 && lt_ind < asize) {\
rt = arr_idx->arr_ref[lt_ind];\
}\
}\
}\
rt;\
})

#define basearch_index(__key__, __a__, __offset__, __cond__)({\
size_t i;\
idxarr_array_idx *arr_idx;\
long asize;\
idx_array_rs *arr_rs = idxarr_rs_create(__DEFAULT_RS_CAPACITY__);\
for (i = 0; i < __a__->num_of_index; i++) {\
if (__a__->_index_arr_[i].cmp_func != NULL && __a__->_index_arr_[i].offset == __offset__) {\
arr_idx = __a__->_index_arr_ + i;\
long index, lt_ind, gt_ind;\
if (arr_idx->dtype == idxarr_hstr_dataType || arr_idx->dtype == idxarr_sstr_dataType) {\
assert(__idx_is_valid_string__(__key__) && "please do not need to dereference your string, it is already a pointer.");\
}\
switch (__cond__) {\
case idxarr_eq:\
asize = __a__->size;\
lt_ind = basearch_index_lt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
gt_ind = basearch_index_gt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
if ( lt_ind >= asize || gt_ind <= 0 || gt_ind - lt_ind < 2) {\
break;\
}\
if (lt_ind < 0)\
lt_ind = 0;\
else lt_ind++;\
if (gt_ind >= asize)\
gt_ind = asize;\
idxarr_push_rs_n(arr_rs, lt_ind, gt_ind - lt_ind, arr_idx->arr_ref);\
break;\
case idxarr_gt:\
asize = __a__->size;\
index = basearch_index_gt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
if (index >= 0 && index < asize) {\
idxarr_push_rs_n(arr_rs, index, asize - index, arr_idx->arr_ref);\
}\
break;\
case idxarr_lt:\
asize = __a__->size;\
index = basearch_index_lt_(__key__,  arr_idx->arr_ref, asize, arr_idx->cmp_func);\
if (index >= 0 && index < asize) {\
idxarr_push_rs_n(arr_rs, 0, index + 1, arr_idx->arr_ref);\
}\
break;\
default:\
break;\
}\
break;\
}\
}\
arr_rs;\
})

#define idxarr_search_one(__a__, __struct_type__, __field_member__, __key__)\
basearch_direct_one(__key__, __a__, offsetof(__struct_type__, __field_member__))

#define idxarr_search_eq(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq)

#define idxarr_search_lt(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_lt)

#define idxarr_search_gt(__a__, __struct_type__, __field_member__, __key__)\
basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_gt)




/*** For multiple eq query scope ***/

#define idxarr_search_multi_eq2(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = basearch_index(__VA_ARGS__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq3(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq2(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})


#define idxarr_search_multi_eq4(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq3(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq5(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq4(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq6(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq5(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq7(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq6(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq8(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq7(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq9(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq8(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})

#define idxarr_search_multi_eq10(__a__, __struct_type__, __field_member__, __key__, ...) ({\
idx_array_rs *__rs1__ = basearch_index(__key__, __a__, offsetof(__struct_type__, __field_member__), idxarr_eq);\
idx_array_rs *__rs2__ = idxarr_search_multi_eq9(__a__, __struct_type__, __field_member__, __VA_ARGS__);\
idx_array_rs *__rs10__ = idxarr_union_rs(__rs1__, __rs2__, true);\
__rs10__;})


/*** End for multiple eq query scope ***/

static inline int
idxarr_array_init(idx_array_t *array, idxarr_u_int capacity, idxarr_u_int num_of_index)
{

    if (num_of_index == 0)  {
        perror("at least 1 index");
        return 0;
    }

    array->num_of_index = num_of_index;

    array->size = 0;
    array->capacity = capacity;

    array->_index_arr_ = (idxarr_array_idx *) __idxarr_malloc_fn(num_of_index * sizeof(idxarr_array_idx));
    size_t i;
    for (i = 0; i < num_of_index; i++) {
        array->_index_arr_[i].arr_ref = (idxarr_u_char **) __idxarr_malloc_fn(capacity * sizeof(idxarr_u_char *));
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

#endif /* _HEADER_IDX_ARRAY_T_ */

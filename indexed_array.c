#include "indexed_array.h"
#include <unistd.h>


void idxarr_insert(idx_array_t *a, idxarr_u_char* node, size_t index_num);
void idxarr_push_idx_ref(idx_array_t *a, void* node);

void __union__(idx_array_rs *rs1, idx_array_rs *rs2, idx_array_rs *merged_rs, bool free_after_merge);
void __intersection__(idx_array_rs *rs1, idx_array_rs *rs2, idx_array_rs *merged_rs, bool free_after_merge);

static inline int cmp_idx_rs_uint(const void * a, const void * b) {
    return (int)( (*(const idxarr_u_int *)a) - (*(const idxarr_u_int *)b) );
}

// #ifndef DISABLE_BA_SWAP

// #include <pthread.h>

// void *destroy_old_array_(void *swp);
// /* this function and struct is run by the thread only */
// typedef struct {
//     idx_array_t *old_a;
//     free_node_fn free_node;
//     unsigned int secs;
// } ba_swapping_t;

// void *destroy_old_array_(void *swp) {
//     ba_swapping_t *s = (ba_swapping_t*)swp;
//     sleep(s->secs);
//     idxarr_destroy(s->old_a, s->free_node);
//     __idxarr_free_fn(swp);
//     pthread_exit(NULL);
// }

// #endif

/*
 * This is reference of bsearch - https://github.com/torvalds/linux/blob/master/lib/bsearch.c
 * but return index, passing struct field as comparison
 */
long
basearch_index_eq_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    //  size_t ptr_size = sizeof(idxarr_u_char*);
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i));

        if (result == 0)
            return i;

        if (result > 0) {
            mid = i + 1;
            num--;
        }
        num >>= 1;
    }

    if (result > 0) return i + 1; // If cannot find the index and previous result is bigger, please plus 1
    return i;
}

long
basearch_index_gt_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i));

        if (result >= 0) {
            mid = i + 1;
            num--;
        }
        num >>= 1;
    }
    if (result >= 0) return i + 1;
    return i;
}

long
basearch_index_lt_(register const void *key, idxarr_u_char **base, size_t num, register idx_cmp_func cmp) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i));

        if (result > 0) {
            mid = i + 1;
            num--;
        }
        num >>= 1;
    }
    if (result <= 0) return i - 1;
    return i;
}


idx_array_rs*
basearch_index_by_func(const char *key, idx_array_t *a, idxarr_u_long offset, idx_cmp_func cmp_func)
{
    size_t i;
    idxarr_array_idx *arr_idx;
    // long index;
    long asize;
    idx_array_rs *arr_rs = idxarr_rs_create(__DEFAULT_RS_CAPACITY__);
    for (i = 0; i < a->num_of_index; i++) {
        if (a->_index_arr_[i].cmp_func != NULL && a->_index_arr_[i].offset == offset) {
            goto FOUND_INDEX;
        }
    }

    return arr_rs;

FOUND_INDEX:
    arr_idx = a->_index_arr_ + i ; // * sizeof(idxarr_array_idx); it is not void/char ptr, not need adjust by size
    long lt_ind, gt_ind;
    asize = a->size;

    __idxarr_args_with_meta__ key_arg;
    key_arg.skey  = key;
    key_arg.klen = strlen(key);
    lt_ind = basearch_index_lt_((void*)&key_arg,  arr_idx->arr_ref, asize, cmp_func);
    gt_ind = basearch_index_gt_((void*)&key_arg,  arr_idx->arr_ref, asize, cmp_func);

    if ( lt_ind >= asize || gt_ind <= 0 || gt_ind - lt_ind < 2) { // means none
        return arr_rs;
    }

    if (lt_ind < 0)
        lt_ind = 0;
    else lt_ind++;

    if (gt_ind >= asize)
        gt_ind = asize;

    // idxarr_u_char **arr_refs = arr_idx->arr_ref;

    idxarr_push_rs_n(arr_rs, lt_ind, gt_ind - lt_ind, arr_idx->arr_ref);
    return arr_rs;
}

idxarr_u_char **
idxarr_get_index_array(idx_array_t *a, idxarr_u_int index_num) {
    if (index_num < a->num_of_index) {
        return a->_index_arr_[index_num].arr_ref;
    }
    return NULL;
}

// Function to sort an array a[] of size 'n'
void
idxarr_insert(idx_array_t *a, idxarr_u_char* node, size_t index_num) {
    idxarr_array_idx *index_arr = a->_index_arr_ + index_num;// * sizeof(idxarr_array_idx); //it is not void ptr, not need adjust by size
    if (index_arr->cmp_func) {
        long curr_sz = a->size;
        long index_ptr;
        if (index_arr->dtype == idxarr_hstr_dataType) {
            index_ptr = basearch_index_eq_(*(void**)(node + index_arr->offset),  index_arr->arr_ref, curr_sz, index_arr->cmp_func);
        } else {
            index_ptr = basearch_index_eq_(node + index_arr->offset,  index_arr->arr_ref, curr_sz, index_arr->cmp_func);
        }
        /** Memmove for overlap destination **/
        memmove(&index_arr->arr_ref[index_ptr + 1], &index_arr->arr_ref[index_ptr], (curr_sz - index_ptr)  * sizeof(idxarr_u_char*));

        index_arr->arr_ref[index_ptr] = node;
    }
}

idx_array_t *
idxarr_create(idxarr_u_int size, idxarr_u_int num_of_index)
{
    idx_array_t *a;

    a = __idxarr_malloc_fn(sizeof(idx_array_t));
    if (a == NULL) {
        return NULL;
    }

    if (idxarr_array_init(a, size, num_of_index) == 0) {
        return NULL;
    }

    return a;
}

void
idxarr_clear(idx_array_t *a, free_node_fn free_node) {
    size_t i, j;
    if (!free_node) free_node = __idxarr_free_fn;
    for (i = 0; i < a->num_of_index; i++) {
        idxarr_array_idx *idx_arr = a->_index_arr_ + i;// * sizeof(idxarr_array_idx); it is not void ptr, not need adjust by size
        if (idx_arr->cmp_func) {
            if (i == 0) {
                for (j = 0; j < a->size; j++)
                    free_node((void*) idx_arr->arr_ref[j]);
            }
        }
    }
    a->size = 0;
}

void
idxarr_destroy(idx_array_t *a, free_node_fn free_node) {
    size_t i, j;
    if (!free_node) free_node = __idxarr_free_fn;
    for (i = 0; i < a->num_of_index; i++) {
        idxarr_array_idx *idx_arr = a->_index_arr_ + i;// * sizeof(idxarr_array_idx); it is not void ptr, not need adjust by size
        if (idx_arr->cmp_func) {
            if (i == 0) {
                for (j = 0; j < a->size; j++)
                    free_node((void*) idx_arr->arr_ref[j]);
            }
        }

        __idxarr_free_fn(idx_arr->arr_ref);
    }

    __idxarr_free_fn(a->_index_arr_);
    __idxarr_free_fn(a);
}

#ifndef DISABLE_BA_SWAP
void
idxarr_safety_swap(idx_array_t **curr, idx_array_t *new_a, free_node_fn free_node_, unsigned int milisecs) {
    /***Proceed Hazard Ptr***/
    idx_array_t *old_a = *curr;

    while (!__sync_bool_compare_and_swap(curr, old_a, new_a)) {
        old_a = *curr;
    }
    // *curr = new_a;

#ifdef __APPLE__
    usleep(milisecs * 1000);
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
    usleep(milisecs * 1000);
#pragma GCC diagnostic pop
#endif
    idxarr_destroy(old_a, free_node_);

}
#endif

// int idxarr_add_index_(idx_array_t *a, idxarr_u_long offset, idx_cmp_func cmp_func) {
//     if (a && a->num_of_index > 0) {
//         size_t i;
//         for (i = 0; i < a->num_of_index; i++) {
//             if (a->_index_arr_[i].cmp_func == NULL) {
//                 a->_index_arr_[i].offset = offset;
//                 a->_index_arr_[i].cmp_func = cmp_func;
//                 return 1;
//             }
//         }
//         goto ERROR;
//     } else {
// ERROR:
//         fprintf(stderr, "Running out of index\n");
//         return 0;
//     }

// }


void
idxarr_push_idx_ref(idx_array_t *a, void* node) {
    size_t i;
    for (i = 0; i < a->num_of_index; i++) {
        idxarr_insert(a, node, i);
    }
}


int
idxarr_push(idx_array_t *a, void* node) {
    if (a->size == a->capacity) {
        size_t i;
        size_t size_of_ptr = sizeof(idxarr_u_char*);
        for (i = 0; i < a->num_of_index; i++) {
            idxarr_array_idx *idx_array = a->_index_arr_ + i; //* sizeof(idxarr_array_idx); it is not void ptr, not need adjust by size
            idxarr_u_char **new_arr = __idxarr_realloc_fn(idx_array->arr_ref, (a->capacity * 2) * size_of_ptr);
            if (new_arr == NULL) {
                fprintf(stderr, "unable to rellocate more space...\n");
                return 0;
            }
            idx_array->arr_ref = new_arr;
        }
        a->capacity *= 2;
    }
    idxarr_push_idx_ref(a, node);
    a->size++;

    return 1;
}

int
idxarr_push_n(idx_array_t *a,  idxarr_u_char* node, idxarr_u_int num) {
    idxarr_u_int i;
    for (i = 0; i < num; i++) {
        idxarr_push(a, (void*) (uintptr_t)node[i]);
    }
    a->size += num;

    return 1;
}

idx_array_rs*
idxarr_rs_create(size_t capacity) {
    idx_array_rs *rs = __idxarr_malloc_fn(sizeof(idx_array_rs));
    rs->ptrs = __idxarr_malloc_fn(capacity * sizeof(idxarr_u_char*));
    rs->capacity = capacity;
    rs->size = 0;
    return rs;
}

int
idxarr_push_rs(idx_array_rs *rs, void* data) {
    if (rs->size >= rs->capacity) {
        idxarr_u_char** new_rs = __idxarr_realloc_fn(rs->ptrs , (rs->capacity * 2) * sizeof(idxarr_u_char*));
        if (new_rs == NULL) {
            fprintf(stderr, "NO more allocated space\n");
            return 0;
        }
        rs->ptrs = new_rs;
        rs->capacity *= 2;
    }

    rs->ptrs[rs->size++] = data;

    return 1;

}

/* all the way to highest */
int
idxarr_push_rs_n(idx_array_rs *rs, idxarr_u_int start_ind, idxarr_u_int width,  idxarr_u_char **arr_ref) {
    if (rs->size + width > rs->capacity) {
        size_t ext_size_ = width - (rs->capacity - rs->size);
        idxarr_u_char** new_rs = __idxarr_realloc_fn(rs->ptrs , (rs->capacity + ext_size_) * sizeof(idxarr_u_char*));
        if (new_rs == NULL) {
            fprintf(stderr, "No more allocated space\n");
            return 0;
        }
        rs->ptrs = new_rs;
        rs->capacity += ext_size_;
    }

    memcpy(&rs->ptrs[rs->size], &arr_ref[start_ind], width  * sizeof(idxarr_u_char*));

    rs->size += width;

    return 1;
}

idx_array_rs*
idxarr_append_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge) {
    idxarr_push_rs_n(rs1, 0, rs2->size, rs2->ptrs);

    if (free_after_merge)
        idxarr_free_rs(rs2);

    return rs1;
}


idx_array_rs*
idxarr_rs_rm_dup_by(idx_array_rs *rs, idxarr_cmp_func cmp_func, bool free_after_merge) {
    size_t i, j, count = 0, size = rs->size;
    idx_array_rs *group_rs = idxarr_rs_create(size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < count; j++) {
            if (cmp_func(rs->ptrs + i, group_rs->ptrs + j) == 0)
                break;
        }
        if (j == count) {
            idxarr_push_rs(group_rs, rs->ptrs[i]);
            count++;
        }
    }
    if (free_after_merge && rs != NULL)
        idxarr_free_rs(rs);
    return group_rs;
}

void
idxarr_free_rs(idx_array_rs *rs) {
    if (rs) {
        if (rs->ptrs) __idxarr_free_fn(rs->ptrs);

        rs->size = 0;
        rs->capacity = 0;
        __idxarr_free_fn(rs);
    }
    rs = NULL;
}

idx_array_rs*
idxarr_union_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge) {
    idx_array_rs *merged_rs = idxarr_rs_create(__DEFAULT_RS_CAPACITY__);
    __union__(rs1, rs2, merged_rs, free_after_merge);

    return merged_rs;
}

idx_array_rs*
idxarr_intersect_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge) {
    idx_array_rs *merged_rs = idxarr_rs_create(__DEFAULT_RS_CAPACITY__);
    __intersection__(rs1, rs2, merged_rs, free_after_merge);

    return merged_rs;
}

/**
 ** ref http://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/
 **/
void
__union__(idx_array_rs *rs1, idx_array_rs *rs2, idx_array_rs *merged_rs, bool free_after_merge) {
    size_t size1 = rs1->size, size2 = rs2->size;
    size_t i = 0, j = 0;
    if ((size1 + size2) > __MIN_ON2_THRESHOLD__ ) {
        /* If more than on2 method threshold, please using sorting logic */
        qsort(rs1->ptrs, size1, sizeof(idxarr_u_char*), cmp_idx_rs_uint);
        qsort(rs2->ptrs, size2, sizeof(idxarr_u_char*), cmp_idx_rs_uint);
        idxarr_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;

        while (i < size1 && j < size2)
        {
            if (uintptrs1[i] < uintptrs2[j])
                idxarr_push_rs(merged_rs, (void*) uintptrs1[i++]);
            else if (uintptrs2[j] < uintptrs1[i])
                idxarr_push_rs(merged_rs, (void*) uintptrs2[j++]);
            else
            {
                idxarr_push_rs(merged_rs, (void*) uintptrs2[j++]);
                i++;
            }
        }

        while (i < size1)
            idxarr_push_rs(merged_rs, (void*) uintptrs1[i++]);
        while (j < size2)
            idxarr_push_rs(merged_rs, (void*) uintptrs2[j++]);


        if (free_after_merge) {
            idxarr_free_rs(rs1);
            idxarr_free_rs(rs2);
        }
    } else {
        idxarr_append_rs(merged_rs, rs1, false); // Do not free the rs1 as still need to merge
        idxarr_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;
        for (i = 0; i < size2; i++) {
            int found = 0;
            for (j = 0; j < size1; j++) {
                if (uintptrs2[i] == uintptrs1[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found)idxarr_push_rs(merged_rs, (void*) uintptrs2[i]); // if rs2 value not present in merged_rs then add it
        }

        if (free_after_merge) {
            idxarr_free_rs(rs1);
            idxarr_free_rs(rs2);
        }
    }
}

void
__intersection__(idx_array_rs *rs1, idx_array_rs *rs2, idx_array_rs *merged_rs, bool free_after_merge) {
    size_t size1 = rs1->size, size2 = rs2->size;
    size_t i = 0, j = 0;
    if ((size1 + size2) > __MIN_ON2_THRESHOLD__ ) {
        /* If more than on2 method threshold, please using sorting logic */
        qsort(rs1->ptrs, size1, sizeof(idxarr_u_char*), cmp_idx_rs_uint);
        qsort(rs2->ptrs, size2, sizeof(idxarr_u_char*), cmp_idx_rs_uint);
        idxarr_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;

        while ((i < size1) && (j < size2))
        {
            if (uintptrs1[i] < uintptrs2[j]) {
                i++;
            }
            else if (uintptrs1[i] > uintptrs2[j]) {
                j++;
            }
            else {
                idxarr_push_rs(merged_rs, (void*) uintptrs1[i++]);
                j++;
            }
        }
    } else {
        idxarr_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;
        for (i = 0; i < size1; i++) {
            for (j = 0; j < size2; j++) {
                if (uintptrs1[i] == uintptrs2[j]) {
                    idxarr_push_rs(merged_rs, (void*) uintptrs1[i]);
                    break;
                }
            }
        }
    }

    // clean the previous result
    if (free_after_merge) {
        idxarr_free_rs(rs1);
        idxarr_free_rs(rs2);
    }
}


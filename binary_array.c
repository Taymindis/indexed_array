#include "binary_array.h"

void bin_insert(bin_array_t *a, bin_u_char* node, size_t index_num);
void bin_array_push_idx_ref(bin_array_t *a, void* node);

int bin_push_rs(bin_array_rs *rs, void* data);
int bin_push_rs_n(bin_array_rs *rs, bin_u_int start_ind, bin_u_int width,  bin_u_char **arr_ref);
void __union__(bin_array_rs *rs1, bin_array_rs *rs2, bin_array_rs *merged_rs, bool free_after_merge);
void __intersection__(bin_array_rs *rs1, bin_array_rs *rs2, bin_array_rs *merged_rs, bool free_after_merge);
long basearch_index_eq_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                        register int (*cmp)(const void *key, const void *elt));
long basearch_index_gt_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                        register int (*cmp)(const void *key, const void *elt));
long basearch_index_lt_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                        register int (*cmp)(const void *key, const void *elt));



static inline int cmp_idx_rs_uint(const void * a, const void * b) {
    return (int)( (*(bin_u_int *)a) - (*(bin_u_int *)b) );
}

/*
 * This is reference of bsearch - https://github.com/torvalds/linux/blob/master/lib/bsearch.c
 * but return index, passing struct field as comparison
 */
long
basearch_index_eq_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                   register int (*cmp)(const void *key, const void *elt)) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    //  size_t ptr_size = sizeof(bin_u_char*);
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i) + offset);

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
basearch_index_gt_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                   register int (*cmp)(const void *key, const void *elt)) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i) + offset);

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
basearch_index_lt_(register const void *key, const bin_u_char **base, size_t num, register bin_u_long offset,
                   register int (*cmp)(const void *key, const void *elt)) {
    register int result = 0;
    register size_t mid = 0, i = 0;
    while (num > 0) {
        i = mid + (num >> 1);
        result = cmp(key, *(base + i) + offset);

        if (result > 0) {
            mid = i + 1;
            num--;
        }
        num >>= 1;
    }
    if (result <= 0) return i - 1;
    return i;
}

bin_array_rs*
basearch_index(const void *key, bin_array_t *a, bin_u_long offset, bin_idx_condtion cond)
{
    int i;
    bin_array_idx *arr_idx;
    // long index;
    long asize;
    bin_array_rs *arr_rs = bin_array_rs_create(__DEFAULT_RS_CAPACITY__);
    for (i = 0; i < a->num_of_index; i++) {
        if (a->_index_arr_[i].cmp_func != NULL && a->_index_arr_[i].offset == offset) {
            goto FOUND_INDEX;
        }
    }

    return arr_rs;

FOUND_INDEX:
    arr_idx = a->_index_arr_ + i ; // * sizeof(bin_array_idx); it is not void/char ptr, not need adjust by size
    long index, lt_ind, gt_ind;
    switch (cond) {
    case bin_idx_eq:
        asize = a->size;
        lt_ind = basearch_index_lt_(key, (const bin_u_char**) arr_idx->arr_ref, asize, arr_idx->offset, arr_idx->cmp_func);
        gt_ind = basearch_index_gt_(key, (const bin_u_char**) arr_idx->arr_ref, asize, arr_idx->offset, arr_idx->cmp_func);

        if ( lt_ind >= asize || gt_ind <= 0 || gt_ind - lt_ind < 2) { // means none
            break;
        }

        if (lt_ind < 0)
            lt_ind = 0;
        else lt_ind++;

        if (gt_ind >= asize)
            gt_ind = asize;

        bin_u_char **arr_refs = arr_idx->arr_ref;

        bin_push_rs_n(arr_rs, lt_ind, gt_ind - lt_ind, arr_idx->arr_ref);

        break;
    case bin_idx_gt:
        asize = a->size;
        index = basearch_index_gt_(key, (const bin_u_char**) arr_idx->arr_ref, asize, arr_idx->offset, arr_idx->cmp_func);
        if (index >= 0 && index < asize) {
            bin_u_char **arr_refs = arr_idx->arr_ref;
            bin_push_rs_n(arr_rs, index, asize - index, arr_idx->arr_ref);
        }
        break;
    case bin_idx_lt:
        asize = a->size;
        index = basearch_index_lt_(key, (const bin_u_char**) arr_idx->arr_ref, asize, arr_idx->offset, arr_idx->cmp_func);
        if (index >= 0 && index < asize) {
            bin_u_char **arr_refs = arr_idx->arr_ref;
            bin_push_rs_n(arr_rs, 0, index + 1, arr_idx->arr_ref);
        }
        break;
    default:
        break;
    }
    return arr_rs;
}


void*
basearch_direct_one(const void *key, bin_array_t *a, bin_u_long offset)
{
    int i;
    bin_array_idx *arr_idx;
    long asize, index;
    for (i = 0; i < a->num_of_index; i++) {
        if (a->_index_arr_[i].cmp_func != NULL && a->_index_arr_[i].offset == offset) {
            goto FOUND_INDEX;
        }
    }

    return NULL;

FOUND_INDEX:
    arr_idx = a->_index_arr_ + i;
    asize = a->size;
    index = basearch_index_eq_(key, (const bin_u_char**) arr_idx->arr_ref, asize, arr_idx->offset, arr_idx->cmp_func);

    if (index >= 0 && index < asize) {
        return arr_idx->arr_ref[index];
    }

    return NULL;
}

bin_u_char **
get_index_array(bin_array_t *a, bin_u_int index_num) {
    if (index_num < a->num_of_index) {
        return a->_index_arr_[index_num].arr_ref;
    }
    return NULL;
}

// Function to sort an array a[] of size 'n'
void
bin_insert(bin_array_t *a, bin_u_char* node, size_t index_num) {
    bin_array_idx *index_arr = a->_index_arr_ + index_num;// * sizeof(bin_array_idx); //it is not void ptr, not need adjust by size
    if (index_arr->cmp_func) {
        long curr_sz = a->size;
        long index_ptr = basearch_index_eq_(node + index_arr->offset, (const bin_u_char**) index_arr->arr_ref, curr_sz, index_arr->offset, index_arr->cmp_func);

        /** Memmove for overlap destination **/
        memmove(&index_arr->arr_ref[index_ptr + 1], &index_arr->arr_ref[index_ptr], (curr_sz - index_ptr)  * sizeof(bin_u_char*));

        index_arr->arr_ref[index_ptr] = node;
    }
}

bin_array_t *
bin_array_create(bin_u_int size, bin_u_int num_of_index)
{
    bin_array_t *a;

    a = malloc(sizeof(bin_array_t));
    if (a == NULL) {
        return NULL;
    }

    if (bin_array_init(a, size, num_of_index) == 0) {
        return NULL;
    }

    return a;
}

void
bin_array_clear(bin_array_t *a, free_node_fn free_node_fn) {
    int i, j;
    if (!free_node_fn) free_node_fn = free;
    for (i = 0; i < a->num_of_index; i++) {
        bin_array_idx *idx_arr = a->_index_arr_ + i;// * sizeof(bin_array_idx); it is not void ptr, not need adjust by size
        if (idx_arr->cmp_func) {
            if (i == 0) {
                for (j = 0; j < a->size; j++)
                    free_node_fn((void*) idx_arr->arr_ref[j]);
            }
        }
    }
    a->size = 0;
}

void
bin_array_destroy(bin_array_t *a, free_node_fn free_node_fn) {
    int i, j;
    if (!free_node_fn) free_node_fn = free;
    for (i = 0; i < a->num_of_index; i++) {
        bin_array_idx *idx_arr = a->_index_arr_ + i;// * sizeof(bin_array_idx); it is not void ptr, not need adjust by size
        if (idx_arr->cmp_func) {
            if (i == 0) {
                for (j = 0; j < a->size; j++)
                    free_node_fn((void*) idx_arr->arr_ref[j]);
            }
        }

        free(idx_arr->arr_ref);
    }

    free(a->_index_arr_);
    free(a);
}

int bin_add_index_(bin_array_t *a, bin_u_long offset, idx_cmp_func cmp_func) {
    if (a && a->num_of_index > 0) {
        int i;
        for (i = 0; i < a->num_of_index; i++) {
            if (a->_index_arr_[i].cmp_func == NULL) {
                a->_index_arr_[i].offset = offset;
                a->_index_arr_[i].cmp_func = cmp_func;
                return 1;
            }
        }
        goto ERROR;
    } else {
ERROR:
        perror("Running out of index");
        return 0;
    }

}


void
bin_array_push_idx_ref(bin_array_t *a, void* node) {
    size_t i;
    for (i = 0; i < a->num_of_index; i++) {
        bin_insert(a, node, i);
    }
}


int
bin_array_push(bin_array_t *a, void*  node) {
    if (a->size == a->capacity) {
        int i;
        size_t size_of_ptr = sizeof(bin_u_char*);
        for (i = 0; i < a->num_of_index; i++) {
            bin_array_idx *idx_array = a->_index_arr_ + i; //* sizeof(bin_array_idx); it is not void ptr, not need adjust by size
            bin_u_char **new_arr = realloc(idx_array->arr_ref, (a->capacity * 2) * size_of_ptr);
            if (new_arr == NULL) {
                perror("unable to rellocate more space...");
                return 0;
            }
            idx_array->arr_ref = new_arr;
        }
        a->capacity *= 2;
    }
    bin_array_push_idx_ref(a, node);
    a->size++;

    return 1;
}

int
bin_array_push_n(bin_array_t *a,  bin_u_char* node, bin_u_int num) {
    bin_u_int i;
    for (i = 0; i < num; i++) {
        bin_array_push(a, (void*) (uintptr_t)node[i]);
    }
    a->size += num;

    return 1;
}

bin_array_rs*
bin_array_rs_create(size_t capacity) {
    bin_array_rs *rs = __bin_arr_malloc_fn(sizeof(bin_array_rs));
    rs->ptrs = __bin_arr_malloc_fn(capacity * sizeof(bin_u_char*));
    rs->capacity = capacity;
    rs->size = 0;
    return rs;
}

int
bin_push_rs(bin_array_rs *rs, void* data) {
    if (rs->size >= rs->capacity) {
        bin_u_char** new_rs = __bin_arr_realloc_fn(rs->ptrs , (rs->capacity * 2) * sizeof(bin_u_char*));
        if (new_rs == NULL) {
            perror("NO more allocated space");
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
bin_push_rs_n(bin_array_rs *rs, bin_u_int start_ind, bin_u_int width,  bin_u_char **arr_ref) {
    if (rs->size + width > rs->capacity) {
        size_t ext_size_ = width - (rs->capacity - rs->size);
        bin_u_char** new_rs = __bin_arr_realloc_fn(rs->ptrs , (rs->capacity + ext_size_) * sizeof(bin_u_char*));
        if (new_rs == NULL) {
            perror("No more allocated space");
            return 0;
        }
        rs->ptrs = new_rs;
        rs->capacity += ext_size_;
    }

    memcpy(&rs->ptrs[rs->size], &arr_ref[start_ind], width  * sizeof(bin_u_char*));

    rs->size += width;

    return 1;
}

bin_array_rs*
bin_append_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge) {
    bin_push_rs_n(rs1, 0, rs2->size, rs2->ptrs);

    if (free_after_merge)
        bin_free_rs(rs2);

    return rs1;
}

void
bin_free_rs(bin_array_rs *rs) {
    if (rs) {
        if (rs->ptrs) __bin_arr_free_fn(rs->ptrs);

        rs->size = 0;
        rs->capacity = 0;
        __bin_arr_free_fn(rs);
    }
    rs = NULL;
}

bin_array_rs*
bin_union_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge) {
    bin_array_rs *merged_rs = bin_array_rs_create(__DEFAULT_RS_CAPACITY__);
    __union__(rs1, rs2, merged_rs, free_after_merge);

    return merged_rs;
}

bin_array_rs*
bin_intersect_rs(bin_array_rs *rs1, bin_array_rs *rs2, bool free_after_merge) {
    bin_array_rs *merged_rs = bin_array_rs_create(__DEFAULT_RS_CAPACITY__);
    __intersection__(rs1, rs2, merged_rs, free_after_merge);

    return merged_rs;
}

/**
 ** ref http://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/
 **/
void
__union__(bin_array_rs *rs1, bin_array_rs *rs2, bin_array_rs *merged_rs, bool free_after_merge) {
    size_t size1 = rs1->size, size2 = rs2->size;
    size_t i = 0, j = 0;
    if ((size1 + size2) > __MIN_ON2_THRESHOLD__ ) {
        /* If more than on2 method threshold, please using sorting logic */
        qsort(rs1->ptrs, size1, sizeof(bin_u_char*), cmp_idx_rs_uint);
        qsort(rs2->ptrs, size2, sizeof(bin_u_char*), cmp_idx_rs_uint);
        bin_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;

        while (i < size1 && j < size2)
        {
            if (uintptrs1[i] < uintptrs2[j])
                bin_push_rs(merged_rs, (void*) uintptrs1[i++]);
            else if (uintptrs2[j] < uintptrs1[i])
                bin_push_rs(merged_rs, (void*) uintptrs2[j++]);
            else
            {
                bin_push_rs(merged_rs, (void*) uintptrs2[j++]);
                i++;
            }
        }

        while (i < size1)
            bin_push_rs(merged_rs, (void*) uintptrs1[i++]);
        while (j < size2)
            bin_push_rs(merged_rs, (void*) uintptrs2[j++]);


        if (free_after_merge) {
            bin_free_rs(rs1);
            bin_free_rs(rs2);
        }
    } else {
        bin_append_rs(merged_rs, rs1, false); // Do not free the rs1 as still need to merge
        bin_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;
        for (i = 0; i < size2; i++) {
            int found = 0;
            for (j = 0; j < size1; j++) {
                if (uintptrs2[i] == uintptrs1[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found)bin_push_rs(merged_rs, (void*) uintptrs2[i]); // if rs2 value not present in merged_rs then add it
        }

        if (free_after_merge) {
            bin_free_rs(rs1);
            bin_free_rs(rs2);
        }
    }
}

void
__intersection__(bin_array_rs *rs1, bin_array_rs *rs2, bin_array_rs *merged_rs, bool free_after_merge) {
    size_t size1 = rs1->size, size2 = rs2->size;
    size_t i = 0, j = 0;
    if ((size1 + size2) > __MIN_ON2_THRESHOLD__ ) {
        /* If more than on2 method threshold, please using sorting logic */
        qsort(rs1->ptrs, size1, sizeof(bin_u_char*), cmp_idx_rs_uint);
        qsort(rs2->ptrs, size2, sizeof(bin_u_char*), cmp_idx_rs_uint);
        bin_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;

        while ((i < size1) && (j < size2))
        {
            if (uintptrs1[i] < uintptrs2[j]) {
                i++;
            }
            else if (uintptrs1[i] > uintptrs2[j]) {
                j++;
            }
            else {
                bin_push_rs(merged_rs, (void*) uintptrs1[i++]);
                j++;
            }
        }
    } else {
        bin_u_int *uintptrs1 = rs1->_uintptrs_, *uintptrs2 = rs2->_uintptrs_;
        for (i = 0; i < size1; i++) {
            for (j = 0; j < size2; j++) {
                if (uintptrs1[i] == uintptrs2[j]) {
                    bin_push_rs(merged_rs, (void*) uintptrs1[i]);
                    break;
                }
            }
        }
    }

    // clean the previous result
    if (free_after_merge) {
        bin_free_rs(rs1);
        bin_free_rs(rs2);
    }
}


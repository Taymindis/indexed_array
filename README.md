# binary_array
Binary Array is a struct array for your to add index for struct field member and it can be binary search
When the field been indexed, it will auto sorted while inserting new node, please make sure the field has value pre-allocated memory.

## Noted
Every struct have to be pre-allocated and value setted before push into binary array.

### Target Project
- this lib mostly target for big datatable searching on cache.

### Requirement
- gcc/clang/llvm-gcc


### Sample of Testing
- goto root directory
- gcc binary_array.c main.c -otest
- ./test

### API document
###### idx_array_t *idxarr_create(idxarr_u_int size, idxarr_u_int num_of_index)
* arg0=The size of array
* arg1=number of index will be created
* return idxarr_array (root_array)

###### int idxarr_add_index(idx_array_t *a, struct_name, field_member, cmp_func);
* arg0=root_array
* arg1=the struct name
* arg2=the field name
* arg3=the cmp_function(built in given, please see demo for details)
* return 1 if successful, 0 if fail created

###### int idxarr_array_push(idx_array_t *a, void* node);
* arg0=root array
* arg1=pre-allocated node
* return 1 if successful, 0 if fail pushed

###### int idxarr_array_push_n(idx_array_t *a,  idxarr_u_char* node, idxarr_u_int num)
* it is push more than 1 elements at one time by given a array node.
* arg0=root array
* arg1=pre-allocated node array
* arg2=number of element does node array have
* return 1 if successful, 0 if fail pushed 

###### void* idxarr_search_one(idx_array_t *a, struct_name, field_name, key)
* it is binary search if key matched, it direct return the pointer which point to the node(do not free the node).
* arg0=root array
* arg1=struct name
* arg2=field name
* arg3=key for search
* return the pointer pointed to the node 


###### idx_array_rs* idxarr_search_eq(idx_array_t *a, struct_name, field_name, key)
* it is binary search if every node matched to the key
* arg0=root array
* arg1=struct name
* arg2=field name
* arg3=key for search
* return the result set(idx_array_rs) and need to be free after use, see demo for details 


###### idx_array_rs* idxarr_search_lt(idx_array_t *a, struct_name, field_name, key)
* it is binary search if every node lower than the key
* arg0=root array
* arg1=struct name
* arg2=field name
* arg3=key for search
* return the result set(idx_array_rs) and need to be free after use, see demo for details 


###### idx_array_rs* idxarr_search_gt(idx_array_t *a, struct_name, field_name, key)
* it is binary search if every node greater than the key
* arg0=root array
* arg1=struct name
* arg2=field name
* arg3=key for search
* return the result set(idx_array_rs) and need to be free after use, see demo for details 


###### idx_array_rs* idxarr_search_multi_eq2(idx_array_t *a, struct_name, field_name, key1, key2)
* it is binary search more than 1 eq, similar to IN query in sql, it so far can up to 10 key search at the same time.
* arg0=root array
* arg1=struct name
* arg2=field name
* arg3=key1 for search
* arg4=key2 for search
* return the result set(idx_array_rs) and need to be free after use, see demo for details 

###### idx_array_rs* idxarr_union_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
* it is union rs1 and rs2, and return the merged result.
* arg0=result set 1
* arg1=result set 2
* arg2=boolean to tell whether want to free rs1 and rs2 after new merged result created
* return the result set(idx_array_rs) and need to be free after use, see demo for details 


###### idx_array_rs* idxarr_intersect_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
* it is finding the intersection value between rs1 and rs2, and return the merged result.
* arg0=result set 1
* arg1=result set 2
* arg2=boolean to tell whether want to free rs1 and rs2 after new merged result created
* return the result set(idx_array_rs) and need to be free after use, see demo for details 


###### idx_array_rs* idxarr_append_rs(idx_array_rs *rs1, idx_array_rs *rs2, bool free_after_merge);
* it is just appending the result, .
* arg0=result set 1
* arg1=result set 2
* arg2=boolean to tell whether want to free rs2 after new merged result created
* return the result set(idx_array_rs) and need to be free after use, see demo for details 

###### void idxarr_sort_rs_by(idx_array_rs *rs, cmp_func) 
* you can resort the result with your customized compare function, .
* arg0=result set 1
* arg1=result set 2
* arg2=boolean to tell whether want to free rs2 after new merged result created
* return nothing

###### void idxarr_free_rs(idx_array_rs *rs)
* free the result set
* arg0=result set

###### void idxarr_array_clear(idx_array_t *a, free_node_fn free_node_fn)
* it is clear all the value but still remaining the array schema and indexed, for the purpose of reload/reset the data.
* arg0=root array
* arg1=free node function, to create a free node function to free the node when destroy array
* return nothing

###### void idxarr_array_destroy(idx_array_t *a, free_node_fn free_node_fn)
* it is destrying the root array
* arg0=root array
* arg1=free node function, to create a free node function to free the node when destroy array
* return nothing


###### void idxarr_array_safety_swap(idx_array_t **curr, idx_array_t *new_a, free_node_fn free_node_fn, unsigned int buffer_time_mic_sec)
* It is making hazard ptr to swap new array into curr array safety, 
* arg0=curr array dereference
* arg1=new array to replace curr array
* arg2=free node function, to create a free node function to free the node when destroy old array
* arg3=the buffer time for old array going to be freed, the purpose for current thread may still using old array with specific of time
* return nothing


### What is pre-allocated value in the readme
* it means the value must allocate memory before push into the array. The buffer will keeping the same value until you destroy the array, it will be freed

### Enjoy your Binary Data searching!!
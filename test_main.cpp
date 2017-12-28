#ifdef __cplusplus
extern "C" {
#endif

#define idxarr_assert(message, test) do { \
if (!(test)) { \
printf("TEST FAILED at line numer %d, %s\n", __LINE__, message);\
return 0; \
} \
} while (0)

#define idxarr_run_test(test) do { \
idxarr_tests_run++; \
if(!test()) return 0;\
} while (0)

extern int idxarr_tests_run;

#include <time.h>
#include <string.h>
#include "indexed_array.h"
#include <unistd.h>


/****
g++ -std=c++11 indexed_array.c test_main.cpp
valgrind ./a.out // if you have valgrind
or
clang++ -std=c++11 indexed_array.c test_main.cpp
drmemory / valgrind to run
*
*/
typedef struct {
    int val;
    long val_l;
    float val_f;
    char *val_cstr;
    char val_cstr2[10];
} my_node;

int cmp_group_func (const void *a, const void *b) {
    return strcmp((*(my_node**)a)->val_cstr, (*(my_node**)b)->val_cstr);
}

void free_my_node(void *a) {
    free(((my_node*)a)->val_cstr);
    free(a);
}


int idxarr_tests_run = 0;

/***Uniq variable***/
idx_array_t *my_indexed_arr;


static int setup_test() {
    // at first 50 quantity only
    int n = 50;

    my_indexed_arr = idxarr_create(n, 5);


    idxarr_assert("First Start indexed array capacity should be 50", my_indexed_arr->capacity == 50);


    if (idxarr_add_float_index(my_indexed_arr, my_node, val_f)
            && idxarr_add_int_index(my_indexed_arr, my_node, val)
            && idxarr_add_heap_str_index(my_indexed_arr, my_node, val_cstr)
            && idxarr_add_stack_str_index(my_indexed_arr, my_node, val_cstr2)
            && idxarr_add_long_index(my_indexed_arr, my_node, val_l)
       ) {
        int i;
        for (i = 0; i < 100; i++) {
            my_node *s =(my_node*) malloc(sizeof(my_node));
            if (i < 10) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Jack Prabas");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 20) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Abbar");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 30) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("BETTY");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 40) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Eason");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 50) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Cat");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 60) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Genus Gan");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 70) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Heat Jack");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 80) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Ian Cheng@");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 90) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Finn");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Denny");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            idxarr_push(my_indexed_arr, s);
        }
        idxarr_assert("indexed array size should become to 100", my_indexed_arr->size == 100);

    } else {
        idxarr_assert("indexed array indexing failed", 0);
    }
    return 1;
}

static int test_value_had_indexed() {
    my_node **ss = (my_node**)idxarr_get_index_array(my_indexed_arr, 2); // val_cstr indexed
    int i;
    for (i = 0; i < 10; i++) {
        idxarr_assert("Abbar should have val 10-19", ss[i]->val >= 10 && ss[i]->val < 20);
    }


    for (i = 10; i < 20; i++) {
        idxarr_assert("BETTY should have val 20-29", ss[i]->val >= 20 && ss[i]->val < 30);
    }

    for (i = 20; i < 30; i++) {
        idxarr_assert("Cat should have val 40-49", ss[i]->val >= 40 && ss[i]->val < 50);
    }


    return 1;
}

static int test_search_algorithm() {
    int search_key1 = 10;
    int search_key2 = 20;
    int search_key3 = 30;
    int search_key4 = 40;
    int search_key5 = 50;
    int search_key6 = 60;
    int search_key7 = 70;
    int search_key8 = 80;
    int search_key9 = 90;
    int search_key10 = 200;
    char* search_key5c = "Abbar";
    char* search_key6c = "Cat";
    long search_key12 = 12;


    idx_array_rs *rs = idxarr_search_multi_eq10(my_indexed_arr, my_node, val, &search_key1,
                       &search_key2, &search_key3, &search_key4, &search_key5, &search_key6, &search_key7,
                       &search_key8, &search_key9 , &search_key10 );

    idxarr_assert("result size should be 9", rs->size == 9);

    idxarr_free_rs(rs);


    rs = idxarr_search_gt(my_indexed_arr, my_node, val_l, &search_key12);


    idxarr_assert("result size should be 87", rs->size == 87);

    idxarr_free_rs(rs);

    rs = idxarr_search_lt(my_indexed_arr, my_node, val_l, &search_key12);

    idxarr_assert("result size should be 12", rs->size == 12);

    idxarr_free_rs(rs);

    rs = idxarr_search_eq(my_indexed_arr, my_node, val_cstr, search_key5c);

    idxarr_assert("result size should be 10", rs->size == 10);

    idxarr_free_rs(rs);

    idx_array_rs *rs1 = idxarr_search_eq(my_indexed_arr, my_node, val_cstr, search_key5c);
    idx_array_rs *rs2 = idxarr_search_eq(my_indexed_arr, my_node, val_cstr2, search_key6c);

    idx_array_rs *rs10 = idxarr_intersect_rs(rs1, rs2, false); // Join and false mean remain rs1 and rs2, true means free the result

    idxarr_assert("result size should be 0 as join is not match", rs10->size == 0);

    idxarr_free_rs(rs10);

    rs10 = idxarr_union_rs(rs1, rs2, false); // Join

    idxarr_assert("result size should be 20 as union ", rs10->size == 20);

    idxarr_free_rs(rs10);


    // Join more
    rs10 = idxarr_intersect_rs(rs1, idxarr_union_rs(rs1, rs2, false), true);

    idxarr_assert("result size should be 10 as join and the union ", rs10->size == 10);

    // idxarr_free_rs(rs1);  // rs1 is already free at above
    idxarr_free_rs(rs2);
    idxarr_free_rs(rs10);


    // Test String start with
    char* search_keyStartWith = "A";
    idx_array_rs *rs111 = idxarr_search_str_start_with(my_indexed_arr, my_node, val_cstr, search_keyStartWith);

    idxarr_assert("result size should be 10 as Start with A only have 10 result ", rs111->size == 10);
    int i;

    for (i = 0; i < rs111->size; i++) {
        idxarr_assert("result start with A should be Abbar only ", strcmp(((my_node*) rs111->ptrs[i])->val_cstr, "Abbar") == 0);
    }

    idxarr_free_rs(rs111);

    search_keyStartWith = "F";

    rs111 = idxarr_search_str_start_with(my_indexed_arr, my_node, val_cstr2, search_keyStartWith);

    idxarr_assert("result size should be 10 as Start with F only have 10 result ", rs111->size == 10);

    for (i = 0; i < rs111->size; i++) {
        idxarr_assert("result start with F should be Finn only ", strcmp(((my_node*) rs111->ptrs[i])->val_cstr2, "Finn") == 0);
    }

    idxarr_free_rs(rs111);

    return 1;
}

static int test_reload_data() {
    // at first 50 quantity only
    int n = 50;

    idx_array_t *my_new_indexed_arr = idxarr_create(n, 5);

    idxarr_assert("new indexed array capacity should be 50", my_new_indexed_arr->capacity == 50);


    if (idxarr_add_float_index(my_new_indexed_arr, my_node, val_f)
            && idxarr_add_int_index(my_new_indexed_arr, my_node, val)
            && idxarr_add_heap_str_index(my_new_indexed_arr, my_node, val_cstr)
            && idxarr_add_stack_str_index(my_new_indexed_arr, my_node, val_cstr2)
            && idxarr_add_long_index(my_new_indexed_arr, my_node, val_l)
       ) {
        int i;
        for (i = 0; i < 50; i++) {
            my_node *s =(my_node*) malloc(sizeof(my_node));
            if (i < 10) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Jack Prabas");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 20) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Abbar");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 30) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("BETTY");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else if (i < 40) {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Eason");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            else {
                s->val = i;
                s->val_l = i * 1L;
                s->val_cstr = strdup("Cat");
                memcpy(s->val_cstr2,  s->val_cstr, strlen(s->val_cstr) + 1);
                s->val_f = i * 1.0f;
            }
            idxarr_push(my_new_indexed_arr, s);
        }
    } else {
        idxarr_assert("new indexed array indexing failed", 0);
    }

    unsigned int buffer_milisecs_to_purge_old_array = 500;

    idxarr_safety_swap(&my_indexed_arr, my_new_indexed_arr, free_my_node, buffer_milisecs_to_purge_old_array);

    idxarr_assert("reloaded indexed array size should become to 50", my_indexed_arr->size == 50);


    return 1;
}


static int all_tests() {

    idxarr_run_test(setup_test);
    idxarr_run_test(test_value_had_indexed);
    idxarr_run_test(test_search_algorithm);
    idxarr_run_test(test_reload_data);

    idxarr_destroy(my_indexed_arr, free_my_node);

    // Use Valgrind compile

    return 1;
}

int main(int argc, char **argv) {
    int test_status = all_tests();
    if (test_status) {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", idxarr_tests_run);

    return test_status;
}

#ifdef __cplusplus
}
#endif

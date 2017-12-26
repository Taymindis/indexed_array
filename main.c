/***MAIN TEST***/

#include <time.h>
#include <string.h>
#include "indexed_array.h"
#include <unistd.h>

// int values[] = { 3, 5, 1, 1, 5, 8, 4, 2, 99, 7 };


typedef struct {
    int val;
    long val_l;
    float val_f;
    char *val_cstr;
    char val_cstr2[10];
} my_node;


int __fn__ (const void *a, const void *b) {
    return (*(my_node**)a)->val - (*(my_node**)b)->val;
}

int cmp_group_func (const void *a, const void *b) {
    // printf("comparing %s and %s \n", (*(my_node**)a)->val_cstr,(*(my_node**)b)->val_cstr);
    return strcmp((*(my_node**)a)->val_cstr, (*(my_node**)b)->val_cstr);
}

void free_my_node(void *a) {
    // printf("%s\n", "free");
    free(((my_node*)a)->val_cstr);
    free(a);
}

#define dump_rs(_idxarr_rs_, __struct_type__, __field__, fmt)({\
if (_idxarr_rs_ != NULL) {\
int i;\
for (i = 0; i < _idxarr_rs_->size; i++) {\
printf(fmt, (((__struct_type__*) _idxarr_rs_->ptrs[i])->__field__));\
}\
}\
})

#define dump_rs_and_free(_idxarr_rs_, __struct_type__, __field__, fmt)({\
if (_idxarr_rs_ != NULL) {\
int i;\
for (i = 0; i < _idxarr_rs_->size; i++) {\
printf(fmt, (((__struct_type__*) _idxarr_rs_->ptrs[i])->__field__));\
}\
idxarr_free_rs(_idxarr_rs_);\
}\
})

long sorted_number[] = {
    12, 38, 45, 48, 57, 72, 84, 91, 110, 116, 119, 125, 133, 135, 151, 168, 172, 188, 190, 218,
    226, 230, 245, 283, 292, 300, 318, 319, 324, 332, 374, 395, 406, 410, 423, 441, 449, 454, 457, 463,
    475, 477, 509, 515, 519, 520, 523, 529, 533, 545, 553, 576, 580, 593, 596, 598, 602, 603, 609, 621,
    639, 645, 654, 656, 658, 670, 689, 730, 731, 735, 744, 750, 772, 779, 803, 809, 821, 851, 854, 858,
    862, 863, 864, 870, 871, 889, 890, 901, 907, 908, 922, 934, 940, 949, 952, 969, 970, 973, 975, 989,
    1001, 1803, 1809, 1821, 1851, 1854, 1858, 1862, 1863, 1864, 1870, 1871, 1889, 1890, 1901, 1907, 1908, 1922, 1934, 1940
};

int main() {

    int n = 50;

    idx_array_t *my_indexed_arr = idxarr_create(n, 5);
    if (idxarr_add_float_index(my_indexed_arr, my_node, val_f)
            && idxarr_add_int_index(my_indexed_arr, my_node, val)
            && idxarr_add_heap_str_index(my_indexed_arr, my_node, val_cstr)
            && idxarr_add_stack_str_index(my_indexed_arr, my_node, val_cstr2)
            && idxarr_add_long_index(my_indexed_arr, my_node, val_l)
       ) {
        int i;
        for (i = 0; i < 110; i++) {
            my_node *s = malloc(sizeof(my_node));
            s->val_l = sorted_number[i];
            if (i < 1) {
                s->val = 5;//i + i + i ;//values[i];
                s->val_cstr = strdup("JOHN");
                strcpy(s->val_cstr2, "HEBE");
                s->val_f = 5.9f;
            }
            else if (i < 20) {
                s->val = 20;//i + i + i ;//values[i];
                s->val_cstr = strdup("BETTY");
                strcpy(s->val_cstr2, "Vivi");

                s->val_f = 40.9f;
            }
            else if (i < 30) {
                s->val = 30;//i + i + i ;//values[i];
                s->val_cstr = strdup("RALF Ken");
                strcpy(s->val_cstr2, "WAWA");

                s->val_f = 40.9f;
            }
            else if (i < 40) {
                s->val = 40;//i + i + i ;//values[i];
                s->val_cstr = strdup("Kent");
                strcpy(s->val_cstr2, "Casy");
                s->val_f = 40.9f;
            }
            else if (i < 50) {
                s->val = 50;//i + i + i ;//values[i];
                s->val_cstr = strdup("LES");
                strcpy(s->val_cstr2, "Kaka");
                s->val_f = 50.9f;
            }
            else if (i < 60) {
                s->val = 60;//i + i + i ;//values[i];
                s->val_cstr = strdup("AB JIM");
                strcpy(s->val_cstr2, "Nano Pin");
                s->val_f = 60.9f;
            }
            else if (i < 70) {
                s->val = 70;//i + i + i ;//values[i];
                s->val_cstr = strdup("Abbar");
                strcpy(s->val_cstr2, "Rachel");
                s->val_f = 70.9f;
            }
            else if (i < 80) {
                s->val = 80;//i + i + i ;//values[i];
                s->val_cstr = strdup("Shawn");
                strcpy(s->val_cstr2, "Nina");
                s->val_f = 80.9f;
            }
            else if (i < 90) {
                s->val = 90;//i + i + i ;//values[i];
                s->val_cstr = strdup("SEAN");
                strcpy(s->val_cstr2, "Ember");
                s->val_f = 90.9f;
            }
            else if (i < 95) {
                s->val = 100;
                s->val_cstr = strdup("Bob");
                strcpy(s->val_cstr2, "Abby");
                s->val_f = 109.9f;
            } else {
                s->val = 111;
                s->val_cstr = strdup("GENUS");
                strcpy(s->val_cstr2, "Gigi");
                s->val_f = 119.9f;
            }
            idxarr_array_push(my_indexed_arr, s);
        }

        printf("%s\n", "Simply get a list of ordered index");
        my_node **ss = (my_node**)idxarr_get_index_array(my_indexed_arr, 2);
        for (i = 0; i < my_indexed_arr->size; i++) {
            printf("Sequence index is %s\n", ss[i]->val_cstr);
        }

        int search_key = 41;
        int search_key2 = 50;
        int search_key3 = 91;
        int search_key4 = 70;
        int search_key5 = 81;
        int search_key6 = 90;
        int search_key7 = 100;
        int search_key8 = 111;
        int search_key9 = 20;
        int search_key10 = 5;
        // float search_key4f = 19.9f;
        char* search_key5c = "DDD";
        long search_key1l = 1864;
        long search_key12 = 12;
        clock_t t;
        t = clock();
        for (i = 0; i <
                1//00
                ; i++) {
            idx_array_rs *rs = idxarr_search_multi_eq10(my_indexed_arr, my_node, val, &search_key,
                                                    &search_key2, &search_key3, &search_key4, &search_key5, &search_key6, &search_key7,
                                                    &search_key8, &search_key9 , &search_key10 );
            dump_rs(rs, my_node, val, "%d\n");
            idxarr_free_rs(rs);
        }
        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
        printf("fun() took %f seconds to execute \n", time_taken);
        idx_array_rs *rs4 = idxarr_search_gt(my_indexed_arr, my_node, val_l, &search_key12);
        printf("%s\n", "printing before remove duplicate");
        dump_rs(rs4, my_node, val_cstr, "%s\n");
        printf("%s\n\n\n", "End printing before remove duplicate");
        idx_array_rs *rs10 = idxarr_rs_rm_dup_by(rs4, cmp_group_func, true);
        printf("%s\n", "printing after remove duplicate");


        printf("%s\n", "Dump rs10 without Sorting string");
        dump_rs(rs10, my_node, val_cstr, "%s\n");

        printf("%s\n", "Dump rs10 With Sorting String");
        idxarr_sort_rs(rs10, my_node, val_cstr, 1);
        dump_rs(rs10, my_node, val_cstr, "%s\n");


        printf("%s\n", "Dump rs10 With Sorting String Desc");
        idxarr_sort_rs_desc(rs10, my_node, val_cstr, 1);
        dump_rs_and_free(rs10, my_node, val_cstr, "%s\n");

        printf("%s\n\n\n", "End printing after remove duplicate");



        idx_array_rs *rs1 = idxarr_search_eq(my_indexed_arr, my_node, val_cstr, search_key5c);
        idx_array_rs *rs3 = idxarr_search_lt(my_indexed_arr, my_node, val, &search_key3);
        rs10 = idxarr_intersect_rs(rs1, rs3, false);
        dump_rs_and_free(rs10, my_node, val_cstr, "%s\n");

        rs10 = idxarr_union_rs(rs1, rs3, true);
        dump_rs_and_free(rs10, my_node, val_cstr, "%s\n");

        printf("%s\n", "Direct search pointer location...");

        //     /* Do not free the result if you are using search_one, it will be free when you destroy the array */
        void *found = idxarr_search_one(my_indexed_arr, my_node, val_l, &search_key1l);

        if (found != NULL) {
            printf("%s\n", "Found");
            my_node* found_node = (my_node*)found;
            printf("%ld\n", found_node->val_l);
            printf("%s\n", found_node->val_cstr);
            printf("%d\n", found_node->val);
            printf("%f\n", found_node->val_f);
        }

        //     // Clear the current data set
        idxarr_array_clear(my_indexed_arr, free_my_node);
        printf("Now the array value bean cleared and size is %zu, reseting the data\n", my_indexed_arr->size);

        for (i = 0; i < 110; i++) {
            my_node *s = malloc(sizeof(my_node));
            s->val_l = sorted_number[i];
            if (i < 1) {
                s->val = 5;//i + i + i ;//values[i];
                s->val_cstr = strdup("JOHN");
                strcpy(s->val_cstr2, "HEBE");
                s->val_f = 5.9f;
            }
            else if (i < 20) {
                s->val = 20;//i + i + i ;//values[i];
                s->val_cstr = strdup("BETTY");
                strcpy(s->val_cstr2, "Vivi");

                s->val_f = 40.9f;
            }
            else if (i < 30) {
                s->val = 30;//i + i + i ;//values[i];
                s->val_cstr = strdup("RALF Ken");
                strcpy(s->val_cstr2, "WAWA");

                s->val_f = 40.9f;
            }
            else if (i < 40) {
                s->val = 40;//i + i + i ;//values[i];
                s->val_cstr = strdup("Kent");
                strcpy(s->val_cstr2, "Casy");
                s->val_f = 40.9f;
            }
            else if (i < 50) {
                s->val = 50;//i + i + i ;//values[i];
                s->val_cstr = strdup("LES");
                strcpy(s->val_cstr2, "Kaka");
                s->val_f = 50.9f;
            }
            else if (i < 60) {
                s->val = 60;//i + i + i ;//values[i];
                s->val_cstr = strdup("AB JIM");
                strcpy(s->val_cstr2, "Nano Pin");
                s->val_f = 60.9f;
            }
            else if (i < 70) {
                s->val = 70;//i + i + i ;//values[i];
                s->val_cstr = strdup("Abbar");
                strcpy(s->val_cstr2, "Rachel");
                s->val_f = 70.9f;
            }
            else if (i < 80) {
                s->val = 80;//i + i + i ;//values[i];
                s->val_cstr = strdup("Shawn");
                strcpy(s->val_cstr2, "Nina");
                s->val_f = 80.9f;
            }
            else if (i < 90) {
                s->val = 90;//i + i + i ;//values[i];
                s->val_cstr = strdup("SEAN");
                strcpy(s->val_cstr2, "Ember");
                s->val_f = 90.9f;
            }
            else if (i < 95) {
                s->val = 100;
                s->val_cstr = strdup("Bob");
                strcpy(s->val_cstr2, "Abby");
                s->val_f = 109.9f;
            } else {
                s->val = 111;
                s->val_cstr = strdup("GENUS");
                strcpy(s->val_cstr2, "Gigi");
                s->val_f = 119.9f;
            }
            idxarr_array_push(my_indexed_arr, s);
        }
        found = idxarr_search_one(my_indexed_arr, my_node, val_l, &search_key1l);

        char* search_key111c = "TTT";

        idx_array_rs *rsxx = idxarr_search_eq(my_indexed_arr, my_node, val_cstr2, search_key111c);

        dump_rs_and_free(rsxx, my_node, val_cstr2, "%s\n");

        if (found != NULL) {
            printf("%s\n", "Found After data reset again");
            my_node* found_node = (my_node*)found;
            printf("%ld\n", found_node->val_l);
            printf("%s\n", found_node->val_cstr);
            printf("%d\n", found_node->val);
            printf("%f\n", found_node->val_f);
        }


        // /****Safety SWAPPING ARRAY for MULTITHREADING PURPOSE*****/
        idx_array_t *new_ba = idxarr_create(n, 5);
        if (idxarr_add_float_index(new_ba, my_node, val_f)
                && idxarr_add_int_index(new_ba, my_node, val)
                && idxarr_add_heap_str_index(new_ba, my_node, val_cstr)
                && idxarr_add_long_index(new_ba, my_node, val_l)
           ) {
            int i;
            for (i = 0; i < 110; i++) {
                my_node *s = malloc(sizeof(my_node));
                s->val_l = sorted_number[i];
                if (i < 1) {
                    s->val = 5;//i + i + i ;//values[i];
                    s->val_cstr = strdup("JOHN");
                    strcpy(s->val_cstr2, "HEBE");
                    s->val_f = 5.9f;
                }
                else if (i < 20) {
                    s->val = 20;//i + i + i ;//values[i];
                    s->val_cstr = strdup("BETTY");
                    strcpy(s->val_cstr2, "Vivi");

                    s->val_f = 40.9f;
                }
                else if (i < 30) {
                    s->val = 30;//i + i + i ;//values[i];
                    s->val_cstr = strdup("RALF Ken");
                    strcpy(s->val_cstr2, "WAWA");

                    s->val_f = 40.9f;
                }
                else if (i < 40) {
                    s->val = 40;//i + i + i ;//values[i];
                    s->val_cstr = strdup("Kent");
                    strcpy(s->val_cstr2, "Casy");
                    s->val_f = 40.9f;
                }
                else if (i < 50) {
                    s->val = 50;//i + i + i ;//values[i];
                    s->val_cstr = strdup("LES");
                    strcpy(s->val_cstr2, "Kaka");
                    s->val_f = 50.9f;
                }
                else if (i < 60) {
                    s->val = 60;//i + i + i ;//values[i];
                    s->val_cstr = strdup("AB JIM");
                    strcpy(s->val_cstr2, "Nano Pin");
                    s->val_f = 60.9f;
                }
                else if (i < 70) {
                    s->val = 70;//i + i + i ;//values[i];
                    s->val_cstr = strdup("Abbar");
                    strcpy(s->val_cstr2, "Rachel");
                    s->val_f = 70.9f;
                }
                else if (i < 80) {
                    s->val = 80;//i + i + i ;//values[i];
                    s->val_cstr = strdup("Shawn");
                    strcpy(s->val_cstr2, "Nina");
                    s->val_f = 80.9f;
                }
                else if (i < 90) {
                    s->val = 90;//i + i + i ;//values[i];
                    s->val_cstr = strdup("SEAN");
                    strcpy(s->val_cstr2, "Ember");
                    s->val_f = 90.9f;
                }
                else if (i < 95) {
                    s->val = 100;
                    s->val_cstr = strdup("Bob");
                    strcpy(s->val_cstr2, "Abby");
                    s->val_f = 109.9f;
                } else {
                    s->val = 111;
                    s->val_cstr = strdup("GENUS");
                    strcpy(s->val_cstr2, "Gigi");
                    s->val_f = 119.9f;
                }
                idxarr_array_push(new_ba, s);
            }
        }
        unsigned int buffer_milisecs_to_purge_old_array = 500;
        printf("%s\n", "Proceed Array Safety Swapping");
        idxarr_array_safety_swap(&my_indexed_arr, new_ba, free_my_node, buffer_milisecs_to_purge_old_array);

        char* search_keyStartWith = "S";
        idx_array_rs *rs111 = idxarr_search_str_start_with(my_indexed_arr, my_node, val_cstr, search_keyStartWith);
        dump_rs_and_free(rs111, my_node, val_cstr, "%s\n");
    }
    if (my_indexed_arr != NULL)
        idxarr_array_destroy(my_indexed_arr, free_my_node);


    return (0);
}

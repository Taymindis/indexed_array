/***MAIN TEST***/

#include <time.h>
#include <string.h>
#include "binary_array.h"

// int values[] = { 3, 5, 1, 1, 5, 8, 4, 2, 99, 7 };


typedef struct {
    int val;
    long val_l;
    float val_f;
    char *val_cstr;
    char val_cstr2[10];
} my_node;

// int (*sorter)(const void*, const void*) = ({
int __fn__ (const void *a, const void *b) {
    return (*(my_node**)a)->val - (*(my_node**)b)->val;
}
// __fn__;
// });

void free_my_node(void *a) {
    printf("%s\n", "free");
    free(((my_node*)a)->val_cstr);
    free(a);
}

void dump_rs(bin_array_rs *rs) {
    if (rs != NULL) {
        int i;
        for (i = 0; i < rs->size; i++) {
            printf("%d\n", ((my_node*) rs->ptrs[i])->val);
        }
    }
}

void dump_rs_and_free(bin_array_rs *rs) {
    if (rs != NULL) {
        int i;
        for (i = 0; i < rs->size; i++) {
            printf("%d - %ld\n", i, ((my_node*) rs->ptrs[i])->val_l);
        }

        bin_free_rs(rs);
    }
}


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

    bin_array_t *bin_a_t = bin_array_create(n, 5);
    if (bin_add_index(bin_a_t, my_node, val_f, __def_float_sorted_cmp_func__)
            && bin_add_index(bin_a_t, my_node, val, __def_int_sorted_cmp_func__)
            && bin_add_index(bin_a_t, my_node, val_cstr, __def_cstr_sorted_cmp_func__)
            && bin_add_index(bin_a_t, my_node, val_cstr2, __def_cstr2_sorted_cmp_func__)
            && bin_add_index(bin_a_t, my_node, val_l, __def_long_sorted_cmp_func__)
       ) {

        int i;
        for (i = 0; i < 110; i++) {
            my_node *s = malloc(sizeof(my_node));
            s->val_l = sorted_number[i];
            if (i < 1) {
                s->val = 5;//i + i + i ;//values[i];
                s->val_cstr = strdup("ABC");                
                strcpy(s->val_cstr2, "BBB");
                s->val_f = 5.9f;
            }
            else if (i < 20) {
                s->val = 20;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");
                strcpy(s->val_cstr2, "XXX");

                s->val_f = 40.9f;
            }
            else if (i < 30) {
                s->val = 30;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");                
                strcpy(s->val_cstr2, "WWW");

                s->val_f = 40.9f;
            }
            else if (i < 40) {
                s->val = 40;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");                
                strcpy(s->val_cstr2, "TTT");
                s->val_f = 40.9f;
            }
            else if (i < 50) {
                s->val = 50;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "MMM");
                s->val_f = 50.9f;
            }
            else if (i < 60) {
                s->val = 60;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "NNN");
                s->val_f = 60.9f;
            }
            else if (i < 70) {
                s->val = 70;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "RRRR");
                s->val_f = 70.9f;
            }
            else if (i < 80) {
                s->val = 80;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "ERR");
                s->val_f = 80.9f;
            }
            else if (i < 90) {
                s->val = 90;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "VZX");
                s->val_f = 90.9f;
            }
            else if (i < 95) {
                s->val = 100;
                s->val_cstr = strdup("ZUZ");                
                strcpy(s->val_cstr2, "ZZE");
                s->val_f = 109.9f;
            } else {
                s->val = 111;
                s->val_cstr = strdup("ZUZ");                
                strcpy(s->val_cstr2, "ZUU");
                s->val_f = 119.9f;
            }
            bin_array_push(bin_a_t, s);
        }
        // my_node **ss = (my_node**)get_index_array(bin_a_t, 2);
        // for (i = 0; i < 100; i++) {
        //     printf("Sequence index is %s\n", ss[i]->val_cstr);
        // }
        //    my_node node;
        //    node.val = 5;
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
        float search_key4f = 19.9f;
        char* search_key5c = "DDD";
        long search_key1l = 1864;
        clock_t t;
        t = clock();
        for (i = 0; i <
                1//0000000
                ; i++) {
            bin_array_rs *rs = ba_search_multi_eq10(bin_a_t, my_node, val, &search_key,
                                                    &search_key2, &search_key3, &search_key4, &search_key5, &search_key6, &search_key7,
                                                    &search_key8, &search_key9 , &search_key10 );

//            if (rs != NULL) {
//                int i;
            // dump_rs_and_free(rs);
            bin_free_rs(rs);

//            }

        }
        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

        printf("fun() took %f seconds to execute \n", time_taken);

        bin_array_rs *rs1 = ba_search_eq(bin_a_t, my_node, val_cstr, &search_key5c);
        bin_array_rs *rs3 = ba_search_lt(bin_a_t, my_node, val, &search_key3);

        bin_array_rs *rs10;
        rs10 = bin_intersect_rs(rs1, rs3, false);
        dump_rs_and_free(rs10);

        rs10 = bin_union_rs(rs1, rs3, true);
        dump_rs_and_free(rs10);


        printf("%s\n", "Direct search pointer location...");

        /* Do not free the result if you are using search_one, it will be free when you destroy the array */
        void *found = ba_search_one(bin_a_t, my_node, val_l, &search_key1l);

        if (found != NULL) {
            printf("%s\n", "Found");
            my_node* found_node = (my_node*)found;
            printf("%ld\n", found_node->val_l);
            printf("%s\n", found_node->val_cstr);
            printf("%d\n", found_node->val);
            printf("%f\n", found_node->val_f);
        }

        // Clear the current data set
        bin_array_clear(bin_a_t, free_my_node);


        printf("Now the array value bean cleared and size is %zu, reseting the data\n", bin_a_t->size);

        for (i = 0; i < 110; i++) {
            my_node *s = malloc(sizeof(my_node));
            s->val_l = sorted_number[i];
            if (i < 1) {
                s->val = 5;//i + i + i ;//values[i];
                s->val_cstr = strdup("ABC");                
                strcpy(s->val_cstr2, "BBB");
                s->val_f = 5.9f;
            }
            else if (i < 20) {
                s->val = 20;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");
                strcpy(s->val_cstr2, "XXX");

                s->val_f = 40.9f;
            }
            else if (i < 30) {
                s->val = 30;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");                
                strcpy(s->val_cstr2, "WWW");

                s->val_f = 40.9f;
            }
            else if (i < 40) {
                s->val = 40;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");                
                strcpy(s->val_cstr2, "TTT");
                s->val_f = 40.9f;
            }
            else if (i < 50) {
                s->val = 50;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "MMM");
                s->val_f = 50.9f;
            }
            else if (i < 60) {
                s->val = 60;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "NNN");
                s->val_f = 60.9f;
            }
            else if (i < 70) {
                s->val = 70;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "RRRR");
                s->val_f = 70.9f;
            }
            else if (i < 80) {
                s->val = 80;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "ERR");
                s->val_f = 80.9f;
            }
            else if (i < 90) {
                s->val = 90;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");                
                strcpy(s->val_cstr2, "VZX");
                s->val_f = 90.9f;
            }
            else if (i < 95) {
                s->val = 100;
                s->val_cstr = strdup("ZUZ");                
                strcpy(s->val_cstr2, "ZZE");
                s->val_f = 109.9f;
            } else {
                s->val = 111;
                s->val_cstr = strdup("ZUZ");                
                strcpy(s->val_cstr2, "ZUU");
                s->val_f = 119.9f;
            }
            bin_array_push(bin_a_t, s);
        }


        found = ba_search_one(bin_a_t, my_node, val_l, &search_key1l);

        if (found != NULL) {
            printf("%s\n", "Found After data reset again");
            my_node* found_node = (my_node*)found;
            printf("%ld\n", found_node->val_l);
            printf("%s\n", found_node->val_cstr);
            printf("%d\n", found_node->val);
            printf("%f\n", found_node->val_f);
        }

    }


    /****Safety SWAPPING ARRAY for MULTITHREADING PURPOSE*****/
    bin_array_t *new_ba = bin_array_create(n, 4);
    if (bin_add_index(new_ba, my_node, val_f, __def_float_sorted_cmp_func__)
            && bin_add_index(new_ba, my_node, val, __def_int_sorted_cmp_func__)
            && bin_add_index(new_ba, my_node, val_cstr, __def_cstr_sorted_cmp_func__)
            && bin_add_index(new_ba, my_node, val_l, __def_long_sorted_cmp_func__)
       ) {

        int i;
        for (i = 0; i < 110; i++) {
            my_node *s = malloc(sizeof(my_node));
            s->val_l = sorted_number[i];
            if (i < 1) {
                s->val = 5;//i + i + i ;//values[i];
                s->val_cstr = strdup("ABC");
                s->val_f = 5.9f;
            }
            else if (i < 20) {
                s->val = 20;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");
                s->val_f = 40.9f;
            }
            else if (i < 30) {
                s->val = 30;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");
                s->val_f = 40.9f;
            }
            else if (i < 40) {
                s->val = 40;//i + i + i ;//values[i];
                s->val_cstr = strdup("DXD");
                s->val_f = 40.9f;
            }
            else if (i < 50) {
                s->val = 50;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");
                s->val_f = 50.9f;
            }
            else if (i < 60) {
                s->val = 60;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");
                s->val_f = 60.9f;
            }
            else if (i < 70) {
                s->val = 70;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");
                s->val_f = 70.9f;
            }
            else if (i < 80) {
                s->val = 80;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");
                s->val_f = 80.9f;
            }
            else if (i < 90) {
                s->val = 90;//i + i + i ;//values[i];
                s->val_cstr = strdup("DDD");
                s->val_f = 90.9f;
            }
            else if (i < 95) {
                s->val = 100;
                s->val_cstr = strdup("ZUZ");
                s->val_f = 109.9f;
            } else {
                s->val = 111;
                s->val_cstr = strdup("ZUZ");
                s->val_f = 119.9f;
            }
            bin_array_push(new_ba, s);
        }
    }

    unsigned int buffer_timeMs_to_purge_old_array = 2 * 1000 * 1000;
    printf("%s\n", "Proceed Array Safety Swapping");
    bin_array_safety_swap(&bin_a_t, new_ba, free_my_node, buffer_timeMs_to_purge_old_array);

    usleep(buffer_timeMs_to_purge_old_array * 2);

    if(bin_a_t != NULL)
        bin_array_destroy(bin_a_t, free_my_node);



    return (0);
}
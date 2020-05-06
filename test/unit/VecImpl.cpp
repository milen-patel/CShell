#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

 /* Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */
TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
       Vec_ref(&v, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, get) {
    /* Attempt to get every element of the vec */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t outPosZero, outPosOne, outPosTwo;
    outPosZero = outPosOne = outPosTwo = 0;

    Vec_get(&v, 0, (void*) &outPosZero);
    Vec_get(&v, 1, (void*) &outPosOne); 
    Vec_get(&v, 2, (void*) &outPosTwo);

    ASSERT_EQ(buffer[0], outPosZero);
    ASSERT_EQ(buffer[1], outPosOne);
    ASSERT_EQ(buffer[2], outPosTwo);
    Vec_drop(&v);
}

TEST(VecImpl, get_out_of_bounds) {
    /* Attempt to use Vec_get with an invalid index */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    int16_t out;
    ASSERT_DEATH({
       Vec_get(&v, 2, &out);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, set) {
    /* Use Vec_set to replace elements in a Vec */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    
    int16_t newVal = 1039;
    Vec_set(&v, 0, &newVal);
    Vec_set(&v, 1, &newVal);

    ASSERT_EQ(buffer[0], newVal);
    ASSERT_EQ(buffer[1], newVal);
    ASSERT_EQ(v.length, 2);
    Vec_drop(&v);
}

TEST(VecImpl, set_index_zero) {
    /* Use Vec_set to add an element to an empty vec */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    int16_t newVal = 1039;
    ASSERT_EQ(v.length, 0);
    Vec_set(&v, 0, &newVal);
    ASSERT_EQ(buffer[0], newVal);
    ASSERT_EQ(v.length, 1);
    Vec_drop(&v);
}

TEST(VecImpl, set_out_of_bounds) {
    /* Attempt to use Vec_set on an invalid index */
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    int16_t newVal = 44;
    ASSERT_DEATH({
       Vec_set(&v,4,&newVal);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, set_append) {
    /* Use Vec_set to append an element to a vec */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    
    int16_t newVal = 21;
    Vec_set(&v, 3, &newVal);
    ASSERT_EQ(buffer[3], newVal);
    ASSERT_EQ(v.length, 4);
    Vec_drop(&v);
}

TEST(VecImpl, equals) {
    /* Use Vec_equals to compare two identical Vecs */
    Vec v = Vec_value(3, sizeof(int16_t));
    Vec v2 = Vec_value(3, sizeof(int16_t));
    int16_t *buffer2 = (int16_t*) v2.buffer;
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100; buffer2[0] = 100;
    buffer[1] = 200; buffer2[1] = 200;
    buffer[2] = 300; buffer2[2] = 300;
    v.length = 3;
    v2.length = 3;
    
    ASSERT_EQ(Vec_equals(&v,&v), true);
    ASSERT_EQ(Vec_equals(&v2,&v2), true);
    ASSERT_EQ(Vec_equals(&v,&v2), true); 
    Vec_drop(&v);
    Vec_drop(&v2);
}

TEST(VecImpl, equal_empty) {
    /* Use Vec_equals on two empty arrays */
    Vec v = Vec_value(3, sizeof(int16_t));
    Vec v2 = Vec_value(3, sizeof(int16_t));
    ASSERT_EQ(Vec_equals(&v,&v), true);
    ASSERT_EQ(Vec_equals(&v2,&v2), true);
    ASSERT_EQ(Vec_equals(&v,&v2), true); 
    Vec_drop(&v);
}

TEST(VecImpl, equals_same_length_only) {
    /* Use Vec_Equals on two vecs who differ in buffer content but have the same length */
    Vec v = Vec_value(3, sizeof(int16_t));
    Vec v2 = Vec_value(3, sizeof(int16_t));
    int16_t *buffer2 = (int16_t*) v2.buffer;
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100; buffer2[0] = 400;
    buffer[1] = 200; buffer2[1] = 500;
    buffer[2] = 300; buffer2[2] = 600;
    v.length = 3;
    v2.length = 3;
    ASSERT_EQ(Vec_equals(&v,&v2), false); 
    Vec_drop(&v);
    Vec_drop(&v2);
}

TEST(VecImpl, equals_same_buffer_only) {
    /* Use Vec_Equals on two vecs who differ in length but have identical buffer content */
    Vec v = Vec_value(3, sizeof(int16_t));
    Vec v2 = Vec_value(3, sizeof(int16_t));
    int16_t *buffer2 = (int16_t*) v2.buffer;
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100; buffer2[0] = 100;
    buffer[1] = 200; buffer2[1] = 200;
    buffer[2] = 300; 
    v.length = 3;
    v2.length = 2;
    ASSERT_EQ(Vec_equals(&v,&v2), false); 
    Vec_drop(&v);
    Vec_drop(&v2);
}

TEST(VecImpl, splice) {
    /* Use Vec_splice to both delete from and insert into a vec */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 1, 1, arr, 3);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(buffer[0], 100);
    ASSERT_EQ(buffer[1], 10);
    ASSERT_EQ(buffer[2], 20);
    ASSERT_EQ(buffer[3], 30);
    ASSERT_EQ(buffer[4], 300);
    ASSERT_EQ(v.length, 5);
    Vec_drop(&v);
}

TEST(VecImpl, splice_delete_only) {
    /* Use Vec_splice strictly to delete elements without inserting anything */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 1, 1, arr, 0);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(buffer[0], 100);
    ASSERT_EQ(buffer[1], 300);
    ASSERT_EQ(v.length, 2);
    Vec_drop(&v);
}

TEST(VecImpl, splice_no_modifications) {
    /* Use Vec_splice but dont make any  modifications to the array */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 0, 0, arr, 0);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(v.length, 3);
    ASSERT_EQ(buffer[0], 100);
    ASSERT_EQ(buffer[1], 200);
    ASSERT_EQ(buffer[2], 300);
    Vec_drop(&v);
}

TEST(VecImpl, splice_insert_only) {
    /* Use Vec_splice strictly to insert elements without deleting anything */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 1, 0, arr, 3);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(v.length, 6);
    ASSERT_EQ(buffer[0], 100);
    ASSERT_EQ(buffer[1], 10);
    ASSERT_EQ(buffer[2], 20);
    ASSERT_EQ(buffer[3], 30);
    ASSERT_EQ(buffer[4], 200);
    ASSERT_EQ(buffer[5], 300);
    Vec_drop(&v);
}

TEST(VecImpl, splice_delete_too_much) {
    /* Attempt to use Vec_splice to delete too many elements */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    ASSERT_DEATH({
       Vec_splice(&v, 1,3,arr,0);
    }, ".* - Out of Bounds");
    Vec_drop(&v); 
}

TEST(VecImpl, splice_delete_entire_arr) {
    /* Use Vec_splice to empty the entire buffer of a vec */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 0, 3, arr, 0); 
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(v.length, 0);
    Vec_drop(&v); 
}

TEST(VecImpl, splice_swap_two_arrs) {
    /* Use Vec_splice to replace one vec with another vec */
     Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 0, 3, arr, 4);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(v.length, 4);
    ASSERT_EQ(buffer[0], 10);
    ASSERT_EQ(buffer[1], 20);
    ASSERT_EQ(buffer[2], 30);
    ASSERT_EQ(buffer[3], 40);
    Vec_drop(&v); 
}

TEST(VecImpl, splice_insert_and_grow) {
    /* Ensure that Vec_splice grows dynamically if it's capacity is too little to handle the insertions */
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 1,1,arr,4);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(buffer[5], 300);
    ASSERT_EQ(v.length, 6);
    Vec_drop(&v); 
}

TEST(VecImpl, splice_delete_on_empty_arr) {
    /* Ensure that Vec_splice won't delete anything from an empty buffer */
    Vec v = Vec_value(5, sizeof(int16_t));
    int16_t arr[] = { 10, 20, 30, 40 };
    ASSERT_DEATH({
       Vec_splice(&v,0,1,arr,0);
    }, ".* - Out of Bounds");
    Vec_drop(&v); 
}

TEST(VecImpl, splice_invalid_index) {
    /* Attempt to use Vec_splice on an invalid index */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    ASSERT_DEATH({
       Vec_splice(&v, 5, 0, arr, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v); 
}

TEST(VecImpl, splice_combine_two_arrays) {
    /* Use Vec_splice to combine two different Vecs of the same type */
    Vec v = Vec_value(20, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t arr[] = { 10, 20, 30, 40 };
    Vec_splice(&v, 3, 0, arr, 4);
	buffer = (int16_t*) v.buffer;
    ASSERT_EQ(v.length, 7);
    ASSERT_EQ(buffer[0], 100);
    ASSERT_EQ(buffer[1], 200);
    ASSERT_EQ(buffer[2], 300);
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 20);
    ASSERT_EQ(buffer[5], 30);
    ASSERT_EQ(buffer[6], 40);
    Vec_drop(&v);
}

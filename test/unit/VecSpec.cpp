#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/**
 * The purpose of these tests is to prove correctness of the
 * abstraction barrier from the user's point-of-view. You should
 * not access any member of the Vec directly.
 */

TEST(VecSpec, values_init_empty) {
    Vec v = Vec_value(10, sizeof(double));
    ASSERT_EQ(0, Vec_length(&v));
    Vec_drop(&v);
}

TEST(VecSpec, vec_get_set_empty) {
    Vec v = Vec_value(10, sizeof(uint16_t));
    ASSERT_EQ(0, Vec_length(&v));
    uint16_t arr[] = {1,2,3};
    uint16_t out; 
    Vec_set(&v, 0, arr);
    Vec_set(&v, 1, arr+1);
    Vec_set(&v, 2, arr+2);

    Vec_get(&v, 0, &out);
    ASSERT_EQ(arr[0], out); 
    Vec_get(&v, 1, &out);
    ASSERT_EQ(arr[1], out); 
    Vec_get(&v, 2, &out);
    ASSERT_EQ(arr[2], out); 
    
    Vec_drop(&v); 
}

TEST(VecSpec, vec_get_set_normal_usage) {
    /* Use Vec_get and Vec_set in conjunction */
    Vec v = Vec_value(10, sizeof(uint16_t));
    ASSERT_EQ(0, Vec_length(&v));
    uint16_t arr[] = {1,2,3};
    Vec_set(&v, 0, arr);
    Vec_set(&v, 1, arr+1);
    Vec_set(&v, 2, arr+2);

    uint16_t newVar = 5;
    uint16_t out;
    Vec_set(&v, 0, &newVar);
    Vec_set(&v, 2, &newVar);
    Vec_get(&v, 0, &out);
    ASSERT_EQ(newVar, out); 
    Vec_get(&v, 2, &out);
    ASSERT_EQ(newVar, out); 
    Vec_drop(&v); 
}

TEST(VecSpec, vec_set_invalid_index) {
    /* Ensure that invalid index on Vec_set throws an error */
    Vec v = Vec_value(10, sizeof(uint16_t));
    ASSERT_EQ(0, Vec_length(&v));
    uint16_t arr[] = {1,2,3};
    ASSERT_DEATH({
        Vec_set(&v, 1, arr);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecSpec, vec_get_invalid_index) {
    /* Ensure that invalid index causes Vec_get to fail */
    Vec v = Vec_value(10, sizeof(uint16_t));
    ASSERT_EQ(0, Vec_length(&v));
    uint16_t arr[] = {1,2,3};
    Vec_set(&v, 0, arr);
    uint16_t out;
    ASSERT_DEATH({
        Vec_get(&v, 1, &out);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecSpec, vec_equals_normal_usage) {
    /* Test Equality of Two Vecs */
    Vec v1 = Vec_value(10, sizeof(uint16_t));
    Vec v2 = Vec_value(12, sizeof(uint16_t));
    ASSERT_EQ(Vec_equals(&v1, &v2), true);
    
    uint16_t arr[] = {1,2,3};
    Vec_set(&v1, 0, arr);
    Vec_set(&v1, 1, arr+1);
    Vec_set(&v1, 2, arr+2);
    Vec_set(&v2, 0, arr);
    Vec_set(&v2, 1, arr+1);
    Vec_set(&v2, 2, arr+2);
    ASSERT_EQ(Vec_equals(&v1, &v2), true);
    Vec_drop(&v1);
    Vec_drop(&v2);
}

TEST(VecSpec, vec_equals_empty_vec) {
    /* Use Vec_equals on two empty Vecs */
    Vec v1 = Vec_value(10, sizeof(uint16_t));
    Vec v2 = Vec_value(12, sizeof(uint16_t));
    ASSERT_EQ(Vec_equals(&v1, &v2), true);
    Vec_drop(&v1);
    Vec_drop(&v2);
}

TEST(VecSpec, vec_equals_same_buffer_only) {
    /* Ensure Vec_equals compares buffer content */
    Vec v1 = Vec_value(10, sizeof(uint16_t));
    Vec v2 = Vec_value(12, sizeof(uint16_t));
    uint16_t v1data[] = { 1, 2 };

    Vec_set(&v1, 0, v1data);
    Vec_set(&v1, 1, v1data+1);
    Vec_set(&v2, 0, v1data);
    ASSERT_EQ(Vec_equals(&v1, &v2), false);
    Vec_drop(&v1);
    Vec_drop(&v2);
}

TEST(VecSpec, vec_splice_insert_only)  {
    /* Use Vec_splice to insert into a Vec */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t rawData[] = { 100, 200, 300, 400 };
    int16_t insertData[] = { 800,900 };
     
    Vec_set(&v, 0, rawData); 
    Vec_set(&v, 1, rawData+1); 
    Vec_set(&v, 2, rawData+2); 
    Vec_set(&v, 3, rawData+3); 
    Vec_splice(&v, 2, 0, insertData, 2);
    
    int16_t out; 
    Vec_get(&v, 0, &out);
    ASSERT_EQ(out, 100);
    Vec_get(&v, 1, &out);
    ASSERT_EQ(out, 200);
    Vec_get(&v, 2, &out);
    ASSERT_EQ(out, 800);
    Vec_get(&v, 3, &out);
    ASSERT_EQ(out, 900);
    Vec_get(&v, 4, &out);
    ASSERT_EQ(out, 300);
    Vec_get(&v, 5, &out);
    ASSERT_EQ(out, 400);
    ASSERT_EQ(Vec_length(&v), 6);
    Vec_drop(&v);
}

TEST(VecSpec, vec_splice_delete_and_insert)  {
    /* Use Vec_splice to replace elements of a vec */
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t rawData[] = { 100, 200, 300, 400 };
    int16_t insertData[] = { 800,900 };
     
    Vec_set(&v, 0, rawData); 
    Vec_set(&v, 1, rawData+1); 
    Vec_set(&v, 2, rawData+2); 
    Vec_set(&v, 3, rawData+3); 
    Vec_splice(&v, 2, 1, insertData, 2);
    int16_t out; 
    Vec_get(&v, 0, &out);
    ASSERT_EQ(out, 100);
    Vec_get(&v, 1, &out);
    ASSERT_EQ(out, 200);
    Vec_get(&v, 2, &out);
    ASSERT_EQ(out, 800);
    Vec_get(&v, 3, &out);
    ASSERT_EQ(out, 900);
    Vec_get(&v, 4, &out);
    ASSERT_EQ(out, 400);
    ASSERT_EQ(Vec_length(&v), 5);
    Vec_drop(&v);
}

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

#include <stdint.h>

#include "test_helpers.h"

START_TEST(test_add_positive_integers) {
  s21_decimal left = make_decimal(15, 0, 0, 0, 0);
  s21_decimal right = make_decimal(27, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(42, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_add(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_add_different_scales) {
  s21_decimal left = make_decimal(123, 0, 0, 1, 0);
  s21_decimal right = make_decimal(45, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_add(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_add_opposite_signs) {
  s21_decimal left = make_decimal(10, 0, 0, 0, 1);
  s21_decimal right = make_decimal(4, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(6, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_add(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_add_positive_overflow) {
  s21_decimal maximum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal one = make_decimal(1, 0, 0, 0, 0);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_add(maximum, one, &result), 1);
}
END_TEST

START_TEST(test_add_negative_overflow) {
  s21_decimal minimum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal minus_one = make_decimal(1, 0, 0, 0, 1);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_add(minimum, minus_one, &result), 2);
}
END_TEST

START_TEST(test_sub_positive_result) {
  s21_decimal left = make_decimal(10, 0, 0, 0, 0);
  s21_decimal right = make_decimal(3, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(7, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_sub(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_sub_negative_result) {
  s21_decimal left = make_decimal(3, 0, 0, 0, 0);
  s21_decimal right = make_decimal(10, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(7, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_sub(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_sub_different_scales) {
  s21_decimal left = make_decimal(55, 0, 0, 1, 0);
  s21_decimal right = make_decimal(225, 0, 0, 2, 0);
  s21_decimal expected = make_decimal(325, 0, 0, 2, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_sub(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_sub_bank_rounding_near_maximum) {
  s21_decimal maximum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal six_tenths = make_decimal(6, 0, 0, 1, 0);
  s21_decimal expected =
      make_decimal(UINT32_MAX - 1u, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_sub(maximum, six_tenths, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_positive_integers) {
  s21_decimal left = make_decimal(12, 0, 0, 0, 0);
  s21_decimal right = make_decimal(3, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(36, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_mul(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_negative_and_scaled) {
  s21_decimal left = make_decimal(12, 0, 0, 1, 1);
  s21_decimal right = make_decimal(3, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(36, 0, 0, 1, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_mul(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_fractional_values) {
  s21_decimal left = make_decimal(2, 0, 0, 1, 0);
  s21_decimal right = make_decimal(3, 0, 0, 1, 0);
  s21_decimal expected = make_decimal(6, 0, 0, 2, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_mul(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_positive_overflow) {
  s21_decimal maximum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal two = make_decimal(2, 0, 0, 0, 0);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_mul(maximum, two, &result), 1);
}
END_TEST

START_TEST(test_mul_negative_overflow) {
  s21_decimal minimum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal two = make_decimal(2, 0, 0, 0, 0);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_mul(minimum, two, &result), 2);
}
END_TEST

START_TEST(test_div_exact_integer) {
  s21_decimal dividend = make_decimal(10, 0, 0, 0, 0);
  s21_decimal divisor = make_decimal(2, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(5, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_div(dividend, divisor, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_div_fractional_result) {
  s21_decimal dividend = make_decimal(1, 0, 0, 0, 0);
  s21_decimal divisor = make_decimal(4, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(25, 0, 0, 2, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_div(dividend, divisor, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_div_negative_result) {
  s21_decimal dividend = make_decimal(1, 0, 0, 0, 1);
  s21_decimal divisor = make_decimal(4, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(25, 0, 0, 2, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_div(dividend, divisor, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_div_by_positive_zero) {
  s21_decimal dividend = make_decimal(1, 0, 0, 0, 0);
  s21_decimal zero = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_div(dividend, zero, &result), 3);
}
END_TEST

START_TEST(test_div_by_negative_zero) {
  s21_decimal dividend = make_decimal(1, 0, 0, 0, 0);
  s21_decimal negative_zero = make_decimal(0, 0, 0, 0, 1);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_div(dividend, negative_zero, &result), 3);
}
END_TEST

START_TEST(test_add_rounding_overflow_after_bank_round) {
  s21_decimal maximum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal half = make_decimal(5, 0, 0, 1, 0);
  s21_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_add(maximum, half, &result), 1);
}
END_TEST

START_TEST(test_add_rounding_overflow_reduces_one_more_scale) {
  s21_decimal left = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 0);
  s21_decimal right = make_decimal(5, 0, 0, 2, 0);
  s21_decimal expected =
      make_decimal(0x9999999Au, 0x99999999u, 0x19999999u, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_add(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_scale_reduction_sets_sticky) {
  s21_decimal left = make_decimal(11, 0, 0, 2, 0);
  s21_decimal right = make_decimal(1, 0, 0, 28, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_mul(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_mul_bank_rounds_up_at_minimum_scale) {
  s21_decimal left = make_decimal(6, 0, 0, 1, 0);
  s21_decimal right = make_decimal(1, 0, 0, 28, 0);
  s21_decimal expected = make_decimal(1, 0, 0, 28, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_mul(left, right, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

Suite* arithmetic_suite(void) {
  Suite* suite = suite_create("Arithmetic");
  TCase* addition = tcase_create("s21_add");
  TCase* subtraction = tcase_create("s21_sub");
  TCase* multiplication = tcase_create("s21_mul");
  TCase* division = tcase_create("s21_div");

  tcase_add_test(addition, test_add_positive_integers);
  tcase_add_test(addition, test_add_different_scales);
  tcase_add_test(addition, test_add_opposite_signs);
  tcase_add_test(addition, test_add_positive_overflow);
  tcase_add_test(addition, test_add_negative_overflow);
  tcase_add_test(addition, test_add_rounding_overflow_after_bank_round);
  tcase_add_test(addition, test_add_rounding_overflow_reduces_one_more_scale);

  tcase_add_test(subtraction, test_sub_positive_result);
  tcase_add_test(subtraction, test_sub_negative_result);
  tcase_add_test(subtraction, test_sub_different_scales);
  tcase_add_test(subtraction, test_sub_bank_rounding_near_maximum);

  tcase_add_test(multiplication, test_mul_positive_integers);
  tcase_add_test(multiplication, test_mul_negative_and_scaled);
  tcase_add_test(multiplication, test_mul_fractional_values);
  tcase_add_test(multiplication, test_mul_positive_overflow);
  tcase_add_test(multiplication, test_mul_negative_overflow);
  tcase_add_test(multiplication, test_mul_scale_reduction_sets_sticky);
  tcase_add_test(multiplication, test_mul_bank_rounds_up_at_minimum_scale);

  tcase_add_test(division, test_div_exact_integer);
  tcase_add_test(division, test_div_fractional_result);
  tcase_add_test(division, test_div_negative_result);
  tcase_add_test(division, test_div_by_positive_zero);
  tcase_add_test(division, test_div_by_negative_zero);

  suite_add_tcase(suite, addition);
  suite_add_tcase(suite, subtraction);
  suite_add_tcase(suite, multiplication);
  suite_add_tcase(suite, division);

  return suite;
}

#include <stdint.h>

#include "test_helpers.h"

static void assert_decimal_order(s21_decimal left, s21_decimal right,
                                 int expected_order) {
  int actual_less = s21_is_less(left, right);
  int actual_less_or_equal = s21_is_less_or_equal(left, right);
  int actual_greater = s21_is_greater(left, right);
  int actual_greater_or_equal = s21_is_greater_or_equal(left, right);
  int actual_equal = s21_is_equal(left, right);
  int actual_not_equal = s21_is_not_equal(left, right);

  ck_assert_int_eq(actual_less, expected_order < 0);
  ck_assert_int_eq(actual_less_or_equal, expected_order <= 0);
  ck_assert_int_eq(actual_greater, expected_order > 0);
  ck_assert_int_eq(actual_greater_or_equal, expected_order >= 0);
  ck_assert_int_eq(actual_equal, expected_order == 0);
  ck_assert_int_eq(actual_not_equal, expected_order != 0);
}

START_TEST(test_compare_equal_positive_integers) {
  s21_decimal left = make_decimal(42, 0, 0, 0, 0);
  s21_decimal right = make_decimal(42, 0, 0, 0, 0);

  assert_decimal_order(left, right, 0);
}
END_TEST

START_TEST(test_compare_less_positive_integers) {
  s21_decimal left = make_decimal(12, 0, 0, 0, 0);
  s21_decimal right = make_decimal(42, 0, 0, 0, 0);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_greater_positive_integers) {
  s21_decimal left = make_decimal(42, 0, 0, 0, 0);
  s21_decimal right = make_decimal(12, 0, 0, 0, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_positive_and_negative) {
  s21_decimal left = make_decimal(1, 0, 0, 0, 0);
  s21_decimal right = make_decimal(1, 0, 0, 0, 1);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_negative_and_positive) {
  s21_decimal left = make_decimal(1, 0, 0, 0, 1);
  s21_decimal right = make_decimal(1, 0, 0, 0, 0);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_negative_values) {
  s21_decimal left = make_decimal(10, 0, 0, 0, 1);
  s21_decimal right = make_decimal(2, 0, 0, 0, 1);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_greater_negative_values) {
  s21_decimal left = make_decimal(2, 0, 0, 0, 1);
  s21_decimal right = make_decimal(10, 0, 0, 0, 1);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_equal_different_scales) {
  s21_decimal left = make_decimal(12, 0, 0, 0, 0);
  s21_decimal right = make_decimal(12000, 0, 0, 3, 0);

  assert_decimal_order(left, right, 0);
}
END_TEST

START_TEST(test_compare_equal_negative_different_scales) {
  s21_decimal left = make_decimal(12, 0, 0, 0, 1);
  s21_decimal right = make_decimal(12000, 0, 0, 3, 1);

  assert_decimal_order(left, right, 0);
}
END_TEST

START_TEST(test_compare_positive_and_negative_zero) {
  s21_decimal left = make_decimal(0, 0, 0, 0, 0);
  s21_decimal right = make_decimal(0, 0, 0, 28, 1);

  assert_decimal_order(left, right, 0);
}
END_TEST

START_TEST(test_compare_values_requiring_scale_alignment) {
  s21_decimal left = make_decimal(12, 0, 0, 1, 0);
  s21_decimal right = make_decimal(119, 0, 0, 2, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_negative_values_requiring_scale_alignment) {
  s21_decimal left = make_decimal(12, 0, 0, 1, 1);
  s21_decimal right = make_decimal(119, 0, 0, 2, 1);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_positive_fraction_below_one) {
  s21_decimal left = make_decimal(999, 0, 0, 3, 0);
  s21_decimal right = make_decimal(1, 0, 0, 0, 0);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_minimum_positive_fraction_with_zero) {
  s21_decimal left = make_decimal(1, 0, 0, 28, 0);
  s21_decimal zero = make_decimal(0, 0, 0, 0, 0);

  assert_decimal_order(left, zero, 1);
}
END_TEST

START_TEST(test_compare_minimum_negative_fraction_with_zero) {
  s21_decimal left = make_decimal(1, 0, 0, 28, 1);
  s21_decimal zero = make_decimal(0, 0, 0, 0, 0);

  assert_decimal_order(left, zero, -1);
}
END_TEST

START_TEST(test_compare_middle_word_boundary) {
  s21_decimal left = make_decimal(0, 1, 0, 0, 0);
  s21_decimal right = make_decimal(UINT32_MAX, 0, 0, 0, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_high_word_boundary) {
  s21_decimal left = make_decimal(0, 0, 1, 0, 0);
  s21_decimal right = make_decimal(UINT32_MAX, UINT32_MAX, 0, 0, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_large_equal_with_trailing_zero) {
  s21_decimal left = make_decimal(0x99999999u, 0x99999999u, 0x19999999u, 0, 0);
  s21_decimal right = make_decimal(0xFFFFFFFAu, UINT32_MAX, UINT32_MAX, 1, 0);

  assert_decimal_order(left, right, 0);
}
END_TEST

START_TEST(test_compare_scale_alignment_near_maximum) {
  s21_decimal left = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal right = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_negative_scale_alignment_near_maximum) {
  s21_decimal left = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal right = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 1);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_large_adjacent_values) {
  s21_decimal left = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal right =
      make_decimal(UINT32_MAX - 1u, UINT32_MAX, UINT32_MAX, 0, 0);

  assert_decimal_order(left, right, 1);
}
END_TEST

START_TEST(test_compare_negative_large_adjacent_values) {
  s21_decimal left = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal right =
      make_decimal(UINT32_MAX - 1u, UINT32_MAX, UINT32_MAX, 0, 1);

  assert_decimal_order(left, right, -1);
}
END_TEST

START_TEST(test_compare_decimal_extremes) {
  s21_decimal maximum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal minimum = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);

  assert_decimal_order(maximum, minimum, 1);
}
END_TEST

START_TEST(test_compare_equal_at_maximum_scale) {
  s21_decimal one = make_decimal(1, 0, 0, 0, 0);
  s21_decimal one_with_maximum_scale =
      make_decimal(0x10000000u, 0x3E250261u, 0x204FCE5Eu, 28, 0);

  assert_decimal_order(one, one_with_maximum_scale, 0);
}
END_TEST

START_TEST(test_compare_close_values_at_maximum_scale) {
  s21_decimal one = make_decimal(1, 0, 0, 0, 0);
  s21_decimal value_below_one =
      make_decimal(0x0FFFFFFFu, 0x3E250261u, 0x204FCE5Eu, 28, 0);

  assert_decimal_order(one, value_below_one, 1);
}
END_TEST

Suite* comparison_suite(void) {
  Suite* suite = suite_create("Comparison");
  TCase* comparison = tcase_create("comparison operators");

  tcase_add_test(comparison, test_compare_equal_positive_integers);
  tcase_add_test(comparison, test_compare_less_positive_integers);
  tcase_add_test(comparison, test_compare_greater_positive_integers);
  tcase_add_test(comparison, test_compare_positive_and_negative);
  tcase_add_test(comparison, test_compare_negative_and_positive);
  tcase_add_test(comparison, test_compare_negative_values);
  tcase_add_test(comparison, test_compare_greater_negative_values);
  tcase_add_test(comparison, test_compare_equal_different_scales);
  tcase_add_test(comparison, test_compare_equal_negative_different_scales);
  tcase_add_test(comparison, test_compare_positive_and_negative_zero);
  tcase_add_test(comparison, test_compare_values_requiring_scale_alignment);
  tcase_add_test(comparison,
                 test_compare_negative_values_requiring_scale_alignment);
  tcase_add_test(comparison, test_compare_positive_fraction_below_one);
  tcase_add_test(comparison, test_compare_minimum_positive_fraction_with_zero);
  tcase_add_test(comparison, test_compare_minimum_negative_fraction_with_zero);
  tcase_add_test(comparison, test_compare_middle_word_boundary);
  tcase_add_test(comparison, test_compare_high_word_boundary);
  tcase_add_test(comparison, test_compare_large_equal_with_trailing_zero);
  tcase_add_test(comparison, test_compare_scale_alignment_near_maximum);
  tcase_add_test(comparison,
                 test_compare_negative_scale_alignment_near_maximum);
  tcase_add_test(comparison, test_compare_large_adjacent_values);
  tcase_add_test(comparison, test_compare_negative_large_adjacent_values);
  tcase_add_test(comparison, test_compare_decimal_extremes);
  tcase_add_test(comparison, test_compare_equal_at_maximum_scale);
  tcase_add_test(comparison, test_compare_close_values_at_maximum_scale);

  suite_add_tcase(suite, comparison);

  return suite;
}

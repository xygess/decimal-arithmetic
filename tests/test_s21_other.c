#include <stdint.h>

#include "test_helpers.h"

START_TEST(test_floor_positive_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_positive_integer) {
  s21_decimal dec = make_decimal(12, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_negative_integer) {
  s21_decimal dec = make_decimal(12, 0, 0, 0, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_positive_less_than_one) {
  s21_decimal dec = make_decimal(1, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_negative_greater_than_minus_one) {
  s21_decimal dec = make_decimal(1, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(1, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal dec = make_decimal(0, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_negative_scaled_integer) {
  s21_decimal dec = make_decimal(12000, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_maximum_scale_positive) {
  s21_decimal dec = make_decimal(1, 0, 0, 28, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_maximum_scale_negative) {
  s21_decimal dec = make_decimal(1, 0, 0, 28, 1);
  s21_decimal expected = make_decimal(1, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_large_positive_fraction) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 0);
  s21_decimal expected =
      make_decimal(0x99999999u, 0x99999999u, 0x19999999u, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_floor_large_negative_fraction) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 1);
  s21_decimal expected =
      make_decimal(0x9999999Au, 0x99999999u, 0x19999999u, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_floor(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_positive_fraction_down) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_positive_fraction_up) {
  s21_decimal dec = make_decimal(126, 0, 0, 1, 0);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_negative_fraction_toward_zero) {
  s21_decimal dec = make_decimal(124, 0, 0, 1, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_negative_fraction_away_from_zero) {
  s21_decimal dec = make_decimal(126, 0, 0, 1, 1);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

// These tests use conventional midpoint rounding away from zero.
START_TEST(test_round_positive_midpoint) {
  s21_decimal dec = make_decimal(125, 0, 0, 1, 0);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_negative_midpoint) {
  s21_decimal dec = make_decimal(125, 0, 0, 1, 1);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_fraction_just_below_half) {
  s21_decimal dec = make_decimal(124999, 0, 0, 4, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_fraction_just_above_half) {
  s21_decimal dec = make_decimal(125001, 0, 0, 4, 0);
  s21_decimal expected = make_decimal(13, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_positive_integer) {
  s21_decimal dec = make_decimal(12, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_negative_scaled_integer) {
  s21_decimal dec = make_decimal(12000, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal dec = make_decimal(0, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_maximum_scale_positive) {
  s21_decimal dec = make_decimal(1, 0, 0, 28, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_maximum_scale_negative) {
  s21_decimal dec = make_decimal(1, 0, 0, 28, 1);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_carry_to_middle_word) {
  s21_decimal dec = make_decimal(0xFFFFFFFCu, 0x00000009u, 0, 1, 0);
  s21_decimal expected = make_decimal(0, 1, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_large_positive_midpoint) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 0);
  s21_decimal expected =
      make_decimal(0x9999999Au, 0x99999999u, 0x19999999u, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_round_large_negative_midpoint) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 1);
  s21_decimal expected =
      make_decimal(0x9999999Au, 0x99999999u, 0x19999999u, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_round(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_truncate_positive_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_negative_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_positive_integer) {
  s21_decimal dec = make_decimal(42, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(42, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_negative_integer) {
  s21_decimal dec = make_decimal(42, 0, 0, 0, 1);
  s21_decimal expected = make_decimal(42, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_positive_less_than_one) {
  s21_decimal dec = make_decimal(999, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_negative_greater_than_minus_one) {
  s21_decimal dec = make_decimal(999, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_negative_scaled_integer) {
  s21_decimal dec = make_decimal(12000, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_negative_scaled_zero) {
  s21_decimal dec = make_decimal(0, 0, 0, 28, 1);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_maximum_scale) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 0);
  s21_decimal expected = make_decimal(7, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_large_positive_fraction) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 0);
  s21_decimal expected =
      make_decimal(0x99999999u, 0x99999999u, 0x19999999u, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_truncate_large_negative_fraction) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 1, 1);
  s21_decimal expected =
      make_decimal(0x99999999u, 0x99999999u, 0x19999999u, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_truncate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
  assert_decimal_scale_zero(result);
}
END_TEST

START_TEST(test_negate_positive_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal expected = make_decimal(12345, 0, 0, 3, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_negative_fraction) {
  s21_decimal dec = make_decimal(12345, 0, 0, 3, 1);
  s21_decimal expected = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_positive_integer) {
  s21_decimal dec = make_decimal(42, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(42, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_negative_integer) {
  s21_decimal dec = make_decimal(42, 0, 0, 0, 1);
  s21_decimal expected = make_decimal(42, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_large_positive) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal expected = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_large_negative) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);
  s21_decimal expected = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_maximum_scale) {
  s21_decimal dec = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 0);
  s21_decimal expected =
      make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_positive_zero) {
  s21_decimal dec = make_decimal(0, 0, 0, 0, 0);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_negative_scaled_zero) {
  s21_decimal dec = make_decimal(0, 0, 0, 28, 1);
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_negate(dec, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_negate_twice_restores_value) {
  s21_decimal dec = make_decimal(12345, 67890, 123, 7, 1);
  s21_decimal first = {{-1, -1, -1, -1}};
  s21_decimal second = {{-1, -1, -1, -1}};

  ck_assert_int_eq(s21_negate(dec, &first), 0);
  ck_assert_int_eq(s21_negate(first, &second), 0);
  assert_decimal_value_equal(second, dec);
}
END_TEST

Suite* other_suite(void) {
  Suite* suite = suite_create("Other");
  TCase* floor = tcase_create("s21_floor");
  TCase* round = tcase_create("s21_round");
  TCase* truncate = tcase_create("s21_truncate");
  TCase* negate = tcase_create("s21_negate");

  tcase_add_test(floor, test_floor_positive_fraction);
  tcase_add_test(floor, test_floor_negative_fraction);
  tcase_add_test(floor, test_floor_positive_integer);
  tcase_add_test(floor, test_floor_negative_integer);
  tcase_add_test(floor, test_floor_positive_less_than_one);
  tcase_add_test(floor, test_floor_negative_greater_than_minus_one);
  tcase_add_test(floor, test_floor_zero);
  tcase_add_test(floor, test_floor_negative_scaled_integer);
  tcase_add_test(floor, test_floor_maximum_scale_positive);
  tcase_add_test(floor, test_floor_maximum_scale_negative);
  tcase_add_test(floor, test_floor_large_positive_fraction);
  tcase_add_test(floor, test_floor_large_negative_fraction);

  tcase_add_test(round, test_round_positive_fraction_down);
  tcase_add_test(round, test_round_positive_fraction_up);
  tcase_add_test(round, test_round_negative_fraction_toward_zero);
  tcase_add_test(round, test_round_negative_fraction_away_from_zero);
  tcase_add_test(round, test_round_positive_midpoint);
  tcase_add_test(round, test_round_negative_midpoint);
  tcase_add_test(round, test_round_fraction_just_below_half);
  tcase_add_test(round, test_round_fraction_just_above_half);
  tcase_add_test(round, test_round_positive_integer);
  tcase_add_test(round, test_round_negative_scaled_integer);
  tcase_add_test(round, test_round_zero);
  tcase_add_test(round, test_round_maximum_scale_positive);
  tcase_add_test(round, test_round_maximum_scale_negative);
  tcase_add_test(round, test_round_carry_to_middle_word);
  tcase_add_test(round, test_round_large_positive_midpoint);
  tcase_add_test(round, test_round_large_negative_midpoint);

  tcase_add_test(truncate, test_truncate_positive_fraction);
  tcase_add_test(truncate, test_truncate_negative_fraction);
  tcase_add_test(truncate, test_truncate_positive_integer);
  tcase_add_test(truncate, test_truncate_negative_integer);
  tcase_add_test(truncate, test_truncate_positive_less_than_one);
  tcase_add_test(truncate, test_truncate_negative_greater_than_minus_one);
  tcase_add_test(truncate, test_truncate_negative_scaled_integer);
  tcase_add_test(truncate, test_truncate_negative_scaled_zero);
  tcase_add_test(truncate, test_truncate_maximum_scale);
  tcase_add_test(truncate, test_truncate_large_positive_fraction);
  tcase_add_test(truncate, test_truncate_large_negative_fraction);

  tcase_add_test(negate, test_negate_positive_fraction);
  tcase_add_test(negate, test_negate_negative_fraction);
  tcase_add_test(negate, test_negate_positive_integer);
  tcase_add_test(negate, test_negate_negative_integer);
  tcase_add_test(negate, test_negate_large_positive);
  tcase_add_test(negate, test_negate_large_negative);
  tcase_add_test(negate, test_negate_maximum_scale);
  tcase_add_test(negate, test_negate_positive_zero);
  tcase_add_test(negate, test_negate_negative_scaled_zero);
  tcase_add_test(negate, test_negate_twice_restores_value);

  suite_add_tcase(suite, floor);
  suite_add_tcase(suite, round);
  suite_add_tcase(suite, truncate);
  suite_add_tcase(suite, negate);

  return suite;
}

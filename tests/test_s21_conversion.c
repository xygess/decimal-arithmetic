#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "test_helpers.h"

static uint32_t float_bits(float value) {
  uint32_t bits = 0;
  memcpy(&bits, &value, sizeof(bits));
  return bits;
}

static void assert_float_close(float actual, float expected) {
  ck_assert_msg(isfinite(actual), "result is not finite");

  if (expected == 0.0f) {
    ck_assert_msg(actual == 0.0f, "expected zero, got %.9g", (double)actual);
  } else {
    float lower = nextafterf(expected, -INFINITY);
    float upper = nextafterf(expected, INFINITY);
    ck_assert_msg(actual >= lower && actual <= upper, "expected %.9g, got %.9g",
                  (double)expected, (double)actual);
  }
}

static void assert_float_to_decimal_success(float source,
                                            s21_decimal expected) {
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_float_to_decimal(source, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}

static void assert_float_to_decimal_error(float source) {
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_float_to_decimal(source, &result);

  ck_assert_int_eq(status, 1);
}

static void assert_tiny_float_conversion_error(float source) {
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_float_to_decimal(source, &result);

  ck_assert_int_eq(status, 1);
  assert_decimal_value_equal(result, expected);
}

static void assert_decimal_to_int_success(s21_decimal source, int expected) {
  int result = 123456789;

  int status = s21_from_decimal_to_int(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result, expected);
}

static void assert_decimal_to_int_error(s21_decimal source) {
  int result = 123456789;

  int status = s21_from_decimal_to_int(source, &result);

  ck_assert_int_eq(status, 1);
}

static void assert_decimal_to_float_success(s21_decimal source,
                                            float expected) {
  float result = NAN;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  assert_float_close(result, expected);
}

static void assert_decimal_to_float_exact(s21_decimal source, float expected) {
  float result = NAN;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_msg(result == expected, "expected %.9g, got %.9g", (double)expected,
                (double)result);
}

START_TEST(test_int_to_decimal_positive_integer) {
  s21_decimal expected = make_decimal(12, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_int_to_decimal(12, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_int_to_decimal_negative_integer) {
  s21_decimal expected = make_decimal(12, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_int_to_decimal(-12, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_int_to_decimal_zero) {
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_int_to_decimal(0, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_int_to_decimal_int_max) {
  s21_decimal expected = make_decimal(0x7FFFFFFFu, 0, 0, 0, 0);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_int_to_decimal(INT_MAX, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_int_to_decimal_int_min) {
  s21_decimal expected = make_decimal(0x80000000u, 0, 0, 0, 1);
  s21_decimal result = {{-1, -1, -1, -1}};

  int status = s21_from_int_to_decimal(INT_MIN, &result);

  ck_assert_int_eq(status, 0);
  assert_decimal_value_equal(result, expected);
}
END_TEST

START_TEST(test_float_to_decimal_positive_zero) {
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);

  assert_float_to_decimal_success(0.0f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_negative_zero) {
  s21_decimal expected = make_decimal(0, 0, 0, 0, 0);

  assert_float_to_decimal_success(-0.0f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_positive_fraction) {
  s21_decimal expected = make_decimal(1235, 0, 0, 1, 0);

  assert_float_to_decimal_success(123.5f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_negative_fraction) {
  s21_decimal expected = make_decimal(1275, 0, 0, 2, 1);

  assert_float_to_decimal_success(-12.75f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_exact_binary_fraction) {
  s21_decimal expected = make_decimal(125, 0, 0, 3, 0);

  assert_float_to_decimal_success(0.125f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_seven_significant_digits) {
  s21_decimal expected = make_decimal(1234567, 0, 0, 0, 0);

  assert_float_to_decimal_success(1234567.0f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_rounds_to_seven_significant_digits) {
  s21_decimal expected = make_decimal(16777220, 0, 0, 0, 0);

  assert_float_to_decimal_success(16777216.0f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_bank_rounding_even_down) {
  s21_decimal expected = make_decimal(1234562, 0, 0, 1, 0);

  assert_float_to_decimal_success(123456.25f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_bank_rounding_odd_up) {
  s21_decimal expected = make_decimal(1234568, 0, 0, 1, 0);

  assert_float_to_decimal_success(123456.75f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_negative_bank_rounding_even_down) {
  s21_decimal expected = make_decimal(1234562, 0, 0, 1, 1);

  assert_float_to_decimal_success(-123456.25f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_negative_bank_rounding_odd_up) {
  s21_decimal expected = make_decimal(1234568, 0, 0, 1, 1);

  assert_float_to_decimal_success(-123456.75f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_minimum_positive_magnitude) {
  s21_decimal expected = make_decimal(1, 0, 0, 28, 0);

  assert_float_to_decimal_success(1e-28f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_minimum_negative_magnitude) {
  s21_decimal expected = make_decimal(1, 0, 0, 28, 1);

  assert_float_to_decimal_success(-1e-28f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_small_scientific_value) {
  s21_decimal expected = make_decimal(1234567, 0, 0, 26, 0);

  assert_float_to_decimal_success(1.234567e-20f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_largest_float_below_overflow) {
  s21_decimal expected =
      make_decimal(0x20000000u, 0xB392B21Au, 0xFFFFFF77u, 0, 0);

  assert_float_to_decimal_success(0x1.fffffep+95f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_largest_negative_float_below_overflow) {
  s21_decimal expected =
      make_decimal(0x20000000u, 0xB392B21Au, 0xFFFFFF77u, 0, 1);

  assert_float_to_decimal_success(-0x1.fffffep+95f, expected);
}
END_TEST

START_TEST(test_float_to_decimal_too_small_positive) {
  assert_tiny_float_conversion_error(1e-29f);
}
END_TEST

START_TEST(test_float_to_decimal_too_small_negative) {
  assert_tiny_float_conversion_error(-1e-29f);
}
END_TEST

START_TEST(test_float_to_decimal_positive_overflow_boundary) {
  assert_float_to_decimal_error(0x1p96f);
}
END_TEST

START_TEST(test_float_to_decimal_negative_overflow_boundary) {
  assert_float_to_decimal_error(-0x1p96f);
}
END_TEST

START_TEST(test_float_to_decimal_flt_max) {
  assert_float_to_decimal_error(FLT_MAX);
}
END_TEST

START_TEST(test_float_to_decimal_negative_flt_max) {
  assert_float_to_decimal_error(-FLT_MAX);
}
END_TEST

START_TEST(test_float_to_decimal_positive_infinity) {
  assert_float_to_decimal_error(INFINITY);
}
END_TEST

START_TEST(test_float_to_decimal_negative_infinity) {
  assert_float_to_decimal_error(-INFINITY);
}
END_TEST

START_TEST(test_float_to_decimal_nan) { assert_float_to_decimal_error(NAN); }
END_TEST

START_TEST(test_decimal_to_int_zero) {
  s21_decimal source = make_decimal(0, 0, 0, 0, 0);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_negative_scaled_zero) {
  s21_decimal source = make_decimal(0, 0, 0, 28, 1);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_positive_integer) {
  s21_decimal source = make_decimal(12, 0, 0, 0, 0);

  assert_decimal_to_int_success(source, 12);
}
END_TEST

START_TEST(test_decimal_to_int_negative_integer) {
  s21_decimal source = make_decimal(12, 0, 0, 0, 1);

  assert_decimal_to_int_success(source, -12);
}
END_TEST

START_TEST(test_decimal_to_int_positive_fraction) {
  s21_decimal source = make_decimal(12999, 0, 0, 3, 0);

  assert_decimal_to_int_success(source, 12);
}
END_TEST

START_TEST(test_decimal_to_int_negative_fraction) {
  s21_decimal source = make_decimal(12999, 0, 0, 3, 1);

  assert_decimal_to_int_success(source, -12);
}
END_TEST

START_TEST(test_decimal_to_int_positive_fraction_below_one) {
  s21_decimal source = make_decimal(999, 0, 0, 3, 0);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_negative_fraction_above_minus_one) {
  s21_decimal source = make_decimal(999, 0, 0, 3, 1);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_scaled_integer) {
  s21_decimal source = make_decimal(12000, 0, 0, 3, 0);

  assert_decimal_to_int_success(source, 12);
}
END_TEST

START_TEST(test_decimal_to_int_maximum_scale_positive) {
  s21_decimal source = make_decimal(1, 0, 0, 28, 0);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_maximum_scale_negative) {
  s21_decimal source = make_decimal(1, 0, 0, 28, 1);

  assert_decimal_to_int_success(source, 0);
}
END_TEST

START_TEST(test_decimal_to_int_int_max) {
  s21_decimal source = make_decimal(0x7FFFFFFFu, 0, 0, 0, 0);

  assert_decimal_to_int_success(source, INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_int_min) {
  s21_decimal source = make_decimal(0x80000000u, 0, 0, 0, 1);

  assert_decimal_to_int_success(source, INT_MIN);
}
END_TEST

START_TEST(test_decimal_to_int_scaled_int_max) {
  s21_decimal source = make_decimal(0xFFFFFFF6u, 4, 0, 1, 0);

  assert_decimal_to_int_success(source, INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_scaled_int_min) {
  s21_decimal source = make_decimal(0, 5, 0, 1, 1);

  assert_decimal_to_int_success(source, INT_MIN);
}
END_TEST

START_TEST(test_decimal_to_int_fraction_truncates_to_int_max) {
  s21_decimal source = make_decimal(UINT32_MAX, 4, 0, 1, 0);

  assert_decimal_to_int_success(source, INT_MAX);
}
END_TEST

START_TEST(test_decimal_to_int_fraction_truncates_to_int_min) {
  s21_decimal source = make_decimal(9, 5, 0, 1, 1);

  assert_decimal_to_int_success(source, INT_MIN);
}
END_TEST

START_TEST(test_decimal_to_int_positive_overflow) {
  s21_decimal source = make_decimal(0x80000000u, 0, 0, 0, 0);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_negative_overflow) {
  s21_decimal source = make_decimal(0x80000001u, 0, 0, 0, 1);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_positive_scaled_overflow) {
  s21_decimal source = make_decimal(0, 5, 0, 1, 0);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_negative_scaled_overflow) {
  s21_decimal source = make_decimal(10, 5, 0, 1, 1);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_maximum_decimal_overflow) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_minimum_decimal_overflow) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);

  assert_decimal_to_int_error(source);
}
END_TEST

START_TEST(test_decimal_to_int_maximum_coefficient_with_maximum_scale) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 0);

  assert_decimal_to_int_success(source, 7);
}
END_TEST

START_TEST(
    test_decimal_to_int_negative_maximum_coefficient_with_maximum_scale) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 1);

  assert_decimal_to_int_success(source, -7);
}
END_TEST

START_TEST(test_decimal_to_float_positive_zero) {
  s21_decimal source = make_decimal(0, 0, 0, 0, 0);

  assert_decimal_to_float_success(source, 0.0f);
}
END_TEST

START_TEST(test_decimal_to_float_negative_zero) {
  s21_decimal source = make_decimal(0, 0, 0, 28, 1);

  assert_decimal_to_float_success(source, 0.0f);
}
END_TEST

START_TEST(test_decimal_to_float_positive_integer) {
  s21_decimal source = make_decimal(42, 0, 0, 0, 0);

  assert_decimal_to_float_exact(source, 42.0f);
}
END_TEST

START_TEST(test_decimal_to_float_negative_integer) {
  s21_decimal source = make_decimal(42, 0, 0, 0, 1);

  assert_decimal_to_float_exact(source, -42.0f);
}
END_TEST

START_TEST(test_decimal_to_float_positive_fraction) {
  s21_decimal source = make_decimal(12345, 0, 0, 3, 0);

  assert_decimal_to_float_success(source, 12.345f);
}
END_TEST

START_TEST(test_decimal_to_float_negative_fraction) {
  s21_decimal source = make_decimal(12345, 0, 0, 3, 1);

  assert_decimal_to_float_success(source, -12.345f);
}
END_TEST

START_TEST(test_decimal_to_float_one_tenth) {
  s21_decimal source = make_decimal(1, 0, 0, 1, 0);

  assert_decimal_to_float_success(source, 0.1f);
}
END_TEST

START_TEST(test_decimal_to_float_minimum_positive_fraction) {
  s21_decimal source = make_decimal(1, 0, 0, 28, 0);

  assert_decimal_to_float_success(source, 1e-28f);
}
END_TEST

START_TEST(test_decimal_to_float_minimum_negative_fraction) {
  s21_decimal source = make_decimal(1, 0, 0, 28, 1);

  assert_decimal_to_float_success(source, -1e-28f);
}
END_TEST

START_TEST(test_decimal_to_float_middle_word) {
  s21_decimal source = make_decimal(0, 1, 0, 0, 0);

  assert_decimal_to_float_exact(source, 0x1p32f);
}
END_TEST

START_TEST(test_decimal_to_float_high_word) {
  s21_decimal source = make_decimal(0, 0, 1, 0, 0);

  assert_decimal_to_float_exact(source, 0x1p64f);
}
END_TEST

START_TEST(test_decimal_to_float_maximum_decimal) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 0);

  assert_decimal_to_float_success(source, 0x1p96f);
}
END_TEST

START_TEST(test_decimal_to_float_minimum_decimal) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 0, 1);

  assert_decimal_to_float_success(source, -0x1p96f);
}
END_TEST

START_TEST(test_decimal_to_float_bank_rounding_even_down) {
  s21_decimal source = make_decimal(16777217, 0, 0, 0, 0);
  float result = NAN;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_msg(result == 16777216.0f, "expected 16777216, got %.9g",
                (double)result);
}
END_TEST

START_TEST(test_decimal_to_float_bank_rounding_even_up) {
  s21_decimal source = make_decimal(16777219, 0, 0, 0, 0);
  float result = NAN;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_msg(result == 16777220.0f, "expected 16777220, got %.9g",
                (double)result);
}
END_TEST

START_TEST(test_decimal_to_float_maximum_coefficient_with_maximum_scale) {
  s21_decimal source = make_decimal(UINT32_MAX, UINT32_MAX, UINT32_MAX, 28, 0);

  assert_decimal_to_float_success(source, 7.9228163f);
}
END_TEST

START_TEST(test_decimal_to_float_scaled_integer) {
  s21_decimal source = make_decimal(12000, 0, 0, 3, 0);

  assert_decimal_to_float_exact(source, 12.0f);
}
END_TEST

START_TEST(test_decimal_to_float_rounds_below_binary_midpoint) {
  s21_decimal source =
      make_decimal(0xAB00A424u, 0x1FF19778u, 0x0000D3C2u, 24, 0);
  float result = 0.0f;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_uint_eq(float_bits(result), 0x3F800002u);
}
END_TEST

START_TEST(test_decimal_to_float_exact_binary_midpoint_rounds_to_even) {
  s21_decimal source =
      make_decimal(0xAB00A425u, 0x1FF19778u, 0x0000D3C2u, 24, 0);
  float result = 0.0f;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_uint_eq(float_bits(result), 0x3F800002u);
}
END_TEST

START_TEST(test_decimal_to_float_rounds_above_binary_midpoint) {
  s21_decimal source =
      make_decimal(0xAB00A426u, 0x1FF19778u, 0x0000D3C2u, 24, 0);
  float result = 0.0f;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_uint_eq(float_bits(result), 0x3F800003u);
}
END_TEST

START_TEST(test_decimal_to_float_negative_midpoint_regression) {
  s21_decimal source =
      make_decimal(0xAB00A424u, 0x1FF19778u, 0x0000D3C2u, 24, 1);
  float result = 0.0f;

  int status = s21_from_decimal_to_float(source, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_uint_eq(float_bits(result), 0xBF800002u);
}
END_TEST

Suite* conversion_suite(void) {
  Suite* suite = suite_create("Conversion");
  TCase* from_int = tcase_create("from_int_to_decimal");
  TCase* from_float = tcase_create("from_float_to_decimal");
  TCase* to_int = tcase_create("from_decimal_to_int");
  TCase* to_float = tcase_create("from_decimal_to_float");

  tcase_add_test(from_int, test_int_to_decimal_positive_integer);
  tcase_add_test(from_int, test_int_to_decimal_negative_integer);
  tcase_add_test(from_int, test_int_to_decimal_zero);
  tcase_add_test(from_int, test_int_to_decimal_int_max);
  tcase_add_test(from_int, test_int_to_decimal_int_min);

  tcase_add_test(from_float, test_float_to_decimal_positive_zero);
  tcase_add_test(from_float, test_float_to_decimal_negative_zero);
  tcase_add_test(from_float, test_float_to_decimal_positive_fraction);
  tcase_add_test(from_float, test_float_to_decimal_negative_fraction);
  tcase_add_test(from_float, test_float_to_decimal_exact_binary_fraction);
  tcase_add_test(from_float, test_float_to_decimal_seven_significant_digits);
  tcase_add_test(from_float,
                 test_float_to_decimal_rounds_to_seven_significant_digits);
  tcase_add_test(from_float, test_float_to_decimal_bank_rounding_even_down);
  tcase_add_test(from_float, test_float_to_decimal_bank_rounding_odd_up);
  tcase_add_test(from_float,
                 test_float_to_decimal_negative_bank_rounding_even_down);
  tcase_add_test(from_float,
                 test_float_to_decimal_negative_bank_rounding_odd_up);
  tcase_add_test(from_float, test_float_to_decimal_minimum_positive_magnitude);
  tcase_add_test(from_float, test_float_to_decimal_minimum_negative_magnitude);
  tcase_add_test(from_float, test_float_to_decimal_small_scientific_value);
  tcase_add_test(from_float,
                 test_float_to_decimal_largest_float_below_overflow);
  tcase_add_test(from_float,
                 test_float_to_decimal_largest_negative_float_below_overflow);
  tcase_add_test(from_float, test_float_to_decimal_too_small_positive);
  tcase_add_test(from_float, test_float_to_decimal_too_small_negative);
  tcase_add_test(from_float, test_float_to_decimal_positive_overflow_boundary);
  tcase_add_test(from_float, test_float_to_decimal_negative_overflow_boundary);
  tcase_add_test(from_float, test_float_to_decimal_flt_max);
  tcase_add_test(from_float, test_float_to_decimal_negative_flt_max);
  tcase_add_test(from_float, test_float_to_decimal_positive_infinity);
  tcase_add_test(from_float, test_float_to_decimal_negative_infinity);
  tcase_add_test(from_float, test_float_to_decimal_nan);

  tcase_add_test(to_int, test_decimal_to_int_zero);
  tcase_add_test(to_int, test_decimal_to_int_negative_scaled_zero);
  tcase_add_test(to_int, test_decimal_to_int_positive_integer);
  tcase_add_test(to_int, test_decimal_to_int_negative_integer);
  tcase_add_test(to_int, test_decimal_to_int_positive_fraction);
  tcase_add_test(to_int, test_decimal_to_int_negative_fraction);
  tcase_add_test(to_int, test_decimal_to_int_positive_fraction_below_one);
  tcase_add_test(to_int, test_decimal_to_int_negative_fraction_above_minus_one);
  tcase_add_test(to_int, test_decimal_to_int_scaled_integer);
  tcase_add_test(to_int, test_decimal_to_int_maximum_scale_positive);
  tcase_add_test(to_int, test_decimal_to_int_maximum_scale_negative);
  tcase_add_test(to_int, test_decimal_to_int_int_max);
  tcase_add_test(to_int, test_decimal_to_int_int_min);
  tcase_add_test(to_int, test_decimal_to_int_scaled_int_max);
  tcase_add_test(to_int, test_decimal_to_int_scaled_int_min);
  tcase_add_test(to_int, test_decimal_to_int_fraction_truncates_to_int_max);
  tcase_add_test(to_int, test_decimal_to_int_fraction_truncates_to_int_min);
  tcase_add_test(to_int, test_decimal_to_int_positive_overflow);
  tcase_add_test(to_int, test_decimal_to_int_negative_overflow);
  tcase_add_test(to_int, test_decimal_to_int_positive_scaled_overflow);
  tcase_add_test(to_int, test_decimal_to_int_negative_scaled_overflow);
  tcase_add_test(to_int, test_decimal_to_int_maximum_decimal_overflow);
  tcase_add_test(to_int, test_decimal_to_int_minimum_decimal_overflow);
  tcase_add_test(to_int,
                 test_decimal_to_int_maximum_coefficient_with_maximum_scale);
  tcase_add_test(
      to_int,
      test_decimal_to_int_negative_maximum_coefficient_with_maximum_scale);

  tcase_add_test(to_float, test_decimal_to_float_positive_zero);
  tcase_add_test(to_float, test_decimal_to_float_negative_zero);
  tcase_add_test(to_float, test_decimal_to_float_positive_integer);
  tcase_add_test(to_float, test_decimal_to_float_negative_integer);
  tcase_add_test(to_float, test_decimal_to_float_positive_fraction);
  tcase_add_test(to_float, test_decimal_to_float_negative_fraction);
  tcase_add_test(to_float, test_decimal_to_float_one_tenth);
  tcase_add_test(to_float, test_decimal_to_float_minimum_positive_fraction);
  tcase_add_test(to_float, test_decimal_to_float_minimum_negative_fraction);
  tcase_add_test(to_float, test_decimal_to_float_middle_word);
  tcase_add_test(to_float, test_decimal_to_float_high_word);
  tcase_add_test(to_float, test_decimal_to_float_maximum_decimal);
  tcase_add_test(to_float, test_decimal_to_float_minimum_decimal);
  tcase_add_test(to_float, test_decimal_to_float_bank_rounding_even_down);
  tcase_add_test(to_float, test_decimal_to_float_bank_rounding_even_up);
  tcase_add_test(to_float,
                 test_decimal_to_float_maximum_coefficient_with_maximum_scale);
  tcase_add_test(to_float, test_decimal_to_float_scaled_integer);
  tcase_add_test(to_float, test_decimal_to_float_rounds_below_binary_midpoint);
  tcase_add_test(to_float,
                 test_decimal_to_float_exact_binary_midpoint_rounds_to_even);
  tcase_add_test(to_float, test_decimal_to_float_rounds_above_binary_midpoint);
  tcase_add_test(to_float, test_decimal_to_float_negative_midpoint_regression);

  suite_add_tcase(suite, from_int);
  suite_add_tcase(suite, from_float);
  suite_add_tcase(suite, to_int);
  suite_add_tcase(suite, to_float);

  return suite;
}

#include "s21_decimal.h"
#include "s21_helpers.h"

static int s21_compare_decimal(s21_decimal first, s21_decimal second) {
  s21_big_decimal first_big = s21_transfer(first);
  s21_big_decimal second_big = s21_transfer(second);
  int result = !(s21_is_zero(first_big) && s21_is_zero(second_big));
  if (result) {
    int sign_first = s21_get_sign(first);
    int sign_second = s21_get_sign(second);
    if (sign_first > sign_second) {
      result = -1;
    } else if (sign_first < sign_second) {
      result = 1;
    } else {
      s21_align(&first_big, &second_big, s21_get_scale(first),
                s21_get_scale(second));
      result = s21_compare_big_mantissa(first_big, second_big);
    }
    if (sign_first == 1 && sign_second == 1) {
      result = -result;
    }
  }
  return result;
}

int s21_is_less(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) == -1) {
    result = 1;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) <= 0) {
    result = 1;
  }
  return result;
}

int s21_is_greater(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) == 1) {
    result = 1;
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) >= 0) {
    result = 1;
  }
  return result;
}

int s21_is_equal(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) == 0) {
    result = 1;
  }
  return result;
}

int s21_is_not_equal(s21_decimal first, s21_decimal second) {
  int result = 0;
  if (s21_compare_decimal(first, second) != 0) {
    result = 1;
  }
  return result;
}

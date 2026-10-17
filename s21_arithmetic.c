#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_add(s21_decimal first, s21_decimal second, s21_decimal* result) {
  int error_code = 0;
  if (result == 0) {
    error_code = 1;
  } else {
    int sign_first = s21_get_sign(first);
    int sign_second = s21_get_sign(second);
    int sign_result = sign_first;
    int scale_first = s21_get_scale(first);
    int scale_second = s21_get_scale(second);
    s21_big_decimal first_big = s21_transfer(first);
    s21_big_decimal second_big = s21_transfer(second);
    s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int scale_result =
        s21_align(&first_big, &second_big, scale_first, scale_second);
    if (sign_first == sign_second) {
      s21_add_big_mantissa(first_big, second_big, &result_big);
    } else {
      int compare = s21_compare_big_mantissa(first_big, second_big);
      if (compare >= 0) {
        s21_sub_big_mantissa(first_big, second_big, &result_big);
      } else {
        s21_sub_big_mantissa(second_big, first_big, &result_big);
        sign_result = sign_second;
      }
    }
    error_code = s21_normalize(&result_big, &scale_result, sign_result, 0);
    if (s21_is_zero(result_big)) {
      sign_result = 0;
    }
    if (!error_code) {
      *result = s21_untransfer(result_big);
      s21_set_scale(result, scale_result);
      s21_set_sign(result, sign_result);
    }
  }
  return error_code;
}

int s21_sub(s21_decimal first, s21_decimal second, s21_decimal* result) {
  s21_set_sign(&second, !s21_get_sign(second));
  int error_code = s21_add(first, second, result);
  return error_code;
}

int s21_mul(s21_decimal first, s21_decimal second, s21_decimal* result) {
  int error_code = 0;
  if (result == 0) {
    error_code = 1;
  } else {
    int sign_first = s21_get_sign(first);
    int sign_second = s21_get_sign(second);
    int sign_result = sign_first ^ sign_second;
    int scale_first = s21_get_scale(first);
    int scale_second = s21_get_scale(second);
    s21_big_decimal first_big = s21_transfer(first);
    s21_big_decimal second_big = s21_transfer(second);
    s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int scale_result = scale_first + scale_second;
    s21_mul_mantissa(first_big, second_big, &result_big);
    error_code = s21_normalize(&result_big, &scale_result, sign_result, 0);
    if (s21_is_zero(result_big)) {
      sign_result = 0;
    }
    if (!error_code) {
      *result = s21_untransfer(result_big);
      s21_set_scale(result, scale_result);
      s21_set_sign(result, sign_result);
    }
  }
  return error_code;
}

int s21_div(s21_decimal first, s21_decimal second, s21_decimal* result) {
  int error_code = 0;
  if (result == 0) {
    error_code = 1;
  } else {
    int sign_first = s21_get_sign(first);
    int sign_second = s21_get_sign(second);
    int sign_result = sign_first ^ sign_second;
    int scale_first = s21_get_scale(first);
    int scale_second = s21_get_scale(second);
    s21_big_decimal first_big = s21_transfer(first);
    s21_big_decimal second_big = s21_transfer(second);
    if (s21_is_zero(second_big)) {
      error_code = 3;
    } else {
      s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0, 0}};
      s21_align(&first_big, &second_big, scale_first, scale_second);
      int scale_result = 0;
      s21_big_decimal remainder =
          s21_big_div(first_big, second_big, &result_big);
      if (s21_overflow(&result_big)) {
        error_code = sign_result ? 2 : 1;
      }
      while (!error_code && !s21_is_zero(remainder) && scale_result < 29) {
        remainder = s21_helper_align(remainder, 1);
        result_big = s21_helper_align(result_big, 1);
        s21_big_decimal digit = {{0, 0, 0, 0, 0, 0, 0, 0}};
        remainder = s21_big_div(remainder, second_big, &digit);
        s21_add_big_mantissa(result_big, digit, &result_big);
        scale_result++;
      }
      if (!error_code) {
        int sticky = !s21_is_zero(remainder);
        error_code =
            s21_normalize(&result_big, &scale_result, sign_result, sticky);
      }
      if (!error_code) {
        if (s21_is_zero(result_big)) sign_result = 0;
        *result = s21_untransfer(result_big);
        s21_set_scale(result, scale_result);
        s21_set_sign(result, sign_result);
      }
    }
  }
  return error_code;
}

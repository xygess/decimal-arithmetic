#include <limits.h>
#include <math.h>

#include "s21_decimal.h"
#include "s21_helpers.h"

static int s21_has_fraction(s21_decimal value) {
  s21_big_decimal mantissa = s21_transfer(value);
  int scale = s21_get_scale(value);
  int fraction = 0;

  while (scale > 0) {
    uint32_t remainder = s21_big_div_10(&mantissa);
    fraction = fraction || remainder != 0u;
    scale--;
  }
  return fraction;
}

static void s21_increment_mantissa(s21_decimal* num) {
  uint64_t value = (uint64_t)(uint32_t)num->bits[0] + 1u;

  num->bits[0] = (int)(uint32_t)value;
  uint32_t carry = (uint32_t)(value >> 32);

  value = (uint64_t)(uint32_t)num->bits[1] + carry;
  num->bits[1] = (int)(uint32_t)value;
  carry = (uint32_t)(value >> 32);

  value = (uint64_t)(uint32_t)num->bits[2] + carry;
  num->bits[2] = (int)(uint32_t)value;
}

static uint32_t s21_last_decimal_digit(s21_decimal value) {
  s21_big_decimal mantissa = s21_transfer(value);
  int scale = s21_get_scale(value);
  uint32_t remainder = 0;

  while (scale > 0) {
    remainder = s21_big_div_10(&mantissa);
    scale--;
  }
  return remainder;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int error = result == 0;
  if (!error) {
    s21_big_decimal mantissa = s21_transfer(value);
    int scale = s21_get_scale(value);
    while (scale > 0) {
      s21_big_div_10(&mantissa);
      scale--;
    }
    *result = s21_untransfer(mantissa);
    s21_set_sign(result, s21_get_sign(value));
    s21_set_scale(result, 0);
  }
  return error;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  int error = s21_truncate(value, result);
  int negative = s21_get_sign(value);
  int fraction = s21_has_fraction(value);

  if (!error && negative && fraction) {
    s21_increment_mantissa(result);
    s21_set_sign(result, 1);
  }
  return error;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int error = s21_truncate(value, result);
  uint32_t last_digit = s21_last_decimal_digit(value);

  if (!error && last_digit >= 5u) {
    s21_increment_mantissa(result);
  }
  if (!error) {
    s21_set_sign(result, s21_get_sign(value));
    s21_set_scale(result, 0);
  }
  return error;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  int error = result == 0;
  if (!error) {
    *result = value;
    s21_set_sign(result, !s21_get_sign(value));
  }
  return error;
}

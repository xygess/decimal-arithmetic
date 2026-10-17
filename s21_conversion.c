#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int error = 0;
  if (dst == 0) {
    error = 1;
  } else {
    s21_zero_decimal(dst);
    uint32_t magnitude = src < 0 ? (uint32_t)(-(int64_t)src) : (uint32_t)src;
    dst->bits[0] = (int)magnitude;
    if (src < 0) {
      s21_set_sign(dst, 1);
    }
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    s21_big_decimal value = s21_transfer(src);
    int scale = s21_get_scale(src);
    int sign = s21_get_sign(src);
    for (int i = 0; i < scale; i++) {
      s21_big_div_10(&value);
    }
    if (value.bits[1] != 0 || value.bits[2] != 0 || value.bits[3] != 0 ||
        value.bits[4] != 0 || value.bits[5] != 0 || value.bits[6] != 0 ||
        value.bits[7] != 0) {
      error = 1;
    } else {
      uint32_t magnitude = value.bits[0];
      if ((!sign && magnitude > (uint32_t)INT_MAX) ||
          (sign && magnitude > (uint32_t)INT_MAX + 1u)) {
        error = 1;
      } else if (sign) {
        *dst = (int)(-(int64_t)magnitude);
      } else {
        *dst = (int)magnitude;
      }
    }
  }
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  }
  if (!error) {
    s21_zero_decimal(dst);
    error = s21_float_is_valid_decimal(src);
  }
  if (!error && src != 0.0f) {
    int sign = signbit(src) ? 1 : 0;
    long double abs_src = fabsl((long double)src);
    int exponent = (int)floorl(log10l(abs_src));
    long double normalized = abs_src / powl(10.0L, exponent);
    uint32_t significand =
        s21_round_half_even_positive(normalized * 1000000.0L);
    if (significand == 10000000u) {
      significand = 1000000u;
      exponent++;
    }
    int scale = 6 - exponent;
    while (scale > 0 && significand % 10u == 0u) {
      significand /= 10u;
      scale--;
    }
    s21_big_decimal big = {{significand, 0, 0, 0, 0, 0, 0, 0}};
    if (scale < 0) {
      big = s21_helper_align(big, -scale);
      scale = 0;
    }
    if (s21_normalize(&big, &scale, sign, 0)) {
      error = 1;
    }
    if (!error) {
      *dst = s21_untransfer(big);
      s21_set_scale(dst, scale);
      s21_set_sign(dst, sign);
    }
  }
  if (!error && src == 0.0f && signbit(src)) {
    s21_set_sign(dst, 1);
  }
  return error;
}

static int s21_big_bit_length(s21_big_decimal value) {
  int length = 0;
  for (int i = 7; i >= 0 && length == 0; i--) {
    if (value.bits[i] != 0u) {
      uint32_t word = value.bits[i];
      length = i * 32;
      while (word != 0u) {
        word >>= 1;
        length++;
      }
    }
  }
  return length;
}

static s21_big_decimal s21_big_shift_left(s21_big_decimal value, int shift) {
  for (int step = 0; step < shift; step++) {
    uint32_t carry = 0;
    for (int i = 0; i < 8; i++) {
      uint64_t current = ((uint64_t)value.bits[i] << 1) | carry;
      value.bits[i] = (uint32_t)current;
      carry = (uint32_t)(current >> 32);
    }
  }
  return value;
}

static int s21_binary_exponent(s21_big_decimal numerator,
                               s21_big_decimal denominator) {
  int exponent =
      s21_big_bit_length(numerator) - s21_big_bit_length(denominator);
  int compare = 0;

  if (exponent >= 0) {
    s21_big_decimal scaled_denominator =
        s21_big_shift_left(denominator, exponent);
    compare = s21_compare_big_mantissa(numerator, scaled_denominator);
  } else {
    s21_big_decimal scaled_numerator = s21_big_shift_left(numerator, -exponent);
    compare = s21_compare_big_mantissa(scaled_numerator, denominator);
  }

  if (compare < 0) {
    exponent--;
  }
  return exponent;
}

static uint32_t s21_round_binary_ratio(s21_big_decimal numerator,
                                       s21_big_decimal denominator) {
  s21_big_decimal quotient = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal remainder = s21_big_div(numerator, denominator, &quotient);
  uint32_t result = quotient.bits[0];
  s21_big_decimal twice_remainder = s21_big_shift_left(remainder, 1);
  int compare = s21_compare_big_mantissa(twice_remainder, denominator);

  if (compare > 0 || (compare == 0 && (result & 1u))) {
    result++;
  }
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int error = dst == NULL;
  if (!error) {
    int sign = s21_get_sign(src);
    s21_big_decimal numerator = s21_transfer(src);
    uint32_t float_bits = (uint32_t)sign << 31;

    if (!s21_is_zero(numerator)) {
      int scale = s21_get_scale(src);
      s21_big_decimal denominator = {{1, 0, 0, 0, 0, 0, 0, 0}};
      denominator = s21_helper_align(denominator, scale);

      int exponent = s21_binary_exponent(numerator, denominator);
      int shift = 23 - exponent;
      if (shift >= 0) {
        numerator = s21_big_shift_left(numerator, shift);
      } else {
        denominator = s21_big_shift_left(denominator, -shift);
      }

      uint32_t significand = s21_round_binary_ratio(numerator, denominator);
      if (significand == (1u << 24)) {
        significand >>= 1;
        exponent++;
      }

      uint32_t biased_exponent = (uint32_t)(exponent + 127);
      float_bits |= biased_exponent << 23;
      float_bits |= significand & 0x7FFFFFu;
    }

    memcpy(dst, &float_bits, sizeof(float_bits));
  }
  return error;
}

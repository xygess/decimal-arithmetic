#include <math.h>

#include "s21_helpers.h"

void s21_zero_decimal(s21_decimal* num) {
  for (int i = 0; i < 4; i++) {
    num->bits[i] = 0;
  }
}

int s21_get_sign(s21_decimal num) { return ((uint32_t)num.bits[3] >> 31) & 1u; }

void s21_set_sign(s21_decimal* num, int mode) {
  uint32_t metadata = (uint32_t)num->bits[3];
  uint32_t mask = 1u << 31;
  if (mode) {
    metadata |= mask;
  } else {
    metadata &= ~mask;
  }
  num->bits[3] = (int)metadata;
}

int s21_get_scale(s21_decimal num) {
  return (int)(((uint32_t)num.bits[3] >> 16) & 0xFFu);
}

void s21_set_scale(s21_decimal* num, int scale) {
  if (0 <= scale && scale <= 28) {
    uint32_t metadata = (uint32_t)num->bits[3];
    uint32_t mask = 0xFFu << 16;
    metadata &= ~mask;
    metadata |= (uint32_t)scale << 16;
    num->bits[3] = (int)metadata;
  }
}

s21_big_decimal s21_transfer(s21_decimal num) {
  s21_big_decimal result_mantissa = {{0, 0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) {
    result_mantissa.bits[i] = (uint32_t)num.bits[i];
  }
  return result_mantissa;
}

s21_decimal s21_untransfer(s21_big_decimal num) {
  s21_decimal result_mantissa = {{0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) {
    result_mantissa.bits[i] = (int)num.bits[i];
  }
  return result_mantissa;
}

s21_big_decimal s21_helper_align(s21_big_decimal num, int pow) {
  for (int j = 0; j < pow; j++) {
    uint32_t carry = 0;
    for (int i = 0; i < 8; i++) {
      uint64_t full_sum = (uint64_t)num.bits[i] * 10u + carry;
      num.bits[i] = (uint32_t)full_sum;
      carry = (uint32_t)(full_sum >> 32);
    }
  }
  return num;
}

int s21_align(s21_big_decimal* first, s21_big_decimal* second, int scale_first,
              int scale_second) {
  int result = scale_first;
  if (scale_first > scale_second) {
    *second = s21_helper_align(*second, scale_first - scale_second);
  } else if (scale_first < scale_second) {
    *first = s21_helper_align(*first, scale_second - scale_first);
    result = scale_second;
  }
  return result;
}

int s21_compare_big_mantissa(s21_big_decimal first, s21_big_decimal second) {
  int result = 0;
  for (int i = 7; i >= 0 && result == 0; i--) {
    if (first.bits[i] > second.bits[i]) {
      result = 1;
    } else if (first.bits[i] < second.bits[i]) {
      result = -1;
    }
  }
  return result;
}

void s21_add_big_mantissa(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal* result_mantissa) {
  uint32_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t full_sum = (uint64_t)first.bits[i] + second.bits[i] + carry;
    result_mantissa->bits[i] = (uint32_t)full_sum;
    carry = (uint32_t)(full_sum >> 32);
  }
}

void s21_sub_big_mantissa(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal* result_mantissa) {
  uint32_t borrow = 0;
  for (int i = 0; i < 8; i++) {
    uint32_t diff = first.bits[i] - second.bits[i] - borrow;
    if ((uint64_t)second.bits[i] + borrow > first.bits[i]) {
      borrow = 1;
    } else {
      borrow = 0;
    }
    result_mantissa->bits[i] = diff;
  }
}

int s21_overflow(const s21_big_decimal* num) {
  int overflow = 0;
  for (int i = 3; i < 8 && !overflow; i++) {
    if (num->bits[i] != 0) {
      overflow = 1;
    }
  }
  return overflow;
}

uint32_t s21_big_div_10(s21_big_decimal* num) {
  uint64_t remainder = 0;
  for (int i = 7; i >= 0; i--) {
    uint64_t current = (remainder << 32) | num->bits[i];
    num->bits[i] = (uint32_t)(current / 10u);
    remainder = current % 10u;
  }
  return (uint32_t)remainder;
}

static void s21_banking_round(s21_big_decimal* num, uint32_t remainder,
                              int sticky) {
  int round_up =
      remainder > 5u || (remainder == 5u && (sticky || (num->bits[0] & 1u)));
  if (round_up) {
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
    s21_add_big_mantissa(*num, one, num);
  }
}

int s21_normalize(s21_big_decimal* num, int* scale, int sign, int sticky) {
  int result = 0;
  uint32_t remainder = 0;
  while ((s21_overflow(num) || *scale > 28) && !result) {
    if (*scale > 0) {
      if (remainder != 0) {
        sticky = 1;
      }
      remainder = s21_big_div_10(num);
      (*scale)--;
    } else {
      result = sign ? 2 : 1;
    }
  }
  if (!result) {
    s21_banking_round(num, remainder, sticky);
    if (s21_overflow(num)) {
      if (*scale > 0) {
        remainder = s21_big_div_10(num);
        (*scale)--;
        s21_banking_round(num, remainder, 0);
      } else {
        result = sign ? 2 : 1;
      }
    }
  }
  return result;
}

int s21_float_is_valid_decimal(float src) {
  int error = 0;

  if (!isfinite(src)) {
    error = 1;
  } else {
    long double abs_src = fabsl((long double)src);

    if ((abs_src > 0.0L && abs_src < S21_MIN_FLOAT_DECIMAL) ||
        abs_src >= S21_MAX_DECIMAL) {
      error = 1;
    }
  }

  return error;
}

uint32_t s21_round_half_even_positive(long double value) {
  uint32_t integer = (uint32_t)value;
  long double fraction = value - (long double)integer;

  const long double eps = 1e-9L;

  if (fraction > 0.5L + eps ||
      (fabsl(fraction - 0.5L) <= eps && (integer & 1u))) {
    integer++;
  }

  return integer;
}

int s21_is_zero(s21_big_decimal num) {
  int result = 1;
  for (int i = 0; i < 8; i++) {
    if (num.bits[i]) {
      result = 0;
    }
  }
  return result;
}

void s21_mul_mantissa(s21_big_decimal first, s21_big_decimal second,
                      s21_big_decimal* result_mantissa) {
  for (int i = 0; i < 3; i++) {
    uint32_t carry = 0;
    for (int j = 0; j < 3; j++) {
      uint64_t full_sum = (uint64_t)first.bits[i] * second.bits[j] +
                          result_mantissa->bits[i + j] + carry;
      result_mantissa->bits[i + j] = (uint32_t)full_sum;
      carry = (uint32_t)(full_sum >> 32);
    }
    result_mantissa->bits[i + 3] += carry;
  }
}

static s21_big_decimal s21_to_left(s21_big_decimal num) {
  uint32_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t full_sum = (uint64_t)num.bits[i] * 2u + carry;
    num.bits[i] = (uint32_t)full_sum;
    carry = (uint32_t)(full_sum >> 32);
  }
  return num;
}

s21_big_decimal s21_big_div(s21_big_decimal first, s21_big_decimal second,
                            s21_big_decimal* result) {
  s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0, 0}};
  for (int i = 7; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      remainder = s21_to_left(remainder);
      remainder.bits[0] |= ((first.bits[i] & (1u << j)) >> j);
      if (s21_compare_big_mantissa(remainder, second) >= 0) {
        s21_sub_big_mantissa(remainder, second, &remainder);
        result->bits[i] |= (1u << j);
      }
    }
  }
  return remainder;
}

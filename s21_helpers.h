#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdint.h>

#include "s21_decimal.h"

#define S21_MAX_DECIMAL 79228162514264337593543950335.0L
#define S21_MIN_FLOAT_DECIMAL 1e-28L

typedef struct {
  uint32_t bits[8];
} s21_big_decimal;

void s21_zero_decimal(s21_decimal* num);

int s21_get_sign(s21_decimal num);
void s21_set_sign(s21_decimal* num, int mode);

int s21_get_scale(s21_decimal num);
void s21_set_scale(s21_decimal* num, int scale);

s21_big_decimal s21_transfer(s21_decimal num);
s21_decimal s21_untransfer(s21_big_decimal num);

s21_big_decimal s21_helper_align(s21_big_decimal num, int pow);
int s21_align(s21_big_decimal* first, s21_big_decimal* second, int scale_first,
              int scale_second);

int s21_compare_big_mantissa(s21_big_decimal first, s21_big_decimal second);
void s21_add_big_mantissa(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal* result_mantissa);
void s21_sub_big_mantissa(s21_big_decimal first, s21_big_decimal second,
                          s21_big_decimal* result_mantissa);
void s21_mul_mantissa(s21_big_decimal first, s21_big_decimal second,
                      s21_big_decimal* result_mantissa);

int s21_overflow(const s21_big_decimal* num);
uint32_t s21_big_div_10(s21_big_decimal* num);
int s21_normalize(s21_big_decimal* num, int* scale, int sign, int sticky);

int s21_float_is_valid_decimal(float src);
uint32_t s21_round_half_even_positive(long double value);

int s21_is_zero(s21_big_decimal num);

s21_big_decimal s21_big_div(s21_big_decimal first, s21_big_decimal second,
                            s21_big_decimal* result);

#endif

#ifndef TEST_HELPERS_H_
#define TEST_HELPERS_H_

#include <stdint.h>

#include "s21_decimal.h"
#include "test_s21_decimal.h"

typedef struct {
  uint32_t words[3];
  uint32_t scale;
  uint32_t sign;
} decimal_parts;

s21_decimal make_decimal(uint32_t low, uint32_t middle, uint32_t high,
                         int scale, int is_negative);

uint32_t divide_96_by_10(uint32_t words[3]);

decimal_parts canonicalize_decimal(s21_decimal value);

void assert_decimal_value_equal(s21_decimal actual, s21_decimal expected);
void assert_decimal_scale_zero(s21_decimal value);

#endif  // TEST_HELPERS_H_

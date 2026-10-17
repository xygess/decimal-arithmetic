#include "test_helpers.h"

s21_decimal make_decimal(uint32_t low, uint32_t middle, uint32_t high,
                         int scale, int is_negative) {
  s21_decimal value = {{(int)low, (int)middle, (int)high, 0}};
  uint32_t control = (uint32_t)scale << 16;
  if (is_negative) {
    control |= 1u << 31;
  }
  value.bits[3] = (int)control;
  return value;
}

uint32_t divide_96_by_10(uint32_t words[3]) {
  uint64_t remainder = 0;
  for (int index = 2; index >= 0; index--) {
    uint64_t current = (remainder << 32) | words[index];
    words[index] = (uint32_t)(current / 10u);
    remainder = current % 10u;
  }
  return (uint32_t)remainder;
}

decimal_parts canonicalize_decimal(s21_decimal value) {
  uint32_t metadata = (uint32_t)value.bits[3];
  decimal_parts parts = {
      .words = {(uint32_t)value.bits[0], (uint32_t)value.bits[1],
                (uint32_t)value.bits[2]},
      .scale = (metadata >> 16) & 0xFFu,
      .sign = metadata >> 31,
  };

  ck_assert_msg((metadata & 0x7F00FFFFu) == 0,
                "decimal contains nonzero reserved bits");
  ck_assert_msg(parts.scale <= 28, "decimal scale is greater than 28");

  while (parts.scale > 0) {
    uint32_t quotient[3] = {parts.words[0], parts.words[1], parts.words[2]};
    if (divide_96_by_10(quotient) != 0) {
      break;
    }
    for (int index = 0; index < 3; index++) {
      parts.words[index] = quotient[index];
    }
    parts.scale--;
  }

  if ((parts.words[0] | parts.words[1] | parts.words[2]) == 0) {
    parts.scale = 0;
    parts.sign = 0;
  }

  return parts;
}

void assert_decimal_value_equal(s21_decimal actual, s21_decimal expected) {
  decimal_parts actual_parts = canonicalize_decimal(actual);
  decimal_parts expected_parts = canonicalize_decimal(expected);

  for (int index = 0; index < 3; index++) {
    ck_assert_uint_eq(actual_parts.words[index], expected_parts.words[index]);
  }
  ck_assert_uint_eq(actual_parts.scale, expected_parts.scale);
  ck_assert_uint_eq(actual_parts.sign, expected_parts.sign);
}

void assert_decimal_scale_zero(s21_decimal value) {
  uint32_t metadata = (uint32_t)value.bits[3];
  ck_assert_uint_eq((metadata >> 16) & 0xFFu, 0u);
}

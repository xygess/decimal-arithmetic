CC := gcc
AR := ar
ARFLAGS := rcs

CFLAGS = -std=c11 -Wall -Wextra -Werror
CPPFLAGS := -I.

LIB := s21_decimal.a
HEADERS := s21_decimal.h s21_helpers.h

SRC := helpers.c \
	   s21_arithmetic.c \
       s21_comparison.c \
       s21_conversion.c \
       s21_other.c

OBJ := $(SRC:.c=.o)

TEST_DIR := tests
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_HEADER := $(wildcard $(TEST_DIR)/*.h)
TEST_BIN := test_decimal

CHECK_CFLAGS := $(shell pkg-config --cflags check)
CHECK_LIBS := $(shell pkg-config --libs check) -lm

GCOV_FLAGS := --coverage
REPORT_DIR := gcov_report

.PHONY: all clean test gcov_report rebuild

all: $(LIB)

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean
	$(MAKE) CFLAGS="$(CFLAGS) $(GCOV_FLAGS)" test
	@mkdir -p $(REPORT_DIR)
	@lcov --no-external --capture --directory . \
		--output-file $(REPORT_DIR)/coverage.info
	@lcov --remove $(REPORT_DIR)/coverage.info '*/tests/*' \
		--output-file $(REPORT_DIR)/coverage.filtered.info
	@genhtml $(REPORT_DIR)/coverage.filtered.info \
		--output-directory $(REPORT_DIR)
	@echo "Coverage report: src/$(REPORT_DIR)/index.html"

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(TEST_HEADER) $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CHECK_CFLAGS) $(TEST_SRC) $(LIB) $(CHECK_LIBS) -o $@

clean:
	rm -f $(LIB) $(OBJ) $(TEST_BIN)
	rm -f *.gcno *.gcda *.gcov coverage.info coverage.filtered.info
	rm -f $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcov
	rm -rf $(REPORT_DIR)

rebuild: clean all

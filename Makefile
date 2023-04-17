# Commands and flags to be used
CC := gcc
CFLAGS := -fprofile-arcs -ftest-coverage
LDFLAGS := -lgcov --coverage
MKDIR_P := mkdir -p

# Main compilation target and its source files
MAIN_TARGET := mines
MAIN_SRCS := main.c game.c board.c hof.c ui.c view.c str_utils.c sb.c

# Tests compilation target and its source files
TEST_TARGET := tests/all_tests
TEST_SRCS := game.c board.c hof.c view.c str_utils.c sb.c \
	 tests/all_tests.c tests/test_game.c tests/test_board.c tests/test_view.c \
	 tests/test_hof.c tests/test_str_utils.c

# Build directory
BUILD_DIR := ./build
# Names of object files
MAIN_OBJS := $(addprefix $(BUILD_DIR)/,$(MAIN_SRCS:.c=.o))
TEST_OBJS := $(addprefix $(BUILD_DIR)/,$(TEST_SRCS:.c=.o))
ALL_OBJS = $(MAIN_OBJS) $(TEST_OBJS)

# Main tasks
.PHONY: all build build_tests test coverage clean

# Build everything
all: build build_tests

# Build game
build: $(BUILD_DIR)/$(MAIN_TARGET)

# Build tests
build_tests: $(BUILD_DIR)/$(TEST_TARGET)

# Run tests
test: build/tests/all_tests
	$<  # Just run the source

# Calculate test coverage
coverage:
	gcov $(MAIN_OBJS)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) *.gcov

# Compiling targets
$(BUILD_DIR)/$(MAIN_TARGET): $(MAIN_OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

# Compiling individual modules
$(BUILD_DIR)/%.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

# Include dependency information generated by GCC
DEPS := $(ALL_OBJS:.o=.d)
-include $(DEPS)

# This Makefile is inspired by
# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
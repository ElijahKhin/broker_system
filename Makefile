NAME := libbroker_system.a

SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := tests
BIN_DIR := bin
OBJ_DIR := obj
SAMPLE_DIR := code-samples

SRC := $(wildcard $(SRC_DIR)/*.cc)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)

SRC_NAMES := $(notdir $(SRC))
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC_NAMES:%.cc=%.o))
DEP := $(OBJ:.o=.d)

CXX := g++
CXXFLAGS := -std=c++20 -I$(INCLUDE_DIR) -MMD -MP -Werror -Wextra -Wall
LDFLAGS := -lgtest -lgtest_main -pthread

BUILD ?= release

ifeq ($(BUILD), debug)
	CXXFLAGS += -g -O0 -DDEBUG
else
	CXXFLAGS += -O2
endif

# --- TESTING ---
TESTS := $(wildcard $(TEST_DIR)/*.cc)
#GT_FILTER := "MultiThread.*"
GT_FILTER := "*"
TESTS_BIN := $(BIN_DIR)/tests_bin
GCOV_REPORT_NAME := broker_system_report

# --- COLORS FOR A GOOD-LOOKING ASSEMBLING ---
GREEN := \033[32m
YELLOW := \033[0;33m
RED := \033[0;31m
GRAY := \033[2;37m
CURSIVE := \033[3m
NO_COLOR := \x1b[0m
RESET := \033[0m

# --- JUST TRASH --- 
GCOV_TRASH := $(GCOV_REPORT_NAME) $(GCOV_REPORT_NAME).info *.gcda *.gcno report  *.gch
OTHER_TRASH := obj *.a a.out .*.swp .DS_Store $(NAME) units $(BIN_DIR)

.DEFAULT_GOAL := all

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)
	@printf "$(CURSIVE)Wait...⌛ $(YELLOW)%s$(NO_COLOR) successfully assembled! %-37s" $(NAME)""
	@echo "\n"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(HEADERS) Makefile
	@$(CXX) -c  $(CXXFLAGS) $< -o $@
	@printf "$(CURSIVE)Compiling source files 🚀... $(YELLOW)%-13s $(NO_COLOR)---> %17.33s ✅\r" $< $@

clean:
	@rm -rf $(OBJ_FOLDER)
.PHONY: clean

clean_gcov:
	@rm -rf  $(GCOV_TRASH)
.PHONY: clean_gcov

fclean: clean clean_gcov
	@echo "$(RED)Cleaning $(NAME) & executable..."
	@rm -rf $(NAME) $(OBJ_FOLDER) ./a.out
	@rm -rf $(OTHER_TRASH)
	@echo "$(GRAY)$(CURSIVE)---$(NAME) staff is completely cleaned!\n$(RESET)"
.PHONY: fclean

re: fclean all
.PHONY: re

#------------------------------------------------------------------------------#

launch_sample: all
	@$(CXX) -std=c++20 -I$(INCLUDE_DIR) $(SAMPLE_DIR)/optional.cc -L. $(NAME)
	@./a.out

tests: all $(BIN_DIR)
	@$(CXX) -std=c++20  -I$(INCLUDE_DIR) $(TESTS) $(NAME) $(LDFLAGS) -o $(TESTS_BIN)
.PHONY: test

$(BIN_DIR):
	@mkdir $(BIN_DIR)

show_tests_result: fclean tests
	@./$(TESTS_BIN) --gtest_filter=$(GT_FILTER)
.PHONY: show_tests_result

leaks: tests
	@leaks -quiet --atExit -- ./$(TESTS_BIN) --gtest_filter=$(GT_FILTER)
.PHONY: leaks

correct_format:
	@clang-format -i $(shell find . -name "*.cc" -o -name "*.h" -o -name "*.tpp")
.PHONY: correct_format

check_format:
	@clang-format --dry-run --Werror $(shell find . -name "*.cc" -o -name "*.h" -o -name "*.tpp")
.PHONY: check_format
	
report/index.html: tests
	@$(CXX) -std=c++20 -I$(INCLUDE_DIR) -Wno-deprecated -lgtest --coverage $(TESTS) $(SRC) $(NAME) -o $(GCOV_REPORT_NAME)
	@./$(GCOV_REPORT_NAME)
	@lcov --no-external --no-recursion -c --directory . -o $(GCOV_REPORT_NAME).info
	@genhtml -o report $(GCOV_REPORT_NAME).info

open_gcov_report: report/index.html $(TESTS)
	@open report/index.html
.PHONY: open_gcov_report

-include $(DEP)

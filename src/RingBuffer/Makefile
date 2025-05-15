NAME = RingBuffer.a

SRC = RingBuffer.cc
HEADER = RingBuffer.hpp

OBJ_FOLDER = obj
OBJ = $(SRC:%.cc=$(OBJ_FOLDER)/%.o)

CC = g++
FLAGS = -std=c++20 -I.

# --- COLORS FOR A GOOD-LOOKING ASSEMBLING ---
GREEN = \033[32m
YELLOW = \033[0;33m
RED = \033[0;31m
GRAY = \033[2;37m
CURSIVE = \033[3m
NO_COLOR = \x1b[0m
RESET = \033[0m

.DEFAULT_GOAL: $(NAME)

$(NAME): $(OBJ_FOLDER) $(OBJ)
	@ar rc $(NAME) obj/*
	@ranlib $(NAME)
	@printf "$(CURSIVE)Wait...⌛ $(YELLOW)%s$(NO_COLOR) successfully assembled! %-37s" $(NAME)""
	@echo "\n"

$(OBJ_FOLDER):
	@mkdir $(OBJ_FOLDER)

$(OBJ_FOLDER)/%.o: %.cc $(HEADER) Makefile
	@$(CC) -c  $(FLAGS) $< -o $@
	@printf "$(CURSIVE)Compiling source files 🚀... $(YELLOW)%-13s $(NO_COLOR)---> %17.33s ✅\r" $< $@

fclean:
	@echo "$(RED)Cleaning $(NAME) & executable..."
	@rm -rf $(NAME) $(OBJ_FOLDER) ./a.out
	@echo "$(GRAY)$(CURSIVE)---$(NAME) staff is completely cleaned!\n$(RESET)"
.PHONY: fclean

launch_main: all
	@$(CC) -std=c++20  main.cc -L. $(NAME)
	@./a.out

re: fclean .DEFAULT_GOAL
.PHONY: re

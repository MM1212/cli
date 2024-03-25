
PROJECT_NAME = CLI

ARCH_DIR = bin
NAME =	$(addprefix $(ARCH_DIR)/, libcli.a)

SRC_FILES = $(shell find src -type f -name '*.c' | sed 's/src\///g')

SRC_DIR = src

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = objs
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

DEP_DIR = deps
DEP_FILES = $(addprefix $(DEP_DIR)/, $(SRCS:.c=.d))

LIBFT_PATH = libs/libft
LIBFT_BIN = $(addprefix $(LIBFT_PATH)/,bin)
LIBFT_INCLUDES = $(addprefix $(LIBFT_PATH)/,includes/)
LIBFT_ARCH = $(addprefix $(LIBFT_BIN)/,libft.a)

INCLUDES = -Iincludes -I. -I$(LIBFT_INCLUDES)

CC = clang
CFLAGS = \
		$(INCLUDES) \
		-MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d \
		-Wall -Wextra -Werror \
		-g -fsanitize=address,undefined


### COLORS ###

RED = \033[0;31m
GREEN = \033[0;92m
YELLOW = \033[93m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[96m
RESET = \033[0m

TAG = [$(CYAN)$(PROJECT_NAME)$(RESET)]

### END OF COLORS ###

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@echo "$(TAG) building $(YELLOW)$(NAME)$(RESET).."
	@mkdir -p $(dir $@)
#	@mkdir -p $$(dirname $$(ar -t $(LIBFT_ARCH) | xargs) | awk "{print \"$(dir $@)$(LIBFT_PATH)/\""'$$1}' | xargs)
#	@ar -x $(LIBFT_ARCH) --output="$(dir $@)$(LIBFT_PATH)"
#	@find $(dir $@)$(LIBFT_PATH) -name '*.o' -exec ar -rcsP $@ {} \;
#	@cp $(LIBFT_ARCH) $@
	@ar -rcsP $@ $(OBJ_FILES)
	@echo "$(TAG) compiled version $(YELLOW)$$(cat VERSION)$(RESET)!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "$(TAG) compiling $(YELLOW)$<$(RESET).."
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	@mkdir -p $@
$(DEP_DIR):
	@mkdir -p $@

$(LIBFT_ARCH):
	@make -C $(LIBFT_PATH) --no-print-directory

$(DEP_FILES):

include $(wildcard $(DEP_FILES))

clean:
	@make -C $(LIBFT_PATH) clean --no-print-directory
	@rm -rf $(OBJ_DIR)
	@rm -rf $(DEP_DIR)
	@echo "$(TAG) cleaned $(YELLOW)objects$(RESET)!"

fclean: clean
	@make -C $(LIBFT_PATH) fclean --no-print-directory
	@rm -f $(NAME)
	@echo "$(TAG) cleaned $(YELLOW)executable$(RESET)!"


re: fclean
	@make -C $(LIBFT_PATH) re --no-print-directory --jobs=$(shell nproc)
	@make $(MAKE_MT) all --jobs=$(shell nproc) --output-sync=target --no-print-directory

watch:
	@while true; do \
		make $(MAKE_MT) all --no-print-directory --no-print; \
		inotifywait -qre close_write --exclude ".*\.d" $(SRCS) $(INCLUDES); \
		echo "$(TAG) $(YELLOW)recompiling$(RESET).."; \
	done

test: all $(LIBFT_ARCH)
	$(CC)  $(CFLAGS)  -o test tests/main.c -Lbin -L$(LIBFT_BIN) -lcli -lft

.PHONY: all clean fclean re

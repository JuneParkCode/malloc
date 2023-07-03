# DEFAULT COMPILE OPTIONS
CC = cc
CFLAGS = -Wall -Wextra -Werror

# ABOUT LIBRARY
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIB_NAME = ft_malloc_$(HOSTTYPE)
NAME = lib$(LIB_NAME).so

LIBFT_LIB_DIR = ./libft
LIBFT_FLAG =	-L $(LIBFT_LIB_DIR) -l ft

ifdef WITH_BONUS
	INC_FLAG =	-I sources/bonus \
							-I includes \
							-I $(LIBFT_LIB_DIR)/include
else
	INC_FLAG = 	-I sources/mandatory \
							-I $(LIBFT_LIB_DIR)/include
endif

# SOURCES
MANDATORY_DIR	= sources/mandatory
BONUS_DIR 		= sources/bonus
TEST_DIR		= sources/test

SRCS			= $(MANDATORY_DIR)/malloc.c \
						$(MANDATORY_DIR)/free.c \
						$(MANDATORY_DIR)/realloc.c \
						$(MANDATORY_DIR)/show_alloc_mem.c \
						$(MANDATORY_DIR)/arena.c \
						$(MANDATORY_DIR)/block.c \
						$(MANDATORY_DIR)/pool.c

MANDATORY_OBJS	= ${SRCS:.c=.o}
BONUS_SRCS		= $(BONUS_DIR)/malloc.c
BONUS_OBJS		= ${BONUS_SRCS:.c=.o}
TEST_SRCS		= $(TEST_DIR)/main.c

# MAKE OPTION
ifdef WITH_BONUS
    OBJ = $(BONUS_OBJS)
else
    OBJ = $(MANDATORY_OBJS)
endif

# ABOUT TEST
TEST_FILE_NAME	= test_malloc

# GENERATE OBJECT FILES
%.o		: %.c
		$(CC) -fPIC $(CFLAGS) $(INC_FLAG) -c $< -o $@ 

# MAKE all
all		: $(NAME)

bonus	:
	@make all WITH_BONUS=1

# MAKE LIB
$(NAME)	: $(OBJ)
	@make -C $(LIBFT_LIB_DIR)
	$(CC) -shared $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT_FLAG)

# MAKE CLEAN
clean	: 
	rm -rf $(MANDATORY_OBJS) $(BONUS_OBJS)
	@make clean -C $(LIBFT_LIB_DIR)

# MAKE FCLEAN
fclean	:
	@make clean
	@make fclean -C $(LIBFT_LIB_DIR)
	rm -rf $(NAME) $(TEST_FILE_NAME)

# MAKE RE
re		:
	@make fclean
	@make all
# MAKE TEST
test	:
	@make all
	$(CC)  $(TEST_SRCS) -o $(TEST_FILE_NAME) -L. -l$(LIB_NAME) $(INC_FLAG)
#	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_FILE_NAME) $(INC_FLAG)
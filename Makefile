# DEFAULT COMPILE OPTIONS
CC = cc
CFLAGS = -Wall -Wextra -Werror

# ABOUT LIBRARY
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIB_NAME = ft_malloc_$(HOSTTYPE)
NAME = lib$(LIB_NAME).so

ifdef WITH_BONUS
	INC_FLAG =	-I sources/bonus 
else
	INC_FLAG = 	-I sources/mandatory 
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
						$(MANDATORY_DIR)/pool.c \
						$(MANDATORY_DIR)/ft_memcpy.c \
						$(MANDATORY_DIR)/ft_print.c \
						$(MANDATORY_DIR)/ft_putaddr.c \
						$(MANDATORY_DIR)/ft_putchar.c \
						$(MANDATORY_DIR)/ft_puthex.c \
						$(MANDATORY_DIR)/ft_putnbr.c \
						$(MANDATORY_DIR)/ft_putstr.c \
						$(MANDATORY_DIR)/ft_strlen.c


MANDATORY_OBJS	= ${SRCS:.c=.o}
BONUS_SRCS		= $(BONUS_DIR)/malloc.c
BONUS_OBJS		= ${BONUS_SRCS:.c=.o}
TEST_SRCS		= $(TEST_DIR)/main.c \
							$(TEST_DIR)/test_malloc.c
TEST_OBJS		= ${TEST_SRCS:.c=.o}

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
	$(CC) -shared $(CFLAGS) $(OBJ) -o $(NAME)

# MAKE CLEAN
clean	: 
	rm -rf $(MANDATORY_OBJS) $(BONUS_OBJS)

# MAKE FCLEAN
fclean	:
	@make clean
	rm -rf $(NAME) $(TEST_FILE_NAME)

# MAKE RE
re		:
	@make fclean
	@make all

test_build : 
	$(CC)  $(TEST_SRCS) -o $(TEST_FILE_NAME) -L. -l$(LIB_NAME) $(INC_FLAG)

# MAKE TEST
test	:
	@make all
	@make test_build
#	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_FILE_NAME) $(INC_FLAG)
# DEFAULT COMPILE OPTIONS
CC = cc
CFLAGS = -Wall -Wextra -Werror

# ABOUT LIBRARY
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIB_NAME = ft_malloc_$(HOSTTYPE)
NAME = lib$(LIB_NAME).so

# SOURCES
SOURCE_DIR		= sources
TEST_DIR		= test
INC_DIR			= includes
INC_FLAG		= -I $(INC_DIR)

SRCS			= $(SOURCE_DIR)/malloc.c \
						$(SOURCE_DIR)/free.c \
						$(SOURCE_DIR)/realloc.c \
						$(SOURCE_DIR)/show_alloc_mem.c \
						$(SOURCE_DIR)/pmalloc.c \
						$(SOURCE_DIR)/debug.c \
						$(SOURCE_DIR)/pool_util.c \
						$(SOURCE_DIR)/pool.c \
						$(SOURCE_DIR)/avl.c \
						$(SOURCE_DIR)/ft_memcpy.c \
						$(SOURCE_DIR)/ft_print.c \
						$(SOURCE_DIR)/ft_putaddr.c \
						$(SOURCE_DIR)/ft_putchar.c \
						$(SOURCE_DIR)/ft_puthex.c \
						$(SOURCE_DIR)/ft_putnbr.c \
						$(SOURCE_DIR)/ft_putstr.c \
						$(SOURCE_DIR)/ft_strcat.c \
						$(SOURCE_DIR)/ft_strcpy.c \
						$(SOURCE_DIR)/ft_strlen.c
OBJECTS			= ${SRCS:.c=.o}
TEST_SRCS		= $(TEST_DIR)/main.c \
					$(TEST_DIR)/test_malloc.c
TEST_OBJS		= ${TEST_SRCS:.c=.o}

AVL_TEST_SRCS	= 	$(SOURCE_DIR)/avl.c \
					$(SOURCE_DIR)/ft_print.c \
					$(SOURCE_DIR)/ft_putaddr.c \
					$(SOURCE_DIR)/ft_putchar.c \
					$(SOURCE_DIR)/ft_puthex.c \
					$(SOURCE_DIR)/ft_putnbr.c \
					$(SOURCE_DIR)/ft_putstr.c \
					$(SOURCE_DIR)/ft_strlen.c \
					$(TEST_DIR)/avl_test.c \
					$(TEST_DIR)/util.c

# ABOUT TEST
TEST_FILE_NAME	= test_malloc

# GENERATE OBJECT FILES
%.o		: %.c
		$(CC) $(CFLAGS) $(INC_FLAG) -c $< -o $@ -fPIC

# MAKE all
all		: $(NAME)

# MAKE LIB
$(NAME)	: $(OBJECTS)
		$(CC) -o $(NAME) -shared $(CFLAGS) $(OBJECTS)
		strip -x $(NAME)


# MAKE CLEAN
clean	: 
	rm -rf $(OBJECTS) $(TEST_OBJS)

# MAKE FCLEAN
fclean	:
	@make clean
	rm -rf $(NAME) $(TEST_FILE_NAME)

# MAKE RE
re		:
	@make fclean
	@make all
	
test_build : 
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_FILE_NAME) -L. -l$(LIB_NAME) $(INC_FLAG)

# MAKE TEST
test_malloc	: fclean
	@make all
	@make test_build

# TEST AVL
avl :
	rm -f avl_test
	$(CC) $(AVL_TEST_SRCS) $(INC_FLAG) -o avl_test -g3 -D DEBUG

avl_fclean:
	rm -f avl_test
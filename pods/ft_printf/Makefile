.PHONY: all re fclean clean libs
NAME=libftprintf.a
CC=gcc
CFLAG=
CFLAG+= -Werror
CFLAG+= -Wextra
CFLAG+= -Wall
#CFLAG+= -fsanitize=address
SRCNAME= ft_print_hex ft_pars wchar_t_to_str ft_print_dec ft_str_is_null ft_convertissor ft_push_point ft_printf ft_print_flags ft_flags ft_rawtoi

LIBS= ./pods/libvec/libvec.a ./pods/libft/libft.a

CC_LIBS= make -C ./pods/libvec/; make -C ./pods/libft/;

INC_DIR_LIBS= -I ./pods/libvec/inc -I ./pods/libft/inc

INC_DIR=./inc/

OBJDIR=./obj/
SRCDIR=./src/

SRC= $(addprefix $(SRCDIR), $(addsuffix .c, $(SRCNAME)))

OBJ= $(addprefix $(OBJDIR), $(addsuffix .o, $(SRCNAME)))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ) $(LIBS)
	ar -rc tmp.a $(OBJ) $(INCDIR)
	libtool -static -o $(NAME) tmp.a $(LIBS)
	rm -rf tmp.a
	@(echo "[ \033[35m$(NAME)\033[00m ]")

$(LIBS): libs ;

libs:
	@$(CC_LIBS)


$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) $(CFLAG) -c $< -o $@ -I $(INC_DIR) $(INC_DIR_LIBS)
	@(echo "\033[32m$@\033[00m")

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJ) $(OBJDIR)
	@(echo "\033[32mcleaned\033[00m")

fclean: clean
	@rm -rf $(NAME)

re: fclean all

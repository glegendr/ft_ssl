.PHONY: all re fclean clean
NAME= ft_ssl
CC=gcc
CFLAG=
CFLAG+= -Werror
CFLAG+= -Wextra
CFLAG+= -Wall
#CFLAG+= -fsanitize=address
SRCNAME= main parsing get_all md5 pad_message sha256 sha224 sha512 sha384 generic base64 base64_url des-ecb

LIBS= ./pods/libvec/libvec.a ./pods/libft/libft.a

CC_LIBS= make -C ./pods/libvec/; make -C ./pods/libft/

INC_DIR_LIBS= -I ./pods/libvec/inc/ -I ./pods/libft/inc

INC_DIR=./inc/

OBJDIR=./obj/
SRCDIR=./src/

SRC= $(addprefix $(SRCDIR), $(addsuffix .c, $(SRCNAME)))

OBJ= $(addprefix $(OBJDIR), $(addsuffix .o, $(SRCNAME)))

all: $(NAME)

$(NAME): $(LIBS) $(OBJDIR) $(OBJ)
	@$(CC) -o $@ $(OBJ) $(LIBS)
	@(echo "[ \033[35m$@\033[00m ]")
	
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

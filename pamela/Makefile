RM	=	rm -f

NAME	=	pam_ela.so
SRC	=	src/pam_log.c \
		src/pamela.c

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-Wall -Wextra -Wno-unused-parameter -fPIC

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CC) $(OBJ) -shared -o $(NAME)
		@echo "Successfully built" $(NAME)

clean:
		@echo "Cleaning"
		$(RM) $(NAME) $(OBJ)

re:		clean all

test:		all
		@echo "Running tests"

check:
		@echo "Checking for installation"

install:	all
		@echo "Installing dependencies: cryptsetup"
		@apt-get -y install cryptsetup >/dev/null
		@cp $(NAME) /lib/security/ #Also add to conf

uninstall:
		@echo "Uninstalling"
		@true || apt-get -y autoremove cryptsetup
		@$(RM) /lib/security/$(NAME) #Also remove from conf

.PHONY:		all clean re test check install uninstall

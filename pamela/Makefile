RM		=	rm -f

NAME		=	pam_ela.so
SRC		=	src/pwd_utils.c \
			src/shell_utils.c \
			src/luks_utils.c \
			src/pamela.c

OBJ		=	$(SRC:.c=.o)

CPPFLAGS	=	-DDEBUG
CFLAGS		=	-Wall -Wextra -Wno-unused-parameter -fPIC

all:			$(NAME)

$(NAME):		$(OBJ)
			$(CC) $(OBJ) -shared -o $(NAME)
			@echo "Successfully built" $(NAME)

clean:
			@echo "Cleaning"
			$(RM) $(NAME) $(OBJ)

re:			clean all

test:			all
			@echo "Running tests"

INSTALLDIR	?=	/lib/security
INSTALLTARGET	=	$(INSTALLDIR)/$(NAME)

CONFDIR		=	/etc/pam.d/
AUTHCONF	=	$(CONFDIR)/common-auth
AUTHSETTINGS	=	auth required $(NAME)
PASSCONF	=	$(CONFDIR)/common-password
PASSSETTINGS	=	password required $(NAME)
SESSCONF	=	$(CONFDIR)/common-session
SESSSETTINGS	=	session required $(NAME)

check:
			@echo "Checking for installation"
			@echo -n "$(NAME): "
			@{ [ -f $(INSTALLTARGET) ] && echo "OK"; } || echo "KO"
			@echo "Checking for configuration files"
			@echo -n "auth: "
			@{ grep -sq "$(AUTHSETTINGS)" $(AUTHCONF) && echo "OK"; } || echo "KO"
			@echo -n "password: "
			@{ grep -sq "$(PASSSETTINGS)" $(PASSCONF) && echo "OK"; } || echo "KO"
			@echo -n "session: "
			@{ grep -sq "$(SESSSETTINGS)" $(SESSCONF) && echo "OK"; } || echo "KO"

install:		all
			@echo "Installing dependencies: cryptsetup"
			@apt-get -y install cryptsetup
			@ln -s /sbin/cryptsetup /usr/bin/cryptsetup || true
			@echo "Installing" $(NAME)
			@cp $(NAME) $(INSTALLTARGET)
			@chown root:root $(INSTALLTARGET)
			@chmod 755 $(INSTALLTARGET)
			@echo "Configuring" $(NAME)
			@echo >> $(AUTHCONF) && echo $(AUTHSETTINGS) >> $(AUTHCONF)
			@echo >> $(PASSCONF) && echo $(PASSSETTINGS) >> $(PASSCONF)
			@echo >> $(SESSCONF) && echo $(SESSSETTINGS) >> $(SESSCONF)

uninstall:
			@echo "Uninstalling"
			@apt-get -y remove cryptsetup
			@sed -i /"$(AUTHSETTINGS)"/d $(AUTHCONF)
			@sed -i /"$(PASSSETTINGS)"/d $(PASSCONF)
			@sed -i /"$(SESSSETTINGS)"/d $(SESSCONF)
			@$(RM) $(INSTALLTARGET)

.PHONY:			all clean re test check install uninstall

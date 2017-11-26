RM		=	rm -f

NAME		=	pam_ela.so
SRC		=	src/pwd_utils.c \
			src/shell_utils.c \
			src/luks_utils.c \
			src/pamela.c

OBJ		=	$(SRC:.c=.o)

LUKS_TEST	=	luksutils-test

LUKS_SRC	=	src/luks_utils.c \
			src/shell_utils.c \
			tests/luksutils-test.c

LUKS_OBJ	=	$(LUKS_SRC:.c=.o)

CPPFLAGS	=	-DDEBUG -Isrc
CFLAGS		=	-Wall -Wextra -Wno-unused-parameter -fPIC

all:			$(NAME)

$(NAME):		$(OBJ)
			$(CC) $(OBJ) -shared -o $(NAME)
			@echo "Successfully built" $(NAME)

clean:
			@echo "Cleaning"
			$(RM) $(NAME) $(OBJ) $(LUKS_TEST) $(LUKS_OBJ)

re:			clean all

TEST_USER	?=	$(shell whoami)

test:			$(LUKS_TEST)
			@echo "Running tests"
			@./luksutils-test $(TEST_USER) >/dev/null \
			&& echo "[OK] LUKS utils test passed" \
			|| echo "[KO] LUKS utils test failed"

$(LUKS_TEST):		$(LUKS_OBJ)
			$(CC) $(LUKS_OBJ) -o $(LUKS_TEST)


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

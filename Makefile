# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cacharle <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 04:14:24 by cacharle          #+#    #+#              #
#    Updated: 2020/04/03 13:50:17 by charles          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RM = rm -f
MAKE = make
DOXYGEN = doxygen
DOXYGEN_FILE = Doxyfile
DOC_DIR = doc

TESTEXEC = test.sh

LIBFTDIR = libft
INCLUDEDIR = include
SRCDIR = src
OBJDIR = obj
OBJDIRS = $(shell find $(SRCDIR) -type d | sed 's/src/$(OBJDIR)/')

INCLUDE = $(shell find $(INCLUDEDIR) -name "*.h")

SRC = $(shell find $(SRCDIR) -name "*.c")

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = gcc
CCFLAGS = -g -I$(LIBFTDIR)/include -I$(INCLUDEDIR) -Wall -Wextra #-Werror
LDFLAGS = -L$(LIBFTDIR) -lft

NAME = minishell

.PHONY: all
all: libft_all prebuild $(NAME)

.PHONY: test
test:
	@./$(TESTEXEC)

.PHONY: prebuild
prebuild:
	@for subdir in $(OBJDIRS); do echo "Making dir $$subdir"; mkdir -p $$subdir; done

$(NAME): $(OBJ)
	@echo "Linking: $@"
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDE)
	@echo "Compiling: $@"
	@$(CC) $(CCFLAGS) -c -o $@ $<

.PHONY: clean
clean: libft_clean
	@echo "Removing objects"
	@$(RM) -r $(OBJDIR)

.PHONY: fclean
fclean: libft_fclean
	@echo "Removing objects"
	@$(RM) -r $(OBJDIR)
	@echo "Removing exectable"
	@$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: relocal
relocal:
	@echo "Removing objects"
	@$(RM) -r $(OBJDIR)
	@echo "Removing exectable"
	@$(RM) $(NAME)
relocal: all

.PHONY: libft_all
libft_all:
	@$(MAKE) -C $(LIBFTDIR) all

.PHONY: libft_clean
libft_clean:
	@$(MAKE) -C $(LIBFTDIR) clean

.PHONY: libft_fclean
libft_fclean:
	@$(MAKE) -C $(LIBFTDIR) fclean

.PHONY: doc
doc:
	mkdir -p tmp
	for f in $(SRC) $(INCLUDE) $(shell find libft/src -name "*.c") $(shell find libft/include -name "*.h"); do mkdir -p tmp/`dirname $$f` && sed 's_^/\*$$_/**_' $$f > tmp/$$f; done
	$(DOXYGEN) $(DOXYGEN_FILE)

.PHONY: doc_clean
doc_clean:
	$(RM) -r $(DOC_DIR)
	$(RM) -r tmp

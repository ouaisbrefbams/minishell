# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cacharle <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 04:14:24 by cacharle          #+#    #+#              #
#    Updated: 2020/02/27 17:58:16 by cacharle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RM = rm -f
MAKE = make

TESTEXEC = test.sh

LIBFTDIR = libft
INCLUDEDIR = include
SRCDIR = src
OBJDIR = obj
OBJDIRS = $(shell find $(SRCDIR) -type d | sed 's/src/$(OBJDIR)/')

INCLUDEFILES = minishell.h
INCLUDE = $(addprefix $(INCLUDEDIR)/, $(INCLUDEFILES))

SRCFILES = $(shell find $(SRCDIR) -name "*.c")
SRC = $(SRCFILES)
# SRC = $(addprefix $(SRCDIR)/, $(SRCFILES))

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = gcc
CCFLAGS = -I$(LIBFTDIR)/include -I$(INCLUDEDIR) -Wall -Wextra #-Werror
LDFLAGS = -L$(LIBFTDIR) -lft

NAME = minishell

.PHONY: all
all: libft_all prebuild $(NAME)

.PHONY: test
test:
	./$(TESTEXEC)

.PHONY: prebuild
prebuild:
	@for subdir in $(OBJDIRS); do echo "Making dir $$subdir"; mkdir -p $$subdir; done

$(NAME): $(OBJ)
	@echo "Linking: $@"
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

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

.PHONY: libft_all
libft_all:
	$(MAKE) -C $(LIBFTDIR) all

.PHONY: libft_clean
libft_clean:
	$(MAKE) -C $(LIBFTDIR) clean

.PHONY: libft_fclean
libft_fclean:
	$(MAKE) -C $(LIBFTDIR) fclean

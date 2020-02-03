# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cacharle <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 04:14:24 by cacharle          #+#    #+#              #
#    Updated: 2020/02/03 04:14:25 by cacharle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RM = rm -f
MAKE = make

TESTEXEC = test.sh

LIBFTDIR = libft
INCLUDEDIR = include
SRCDIR = src
OBJDIR = obj

INCLUDEFILES = minishell.h
INCLUDE = $(addprefix $(INCLUDEDIR)/, $(INCLUDEFILES))

SRCFILES = main.c
SRC = $(addprefix $(SRCDIR)/, $(SRCFILES))

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = gcc
CCFLAGS = -I$(LIBFTDIR)/include -I$(INCLUDEDIR) -Wall -Wextra #-Werror
LDFLAGS = -L$(LIBFTDIR) -lft

NAME = minishell

.PHONY: all
all: libft_all make_obj_dir $(NAME)

.PHONY: test
test:
	./$(TESTEXEC)

.PHONY: make_obj_dir
make_obj_dir:
	@if [ ! -d "$(OBJDIR)" ]; then echo "Making object dir"; mkdir $(OBJDIR); fi

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

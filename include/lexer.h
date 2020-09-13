/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/19 10:51:26 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/13 20:33:53 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   lexer.h
** \brief  Lexer module
*/

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include "libft_lst.h"
# include "libft_str.h"

/*
** \brief                  token tags (type of token)
** \param TAG_AND          &&
** \param TAG_END          ;
** \param TAG_OR           ||
** \param TAG_PIPE         |
** \param TAG_REDIR_IN     <
** \param TAG_REDIR_OUT    >
** \param TAG_REDIR_APPEND >>
** \param TAG_PARENT_OPEN  (
** \param TAG_PARENT_CLOSE )
** \param TAG_STR          unquoted string
** \param TAG_STR_DOUBLE   string quoted with "
** \param TAG_STR_SINGLE   string quoted with '
** \param TAG_STICK        token is sticked to the next one
** \param TAG_IS_STR       all of string tags
** \param TAG_IS_REDIR     all of redirection tags
** \param TAG_IS_SEP       all of separator tags
*/

enum                    e_tok
{
	TAG_AND          = 1 << 0,
	TAG_END          = 1 << 1,
	TAG_OR           = 1 << 2,
	TAG_PIPE         = 1 << 3,
	TAG_REDIR_IN     = 1 << 4,
	TAG_REDIR_OUT    = 1 << 5,
	TAG_REDIR_APPEND = 1 << 6,
	TAG_PARENT_OPEN  = 1 << 7,
	TAG_PARENT_CLOSE = 1 << 8,
	TAG_STR          = 1 << 9,
	TAG_STR_DOUBLE   = 1 << 10,
	TAG_STR_SINGLE   = 1 << 11,
	TAG_STICK        = 1 << 12,

	TAG_IS_STR       = TAG_STR | TAG_STR_SINGLE | TAG_STR_DOUBLE,
    TAG_IS_REDIR     = TAG_REDIR_IN | TAG_REDIR_OUT | TAG_REDIR_APPEND,
    TAG_IS_SEP       = TAG_AND | TAG_END | TAG_OR | TAG_PIPE,
};

/*
** tok_lst.c
*/

/*
** \brief          Token list struct
** \param content  token string content
** \param next     next token in the list (NULL if end)
** \param tag      token tag
** \warning        DO NOT change the order of the fields
**                 (they are in the same order as t_ftlst so
**                 that t_ftlst functions can be used on this struct aswell)
*/

typedef struct			s_tok_lst
{
	char				*content;
	struct s_tok_lst	*next;
	enum e_tok			tag;
}						t_tok_lst;

t_tok_lst				*tok_lst_new(enum e_tok tag, char *content);
t_tok_lst				*tok_lst_new_until(enum e_tok tag, char *content, size_t n);
void					tok_lst_push_back(t_tok_lst **tokens, t_tok_lst *pushed);
t_tok_lst				*tok_lst_push_front(t_tok_lst **tokens, t_tok_lst *pushed);
void					*tok_lst_destroy(t_tok_lst **tokens, void (*del)(void*));
t_tok_lst				*tok_lst_last(t_tok_lst *tokens);
t_tok_lst				*tok_lst_uncons(t_tok_lst **tokens);

/*
** lexer.c
*/

int 					len_until_sep(char *input);
int						tok_len(char *input);
t_tok_lst				*create_token_list(char *input, t_tok_lst **lst);
t_tok_lst             	*lexer(char *input);
// int 					check_input_out(char *input);

/*
** utils.c
*/

enum e_tok      		tok_assign_tag(char *content);
enum e_tok 				tok_assign_stick(t_tok_lst *tok);
enum e_tok 				tok_assign_str(t_tok_lst *tok);
int                     lexer_sep(char input);
int                     lexer_space(char *input);
int                     quote_len(char *input, int i);

/*
** trim.c
*/

void					lexer_trim(t_tok_lst *lst);

#endif

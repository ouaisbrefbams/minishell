/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/19 10:51:26 by nahaddac          #+#    #+#             */
/*   Updated: 2020/08/27 09:57:03 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include "libft_lst.h"
# include "libft_str.h"
# include "minishell.h"

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

typedef struct
{
    enum e_tok    tag;
    char                *content;
}                       t_token;

t_ftlst                 *lexer(char *input);
enum e_tok        ret_token(char *input, int  i);
enum e_tok        ret_token_sep_redir_append(char *input, int i);

int                     lexer_sep(char input);
int                     lexer_check_between_quote(char *input, int i);
int                     lexer_space(char *input);


t_token					*push_token_enum(t_token *lst_token);

t_ftlst             	*lexer_trim_out(t_ftlst *lst);

/*
** token.c
*/

t_token					*token_new(enum e_tok tag, char *content);
t_token					*token_new_until(enum e_tok tag, char *content, int n);
void					token_destroy(t_token *token);
void					token_destroy_lst(t_ftlst *tokens);
void					token_destroy_lst2(t_ftlst *tokens1, t_ftlst *tokens2);
enum e_tok		token_tag(t_ftlst *token_lst);
void					token_set_tag(t_ftlst *token_lst, enum e_tok tag);
char					*token_content(t_ftlst *token_lst);
void					token_set_content(t_ftlst *token_lst, char *content);
void					token_set_contentf(t_ftlst *token_lst, char *content);

/*
** \warning DO NOT change the order of the fields
*/

typedef struct			s_tok_lst
{
	char				*content;
	struct s_tok_lst	*next;
	enum e_tok			tag;
}						t_tok_lst;

t_tok_lst				*tok_lst_new(enum e_tok tag, char *content);
t_tok_lst				*tok_lst_new_until(enum e_tok tag, char *content, size_t n);

#endif

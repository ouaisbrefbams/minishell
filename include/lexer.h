#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include "libft_lst.h"
# include "libft_str.h"
# include "minishell.h"

enum                    e_token_tag
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
    enum e_token_tag    tag;
    char                *content;
}                       t_token;

t_ftlst                 *lexer(char *input);
enum e_token_tag        ret_token(char *input, int  i);
enum e_token_tag        ret_token_sep_redir_append(char *input, int i);

int                     lexer_sep(char input);
int                     lexer_verif_entre_cote(char *input, int i);
int                     lexe_space(char *input);

t_token					*token_new(enum e_token_tag tag, char *content);
void					token_destroy(t_token *token);
void					token_destroy_lst(t_ftlst *tokens);
void					token_destroy_lst2(t_ftlst *tokens1, t_ftlst *tokens2);
t_token					*push_token_enum(t_token *lst_token);

t_ftlst             	*lexe_trim_out(t_ftlst *lst);

#endif

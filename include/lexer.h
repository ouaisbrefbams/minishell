
# include <stdio.h>
# include <stdlib.h>
# include "libft_lst.h"
# include "libft_str.h"


enum                    e_token_tag
{
 LTAG_AND = 1 << 0,
 LTAG_END = 1 << 1,
 LTAG_OR = 1 << 2,
 LTAG_PIPE = 1 << 3,
 LTAG_REDIR_IN = 1 << 4,
 LTAG_REDIR_OUT = 1 << 5,
 LTAG_REDIR_APPEND = 1 << 6,
 LTAG_PARENT_OPEN = 1 << 7,
 LTAG_PARENT_CLOSE = 1 << 8,
 LTAG_STR = 1 << 9,
 LTAG_STR_DOUBLE = 1 << 10,
 LTAG_STR_SINGLE = 1 << 11,
 LTAG_STICK = 1 << 12,
};

typedef struct          s_token
{
    enum e_token_tag    token;
    char                *value;

}                       t_token;

t_ftlst                 *lexer(char *input);
enum e_token_tag        ret_token(char *input, int  i);
enum e_token_tag        ret_token_sep_redir_append(char *input, int i);

int                     lexer_sep(char input);
int                     lexer_verif_entre_cote(char *input, int i);
int                     lexe_space(char *input);

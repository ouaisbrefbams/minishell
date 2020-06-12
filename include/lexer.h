#include "minishell.h"

// just to make iterpolation compile
//////////////////////////////////////
enum e_token_tag
{
	LTAG_STR        = 1 << 0,
	LTAG_STR_SINGLE = 1 << 1,
	LTAG_STR_DOUBLE = 1 << 2,
	LTAG_STICK      = 1 << 3,
};

typedef struct
{
	enum e_token_tag	tag;
	char				*content;
}						t_token;

t_token		*token_new(enum e_token_tag tag, char *content);
void		token_destroy(t_token *token);
//////////////////////////////////////

char        **lexer(char *input);
int         lexer_sep(char input);
int         lexer_verif_entre_cote(char *input, int i);
int         lexer_verif_entre_cote22(char *input, int i);
int         lexer_count_nb_element(char *input);
int 		lexer_count_len_element(char *input, int i);
char 		**lexer_malloc_len_elem(char *input, int i, char **out);
